// Copyright Epic Games, Inc. All Rights Reserved.


#include "CombatCharacter.h"

#include "CombatAttackDirectionUI.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "CombatLifeBar.h"
#include "Engine/DamageEvents.h"
#include "TimerManager.h"
#include "Engine/LocalPlayer.h"
#include "CombatPlayerController.h"
#include "CombatLockOnComponent.h"
#include "CombatVitalityComponent.h"
#include "CombatLogic.h"
#include "CombatLogic/FAttackData.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/State/PlayerCombatStateMachineComponent.h"
#include "Player/PlayerComponent/CombatCharacterInputComponent.h"
#include "EnhancedPlayerInput.h"


ACombatCharacter::ACombatCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(35.0f, 90.0f);

	// Configure character movement
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	// create the camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);

	CameraBoom->TargetArmLength = DefaultCameraDistance;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;

	// create the orbiting camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// create the life bar widget component
	LifeBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("LifeBar"));
	LifeBar->SetupAttachment(RootComponent);
	LockOnComponent = CreateDefaultSubobject<UCombatLockOnComponent>(TEXT("LockOnComponent"));
	VitalityComponent = CreateDefaultSubobject<UCombatVitalityComponent>(TEXT("VitalityComponent"));
	CombatStateMachineComponent = CreateDefaultSubobject<UPlayerCombatStateMachineComponent>("CombatStateMachineComponent");
	CombatInputComponent = CreateDefaultSubobject<UCombatCharacterInputComponent>("CombatInputComponent");


	// set the player tag
	Tags.Add(FName("Player")); 
}

void ACombatCharacter::ToggleCamera()
{
	BP_ToggleCamera();
}

void ACombatCharacter::TryLockOnCamera()
{
	LockOnComponent->TryLockOn(GetFollowCamera()->GetForwardVector(), GetFollowCamera()->GetUpVector());
}

void ACombatCharacter::TryGuardStart()
{
	CombatStateMachineComponent->TryChangeStateByStateEnum(ECombatStateEnum::Guard);
	VitalityComponent->OnRegenStopTimerBegin();
}

void ACombatCharacter::TryGuardEnd()
{
	CombatStateMachineComponent->TryChangeStateByStateEnum(ECombatStateEnum::Idle);
}


void ACombatCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void ACombatCharacter::DoLook(float Yaw, float Pitch)
{
	auto lockOnResult = LockOnComponent->GetLockOnResult();
	if (!lockOnResult.IsType<FLockOnTargetNotExist>())
		return;

	if (GetController() != nullptr)
	{
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ACombatCharacter::OnAttackInputPressed()
{
	CombatStateMachineComponent->OnAttackInputPressed();
}

void ACombatCharacter::DoComboAttackEnd()
{
	// stub
}

bool ACombatCharacter::GetMoveAttackDirection(EAttackDirection& OutDirection) const
{
	return CombatInputComponent->GetMoveAttackDirection(OutDirection);
}

EAttackDirection ACombatCharacter::GetPreparedAttackDirection() const
{
	return CombatStateMachineComponent->GetPreparedAttackDirection();
}


void ACombatCharacter::ResetHP()
{
	// reset the current HP total
	VitalityComponent->ResetVitality();

	// update the life bar
	LifeBarWidget->SetLifePercentage(1.0f);
	LifeBarWidget->SetStaminaPercentage(1.0f);
}

void ACombatCharacter::ComboAttack()
{
	// enter (or re-enter) the Attack state, resetting its combo progress
	CombatStateMachineComponent->TryChangeStateByStateEnum(ECombatStateEnum::Attack);

	VitalityComponent->OnRegenStopTimerBegin();

	// notify enemies they are about to be attacked
	NotifyEnemiesOfIncomingAttack();

	// play the attack montage
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		const float MontageLength = AnimInstance->Montage_Play(ComboAttackMontage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);

		// subscribe to montage completed and interrupted events
		if (MontageLength > 0.0f)
		{
			// set the end delegate for the montage
			AnimInstance->Montage_SetEndDelegate(OnAttackMontageEnded, ComboAttackMontage);
		}

		// jump into the section matching the direction we're currently aiming
		if (const FName* SectionName = DirectionalAttackSections.Find(GetPreparedAttackDirection()))
		{
			AnimInstance->Montage_JumpToSection(*SectionName, ComboAttackMontage);
		}
	}
}

bool ACombatCharacter::CanParryNow() const
{
	return CombatStateMachineComponent->CanParryNow();
}

void ACombatCharacter::ChangeToRiposteState()
{
	CombatStateMachineComponent->TryChangeStateByStateEnum(ECombatStateEnum::PerfectParryRiposte);
}

void ACombatCharacter::DoAttackTrace(FName DamageSourceBone, EAttackDirection AttackDirection, EAttackType AttackType)
{
	TArray<FHitResult> OutHits;

	// start at the provided socket location, sweep forward
	const FVector TraceStart = GetMesh()->GetSocketLocation(DamageSourceBone);
	const FVector TraceEnd = TraceStart + (GetActorForwardVector() * MeleeTraceDistance);

	// check for pawn and world dynamic collision object types
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	// use a sphere shape for the sweep
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(MeleeTraceRadius);

	// ignore self
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->SweepMultiByObjectType(OutHits, TraceStart, TraceEnd, FQuat::Identity, ObjectParams, CollisionShape, QueryParams))
	{
		// iterate over each object hit
		for (const FHitResult& CurrentHit : OutHits)
		{
			// check if we've hit a damageable actor
			ICombatDamageable* Damageable = Cast<ICombatDamageable>(CurrentHit.GetActor());

			if (Damageable)
			{
				// knock upwards and away from the impact normal
				const FVector Impulse = (CurrentHit.ImpactNormal * -MeleeKnockbackImpulse) + (FVector::UpVector * MeleeLaunchImpulse);

				// pass the damage event to the actor
				{
					FAttackData AttackData(CurrentHit.ImpactPoint, Impulse, AttackDirection, AttackType);
					CombatLogic::ResolveAttack(this, this, CurrentHit.GetActor(), Damageable, AttackData);
				}
			}
		}
	}
}

void ACombatCharacter::CheckCombo()
{
	//CombatCharacter 에서는 일단 사용 안합니다.
}

void ACombatCharacter::CheckChargedAttack()
{
	//CombatCharacter 에서는 일단 사용 안합니다.
}

FWeaponId ACombatCharacter::GetWeaponID() const
{
	return WeaponID;
}

void ACombatCharacter::NotifyEnemiesOfIncomingAttack()
{
	// sweep for objects in front of the character to be hit by the attack
	TArray<FHitResult> OutHits;

	// start at the actor location, sweep forward
	const FVector TraceStart = GetActorLocation();
	const FVector TraceEnd = TraceStart + (GetActorForwardVector() * DangerTraceDistance);

	// check for pawn object types only
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);

	// use a sphere shape for the sweep
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(DangerTraceRadius);

	// ignore self
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	if (GetWorld()->SweepMultiByObjectType(OutHits, TraceStart, TraceEnd, FQuat::Identity, ObjectParams, CollisionShape, QueryParams))
	{
		// iterate over each object hit
		for (const FHitResult& CurrentHit : OutHits)
		{
			// check if we've hit a damageable actor
			ICombatDamageable* Damageable = Cast<ICombatDamageable>(CurrentHit.GetActor());

			if (Damageable)
			{
				// notify the enemy
				Damageable->NotifyDanger(GetActorLocation(), this);
			}
		}
	}
}

void ACombatCharacter::ApplyDamage(const FDamageData& DamageData, AActor* DamageCauser, const FVector& DamageLocation, const FVector& DamageImpulse)
{
	const float ActualDamage = ApplyDamageToVitality(DamageData);

	// only process knockback and effects if we received nonzero damage
	if (ActualDamage > 0.0f)
	{
		// apply the knockback impulse
		GetCharacterMovement()->AddImpulse(DamageImpulse, true);

		// is the character ragdolling?
		if (GetMesh()->IsSimulatingPhysics())
		{
			// apply an impulse to the ragdoll
			GetMesh()->AddImpulseAtLocation(DamageImpulse * GetMesh()->GetMass(), DamageLocation);
		}

		// pass control to BP to play effects, etc.
		ReceivedDamage(ActualDamage, DamageLocation, DamageImpulse.GetSafeNormal());
	}

}

void ACombatCharacter::HandleDeath()
{
	// disable movement while we're dead
	GetCharacterMovement()->DisableMovement();

	// enable full ragdoll physics
	GetMesh()->SetSimulatePhysics(true);

	// hide the life bar
	LifeBar->SetHiddenInGame(true);

	// pull back the camera
	GetCameraBoom()->TargetArmLength = DeathCameraDistance;

	// schedule respawning
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &ACombatCharacter::RespawnCharacter, RespawnTime, false);
}

void ACombatCharacter::ApplyHealing(float Healing, AActor* Healer)
{
	// stub
}

void ACombatCharacter::NotifyDanger(const FVector& DangerLocation, AActor* DangerSource)
{
	// stub
}

FArmorTypeId ACombatCharacter::GetArmorTypeID() const
{
	return ArmorTypeID;
}

void ACombatCharacter::RespawnCharacter()
{
	// destroy the character and let it be respawned by the Player Controller
	Destroy();
}

void ACombatCharacter::PlayMontage(UAnimMontage* Montage)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(Montage, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
	}
}

void ACombatCharacter::PlayMontageWithSectionName(UAnimMontage* Montage, const FName& SectionName, float MontagePlaySpeed)
{
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		AnimInstance->Montage_Play(Montage, MontagePlaySpeed, EMontagePlayReturnType::MontageLength, 0.0f, true);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

EAnimationStateEnum ACombatCharacter::GetCurrentAnimationState()
{
	if (CombatStateMachineComponent)
	{
		return CombatStateMachineComponent->GetAnimationStateEnum();
	}
	return EAnimationStateEnum::Normal;
	
}

float ACombatCharacter::ApplyDamageToVitality(const FDamageData& DamageData)
{
	// only process damage if the character is still alive
	if (!VitalityComponent->IsAlive())
	{
		return 0.0f;
	}

	// reduce the current HP (priority health bypasses stamina, remaining follows stamina-first/overflow)
	const float Damage = VitalityComponent->ApplyDamage(DamageData);

	// have we run out of HP?
	if (!VitalityComponent->IsAlive())
	{
		// die
		HandleDeath();
	}
	else
	{
		// update the life bar
		LifeBarWidget->SetLifePercentage(VitalityComponent->GetHPPercentage());
		LifeBarWidget->SetStaminaPercentage(VitalityComponent->GetStaminaPercentage());

		// enable partial ragdoll physics, but keep the pelvis vertical
		GetMesh()->SetPhysicsBlendWeight(0.5f);
		GetMesh()->SetBodySimulatePhysics(PelvisBoneName, false);

		// enter the Hit (stagger) state so a new attack can't be started mid-reaction
		CombatStateMachineComponent->TryChangeStateByStateEnum(ECombatStateEnum::Hit);
	}

	// return the received damage amount
	return Damage;
}

void ACombatCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	// is the character still alive?
	if (VitalityComponent->IsAlive())
	{
		// disable ragdoll physics
		GetMesh()->SetPhysicsBlendWeight(0.0f);
	}
}

void ACombatCharacter::BeginPlay()
{
	Super::BeginPlay();

	// get the life bar from the widget component
	LifeBarWidget = Cast<UCombatLifeBar>(LifeBar->GetUserWidgetObject());
	check(LifeBarWidget);

	// initialize the camera
	GetCameraBoom()->TargetArmLength = DefaultCameraDistance;

	// save the relative transform for the mesh so we can reset the ragdoll later
	MeshStartingTransform = GetMesh()->GetRelativeTransform();

	// set the life bar color
	LifeBarWidget->SetBarColor(LifeBarColor);


	// reset HP to maximum
	ResetHP();
	CombatStateMachineComponent->Initialize(FCombatStateComponentInitializeParameter(CombatMontageSet, this));
	CombatInputComponent->Initialize(this);
	VitalityComponent->Initialize(CombatTuningRowName);
}


void ACombatCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// clear the respawn timer
	GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);
}

void ACombatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		CombatInputComponent->SetupBindings(EnhancedInputComponent);
	}
}

void ACombatCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// update the respawn transform on the Player Controller
	if (ACombatPlayerController* PC = Cast<ACombatPlayerController>(GetController()))
	{
		PC->SetRespawnTransform(GetActorTransform());
	}
}

void ACombatCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LockOnComponent->CustomUpdate();
	FLockOnResult Result = LockOnComponent->GetLockOnResult();

	if (Result.IsType<FLockOnTargetExist>())
	{
		FVector TargetPos = Result.Get<FLockOnTargetExist>().TargetPosition;
		FVector CameraPos = GetFollowCamera() -> GetComponentLocation();

		FRotator TargetRotation = (UKismetMathLibrary::FindLookAtRotation(CameraPos, TargetPos));
		GetController()->SetControlRotation(TargetRotation);
	}
	EAttackDirection AttackDirection;
	const bool bDirectionDetected = Result.IsType<FLockOnTargetExist>()
	? CombatInputComponent->GetMouseAttackDirection(DeltaTime, AttackDirection)
		: false;

	if (bDirectionDetected)
	{
		CombatStateMachineComponent->SetAttackDirection(AttackDirection);
	}
	CombatStateMachineComponent->UpdateCombatState(DeltaTime);
	VitalityComponent->CustomUpdate(DeltaTime);
	LifeBarWidget->SetStaminaPercentage(VitalityComponent->GetStaminaPercentage());
}

