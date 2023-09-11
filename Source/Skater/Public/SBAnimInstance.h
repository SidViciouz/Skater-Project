// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimInstanceProxy.h"
#include "SkateBoardMovementComponent.h"
#include "SkateBoardPawn.h"
#include "SBAnimInstance.generated.h"

/**
 * 
 */
struct FBoneAnimationData
{
	FName BoneName;
	FVector LocOffset;
	FRotator RotOffest;
};

USTRUCT()
struct SKATER_API FSBAnimInstanceProxy : public FAnimInstanceProxy
{
	GENERATED_BODY()

public:
	FSBAnimInstanceProxy()
		: FAnimInstanceProxy()
	{

	}

	FSBAnimInstanceProxy(UAnimInstance* Instance)
		: FAnimInstanceProxy(Instance)
	{

	}

	/** Set wheel, truck, deck bone name and location, rotation offset */
	void InitializeBoneAnimData(USkateBoardMovementComponent* MovementComponent);

	/** update animation data every frame */
	virtual void PreUpdate(UAnimInstance* InAnimInstance, float DeltaSeconds) override;

	const TArray<FBoneAnimationData>& GetWheelAnimData() const;
	const TArray<FBoneAnimationData>& GetTruckAnimData() const;
	const FBoneAnimationData& GetDeckAnimData() const;

private:

	/** Skateboard movement component reference */
	UPROPERTY()
	TObjectPtr<USkateBoardMovementComponent> SBMovementComponent;

	/**  Wheel animation data */
	TArray<FBoneAnimationData> WheelAnimData;

	/** Truck animation data */
	TArray<FBoneAnimationData> TruckAnimData;

	/** Deck animation data */
	FBoneAnimationData DeckAnimData;
};

UCLASS(Transient)
class SKATER_API USBAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:

	/** UAnimInstance interface begin */
	virtual void NativeInitializeAnimation() override;
	virtual FAnimInstanceProxy* CreateAnimInstanceProxy() override;
	virtual void DestroyAnimInstanceProxy(FAnimInstanceProxy* InProxy) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	/** UAnimInstance interface end */

private:
	
	FSBAnimInstanceProxy AnimInstanceProxy;

	UPROPERTY()
	TObjectPtr<USkateBoardMovementComponent> SBMovementComponent;

	UPROPERTY()
	TObjectPtr<ASkateBoardPawn> SkateBoard;

public:

	UPROPERTY(Category = Animation, BlueprintReadWrite, VisibleAnywhere)
	bool bSlowTurn;

	UPROPERTY(Category = Animation, BlueprintReadWrite, VisibleAnywhere)
	bool bSlowTurnRight;

	UPROPERTY(Category = Animation, BlueprintReadWrite, VisibleAnywhere)
	bool bOllie;

	UPROPERTY(Category = Animation, BlueprintReadWrite, VisibleAnywhere)
	bool bGrind;

	UPROPERTY(Category = Animation, BlueprintReadWrite, VisibleAnywhere)
	bool bPowerSlide;
};
