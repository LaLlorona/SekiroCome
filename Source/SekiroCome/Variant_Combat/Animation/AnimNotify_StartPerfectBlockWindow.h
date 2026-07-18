// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_StartPerfectBlockWindow.generated.h"

/**
 *  AnimNotify to open the PerfectBlock/MasterStrike judgement window on the owning actor's attack montage (§5-1).
 */
UCLASS()
class UAnimNotify_StartPerfectBlockWindow : public UAnimNotify
{
	GENERATED_BODY()

public:

	/** Perform the Anim Notify */
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	/** Get the notify name */
	virtual FString GetNotifyName_Implementation() const override;
};
