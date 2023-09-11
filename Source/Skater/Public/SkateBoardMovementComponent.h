// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WheelCollision.h"
#include "SkateBoardMovementComponent.generated.h"

USTRUCT(BlueprintType)
struct SKATER_API FBodyData
{
	GENERATED_BODY()

	FBodyData()
	{
		bInTouch = false;
		WorldContactNormal = FVector(0, 0, 1);
		RelativeContactLocation = FVector::ZeroVector;
	}

	UPROPERTY()
	bool bInTouch;

	/* contains valid data only if bInTouch is true */
	UPROPERTY()
	FVector RelativeContactLocation;

	/* contains valid data only if bInTouch is true */
	UPROPERTY()
	FVector WorldContactNormal;
};

USTRUCT(BlueprintType)
struct SKATER_API FWheelData : public FBodyData
{
	GENERATED_BODY()

	FWheelData()
	{
		Mass = 1.0f;
		Force = FVector::ZeroVector;
		Velocity = FVector::ZeroVector;
		FrictionalCoef = 0.1f;
		StuckRecoveryHeight = 30.0f;
	}

	FVector NormalForce()
	{
		if (!bInTouch)
			return FVector::ZeroVector;

		return -Force.ProjectOnTo(WorldContactNormal);
	}

	FVector FrictionalForce(FVector NormalForce, FVector BodyForwardVector)
	{
		if(!bInTouch)
			return FVector::ZeroVector;

		FVector Direction = -Velocity;
		Direction.Normalize();

		FVector Forward = Direction.ProjectOnTo(BodyForwardVector);
		FVector Other = Direction - Forward;

		return Forward.GetSafeNormal() * NormalForce.Length() * FrictionalCoef + Other * 1000.0f;
	}

	UPROPERTY()
	FVector Force;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	float Mass;

	UPROPERTY()
	float FrictionalCoef;

	UPROPERTY()
	float StuckRecoveryHeight;
};

USTRUCT(BlueprintType)
struct SKATER_API FTruckData : public FBodyData
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct SKATER_API FDeckData : public FBodyData
{
	GENERATED_BODY()

	UPROPERTY()
	float RollDegree;
};

USTRUCT(BlueprintType)
struct SKATER_API FBodyPhysicsProperty
{
	GENERATED_BODY()

	FBodyPhysicsProperty()
	{
		Velocity = FVector::Zero();
		AngularVelocity = FVector::Zero();
		Force = FVector::Zero();
		Torque = FVector::Zero();
		Gravity = FVector(0, 0, -980.0f);
		Friction = 50.0f;
		RelativePivot = FVector::Zero();
		Mass = 4.0f;
		InvMass = 0.25f;
		InvInertia = 0.01f;
	}

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector Velocity;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector AngularVelocity;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector Force;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector Torque;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector Gravity;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float Friction;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector RelativePivot;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float Mass;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float InvMass;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float InvInertia;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FRotator PrevRotator;

	void ClearForce() { Force = FVector::Zero();}

	void ClearTorque() { Torque = FVector::Zero(); }
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKATER_API USkateBoardMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkateBoardMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Init();

	// Called every frame
	void PhysicsTick(float DeltaTime);

	// Called every frame
	void UpdateContact();

	// Calculate physics property about wheels 
	void WheelPhysics(float DeltaTime);

	// integrate wheel physics and calcualte physics property about body
	void BodyPhysics(float DeltaTime);

	// Clear body and wheels force
	void ClearForce();

	// rotate body when at least one wheel is in contact
	void RotateBody();

	// recover from stuck
	void RecoverFromStuck();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** The component that this movement will affect to */
	UPROPERTY(Category = Movement, BlueprintReadOnly, Transient)
	TObjectPtr<USceneComponent> AffectedComponent;

	/** Affected component casted to USkeletalMeshComponent */
	UPROPERTY(Category = Movement, BlueprintReadOnly, Transient)
	TObjectPtr<USkeletalMeshComponent> SkeletalComponent;

	/** Collision components around wheel, which is child of AffectedComponent */
	UPROPERTY(Category = Movement, BlueprintReadOnly, Transient)
	TMap<FName, TObjectPtr<UWheelCollision>> WheelCollision;
	
	/** Collision components around wheel, which is child of AffectedComponent */
	UPROPERTY(Category = BoneSetup, BlueprintReadOnly, EditAnywhere)
	TArray<FName> WheelBoneName;

	UPROPERTY(Category = BoneSetup, BlueprintReadOnly, EditAnywhere)
	TArray<FName> TruckBoneName;

	UPROPERTY(Category = BoneSetup, BlueprintReadOnly, EditAnywhere)
	FName DeckBoneName;

	/** Data about each part of body */
	TMap<FName,FWheelData> WheelData;
	TArray<FTruckData> TruckData;
	FDeckData DeckData;
	FBodyData BodyData;

	/** Data about physics property */
	UPROPERTY(Category = PhysicsProperty, BlueprintReadWrite, VisibleAnywhere)
	FBodyPhysicsProperty BodyPhysicsProperty;

	UPROPERTY(Category = PhysicsProperty, BlueprintReadWrite, VisibleAnywhere)
	bool bCustomPhysics;

	UPROPERTY(Category = PhysicsProperty, BlueprintReadWrite, VisibleAnywhere)
	bool bLockRotation;

	UPROPERTY(Category = PhysicsProperty, BlueprintReadWrite, VisibleAnywhere)
	bool bInAir;

	UFUNCTION(Category = PhysicsProperty, BlueprintCallable)
	float GetSpeed() const;

	UFUNCTION(Category = PhysicsProperty, BlueprintCallable)
	FVector GetVelocity() const;

	UFUNCTION(Category = PhysicsProperty, BlueprintCallable)
	void SetVelocity(FVector Velocity);

	UFUNCTION(Category = PhysicsProperty, BlueprintCallable)
	FVector GetGravity() const;

	/** Apply Force on this frame */
	UFUNCTION(Category = PhysicsProperty, BlueprintCallable)
	void ApplyForce(FVector Force, FVector PointOfActionLocation);

	/** Apply Force on this frame */
	UFUNCTION(Category = PhysicsProperty, BlueprintCallable)
	void ApplyImpulse(FVector Impulse, FVector PointOfActionLocation);

	UFUNCTION(Category = PhysicsProperty, BlueprintCallable)
	bool IsInAir();

	/** Triggered on hit event */
	UFUNCTION(Category = PhysicsProperty)
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Push this board forward*/
	UFUNCTION(Category = SBTrick, BlueprintCallable)
	void Push();

	/** Turn this board */
	UFUNCTION(Category = SBTrick, BlueprintCallable)
	void Turn(float Direction);

	/** Jump this board */
	UFUNCTION(Category = SBTrick, BlueprintCallable)
	void Jump();
};
