// Copyright Epic Games, Inc. All Rights Reserved.


#include "AnimNotify_FinishPerfectBlockWindow.h"
#include "CombatDamageable.h"
#include "Components/SkeletalMeshComponent.h"

void UAnimNotify_FinishPerfectBlockWindow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// cast the owner to the damageable interface
	if (ICombatDamageable* DamageableInterface = Cast<ICombatDamageable>(MeshComp->GetOwner()))
	{
		DamageableInterface->SetParryWindowOpen(false);
	}
}

FString UAnimNotify_FinishPerfectBlockWindow::GetNotifyName_Implementation() const
{
	return FString("Finish Perfect Block Window");
}
