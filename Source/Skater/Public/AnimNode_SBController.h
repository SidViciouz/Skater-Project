// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BoneControllers/AnimNode_SkeletalControlBase.h"
#include "SBAnimInstance.h"
#include "AnimNode_SBController.generated.h"

/**
 * 
 */
USTRUCT()
struct SKATER_API FAnimNode_SBController : public FAnimNode_SkeletalControlBase
{
	GENERATED_BODY()

public:
	/** FAnimNode_Base interface begin */
	virtual void Initialize_AnyThread(const FAnimationInitializeContext& Context) override;
	/** FAnimNode_Base interface end */

protected:

	TArray<FBoneReference> WheelBoneReference;
	TArray<FBoneReference> TruckBoneReference;
	FBoneReference DeckBoneReference;

	/** FAnimNode_SkeletalControlBase interface begin */
	virtual void InitializeBoneReferences(const FBoneContainer& RequiredBones) override;
	virtual void EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms) override;
	virtual bool IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones) override;
	/** FAnimNode_SkeletalControlBase interface end */


	FSBAnimInstanceProxy* AnimInstanceProxy;
};
