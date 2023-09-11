// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNode_SBController.h"
#include "SBAnimInstance.h"

void FAnimNode_SBController::Initialize_AnyThread(const FAnimationInitializeContext& Context)
{
	AnimInstanceProxy = static_cast<FSBAnimInstanceProxy*>(Context.AnimInstanceProxy);
}

void FAnimNode_SBController::InitializeBoneReferences(const FBoneContainer& RequiredBones)
{
	const TArray<FBoneAnimationData>& WheelAnimData = AnimInstanceProxy->GetWheelAnimData();
	const TArray<FBoneAnimationData>& TruckAnimData = AnimInstanceProxy->GetTruckAnimData();
	const FBoneAnimationData& DeckAnimData = AnimInstanceProxy->GetDeckAnimData();

	WheelBoneReference.Empty(WheelAnimData.Num());
	TruckBoneReference.Empty(TruckAnimData.Num());


	for (int32 WheelIdx = 0; WheelIdx < WheelAnimData.Num(); ++WheelIdx)
	{
		WheelBoneReference.AddZeroed();
		WheelBoneReference[WheelIdx] = WheelAnimData[WheelIdx].BoneName;
		WheelBoneReference[WheelIdx].Initialize(RequiredBones);
	}

	for (int32 TruckIdx = 0; TruckIdx < TruckAnimData.Num(); ++TruckIdx)
	{
		TruckBoneReference.AddZeroed();
		TruckBoneReference[TruckIdx] = TruckAnimData[TruckIdx].BoneName;
		TruckBoneReference[TruckIdx].Initialize(RequiredBones);
	}

	DeckBoneReference = DeckAnimData.BoneName;
	DeckBoneReference.Initialize(RequiredBones);
}

void FAnimNode_SBController::EvaluateSkeletalControl_AnyThread(FComponentSpacePoseContext& Output, TArray<FBoneTransform>& OutBoneTransforms)
{
	const FBoneAnimationData& DeckAnimData = AnimInstanceProxy->GetDeckAnimData();

	const FBoneContainer& BoneContainer = Output.Pose.GetPose().GetBoneContainer();

	if (DeckBoneReference.IsValidToEvaluate(BoneContainer))
	{
		FCompactPoseBoneIndex BoneIndex = DeckBoneReference.GetCompactPoseIndex(BoneContainer);

		FTransform OutputTransform = Output.Pose.GetComponentSpaceTransform(BoneIndex);

		FQuat BoneQuat(DeckAnimData.RotOffest);

		OutputTransform.SetRotation(BoneQuat * OutputTransform.GetRotation());

		OutBoneTransforms.Add(FBoneTransform(BoneIndex, OutputTransform));
	}
}

bool FAnimNode_SBController::IsValidToEvaluate(const USkeleton* Skeleton, const FBoneContainer& RequiredBones)
{
	for (auto& WheelRef : WheelBoneReference)
	{
		if (WheelRef.IsValidToEvaluate(RequiredBones))
			return true;
	}

	for (auto& TruckRef : TruckBoneReference)
	{
		if (TruckRef.IsValidToEvaluate(RequiredBones))
			return true;
	}

	if (DeckBoneReference.IsValidToEvaluate(RequiredBones))
		return true;

	return false;
}