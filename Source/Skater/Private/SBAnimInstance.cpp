// Fill out your copyright notice in the Description page of Project Settings.


#include "SBAnimInstance.h"

void USBAnimInstance::NativeInitializeAnimation()
{
	if (AActor* Actor = GetOwningActor())
	{
		SkateBoard = Cast<ASkateBoardPawn>(Actor);
		SBMovementComponent = Actor->FindComponentByClass<USkateBoardMovementComponent>();
		AnimInstanceProxy.InitializeBoneAnimData(SBMovementComponent);
	}
}

FAnimInstanceProxy* USBAnimInstance::CreateAnimInstanceProxy()
{
	return &AnimInstanceProxy;
}

void USBAnimInstance::DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy)
{

}

void USBAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (SkateBoard)
	{
		bSlowTurn = SkateBoard->bSlowTurnAnim;
		bSlowTurnRight = SkateBoard->bSlowTurnRight;
		bOllie = SkateBoard->bOllie;
		bGrind = SkateBoard->bGrind;
		bPowerSlide = SkateBoard->bPowerSlide;
	}
}

void FSBAnimInstanceProxy::InitializeBoneAnimData(USkateBoardMovementComponent* MovementComponent)
{
	SBMovementComponent = MovementComponent;
	
	if (SBMovementComponent)
	{
		WheelAnimData.Empty(SBMovementComponent->WheelBoneName.Num());
		for (int32 WheelIdx = 0; WheelIdx < SBMovementComponent->WheelBoneName.Num(); ++WheelIdx)
		{
			WheelAnimData.AddZeroed();
			WheelAnimData[WheelIdx].BoneName = SBMovementComponent->WheelBoneName[WheelIdx];
			WheelAnimData[WheelIdx].RotOffest = FRotator::ZeroRotator;
		}

		TruckAnimData.Empty(SBMovementComponent->TruckBoneName.Num());
		for (int32 TruckIdx = 0; TruckIdx < SBMovementComponent->TruckBoneName.Num(); ++TruckIdx)
		{
			TruckAnimData.AddZeroed();
			TruckAnimData[TruckIdx].BoneName = SBMovementComponent->TruckBoneName[TruckIdx];
			TruckAnimData[TruckIdx].RotOffest = FRotator::ZeroRotator;
		}

		DeckAnimData.BoneName = SBMovementComponent->DeckBoneName;
		DeckAnimData.RotOffest = FRotator::ZeroRotator;
		DeckAnimData.LocOffset = FVector::ZeroVector;
	}
}

void FSBAnimInstanceProxy::PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds)
{
	Super::PreUpdate(InAnimInstance, DeltaSeconds);

	if (SBMovementComponent)
	{
		DeckAnimData.RotOffest = FRotator(0,0,SBMovementComponent->DeckData.RollDegree);
	}
}

const TArray<FBoneAnimationData>& FSBAnimInstanceProxy::GetWheelAnimData() const
{
	return WheelAnimData;
}

const TArray<FBoneAnimationData>& FSBAnimInstanceProxy::GetTruckAnimData() const
{
	return TruckAnimData;
}

const FBoneAnimationData& FSBAnimInstanceProxy::GetDeckAnimData() const
{
	return DeckAnimData;
}