// Copyright Epic Games, Inc. All Rights Reserved.


#include "AnimNotify_StartPerfectBlockWindow.h"
#include "CombatDamageable.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_StartPerfectBlockWindow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// cast the owner to the damageable interface
	if (ICombatDamageable* DamageableInterface = Cast<ICombatDamageable>(MeshComp->GetOwner()))
	{
		DamageableInterface->SetParryWindowOpen(true);
	}
}

FString UAnimNotify_StartPerfectBlockWindow::GetNotifyName_Implementation() const
{
	return FString("Start Perfect Block Window");
}
