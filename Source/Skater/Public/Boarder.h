// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControllerInterface.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Boarder.generated.h"

USTRUCT(BlueprintType)
struct SKATER_API FMovementState
{
	GENERATED_BODY()

public:

	FMovementState()
	{
		CurrentSpeed = 0;
		WalkSpeed = 5.0f;
		RunSpeed = 10.0f;
		bIsRun = false;
		bIsFalling = false;
		bIsLanding = false;
		InputDirection = FVector::ZeroVector;
		Direction = FVector::ZeroVector;
	}

	void Update(ACharacter* Character, float DeltaTime)
	{
		if (bIsLanding)
		{
			CurrentSpeed = 0;
			InputDirection = FVector::ZeroVector;
			Direction = FVector::ZeroVector;
			return;
		}

		if (InputDirection.Length() != 0)
		{
			InputDirection = InputDirection.GetSafeNormal();

			Direction = InputDirection;

			if(!bIsRun)
				CurrentSpeed = FMath::FInterpConstantTo(CurrentSpeed, WalkSpeed, DeltaTime, 1.0f);
			else
				CurrentSpeed = FMath::FInterpConstantTo(CurrentSpeed, RunSpeed, DeltaTime, 1.0f);
		}
		else
			CurrentSpeed = FMath::FInterpConstantTo(CurrentSpeed, 0, DeltaTime, 2.0f);

		if(Character)
			Character->AddMovementInput(Direction * CurrentSpeed);

		InputDirection = FVector::ZeroVector;
	}

	void SetIsRun()
	{
		bIsRun = true;
	}

	void ResetIsRun()
	{
		bIsRun = false;
	}

	void SetIsFalling()
	{
		bIsFalling = true;
	}

	void ResetIsFalling()
	{
		bIsFalling = false;
		ResetIsJumping();
	}

	void SetIsJumping()
	{
		bIsJumping = true;
	}

	void ResetIsJumping()
	{
		bIsJumping = false;
	}

	void SetIsLanding()
	{
		bIsLanding = true;
	}

	void ResetIsLanding()
	{
		bIsLanding = false;
	}

	void AddInputDirection(FVector Input)
	{
		InputDirection += Input;
	}

	UPROPERTY(transient)
	float CurrentSpeed;
	
	UPROPERTY()
	float WalkSpeed;

	UPROPERTY()
	float RunSpeed;

	UPROPERTY(BlueprintReadWrite)
	bool bIsRun;

	UPROPERTY(BlueprintReadWrite)
	bool bIsFalling;

	UPROPERTY(BlueprintReadWrite)
	bool bIsJumping;

	UPROPERTY(BlueprintReadWrite)
	bool bIsLanding;

	UPROPERTY()
	FVector Direction;

	UPROPERTY()
	FVector InputDirection;
};

UCLASS()
class SKATER_API ABoarder : public ACharacter, public IControllerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoarder();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float Value);

	void MoveRight(float Value);

	void TurnPitch(float Value);

	void TurnYaw(float Value);

	void SetIsRun();

	void ResetIsRun();

	/** ACharacter interface begin */
	virtual void Jump() override;
	/** ACharacter interface end */

	/** IControllerInterface interface begin */
	virtual void Detach() override;

	virtual void AttachTo(AActor* Actor, FName SocketName = NAME_None) override;

	virtual void PresetTobeAttached() override;

	virtual void PresetToAttachTo() override;
	/** IControllerInterface interface end */

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UCapsuleComponent* Capsule;

	UPROPERTY(Category = Camera, BlueprintReadWrite, VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(Category = Camera, BlueprintReadWrite, VisibleAnywhere)
	USpringArmComponent* SpringArm;

	/** Movement state when it's not on the board */
	UPROPERTY(Category = Movement, BlueprintReadWrite, VisibleAnywhere)
	FMovementState MovementState;

	/** Set on board */
	UPROPERTY(Category = Movement, BlueprintReadWrite, VisibleAnywhere)
	bool bOnBoard;

	UFUNCTION(Category = Movement)
	void SetOnBoard(bool Value);

	void UpdateRotation();

	/** Push Montage */
	UPROPERTY(Category = Animation, BlueprintReadWrite, VisibleAnywhere)
	TObjectPtr<UAnimMontage> PushMontage;

	UFUNCTION(Category = Animation)
	void PlayPush();

	/** Kickflip Montage */
	UPROPERTY(Category = Animation, BlueprintReadWrite, VisibleAnywhere)
	TObjectPtr<UAnimMontage> KickflipMontage;

	UFUNCTION(Category = Animation)
	void PlayJump();

	UPROPERTY(Category = Animation, BlueprintReadWrite, VisibleAnywhere)
	bool bTurnSlowAnim;

	UFUNCTION(Category = Animation)
	void SetTurnSlowAnim(bool Value);

	UPROPERTY(Category = Animation, BlueprintReadWrite, VisibleAnywhere)
	bool bSit;

	UFUNCTION(Category = Animation)
	void SetSit(bool Value);

	UPROPERTY(Category = Animation, BlueprintReadWrite, VisibleAnywhere)
	bool bOllie;

	UFUNCTION(Category = Animation, BlueprintCallable)
	void SetOllie(bool Value);

	UPROPERTY(Category = Animation, BlueprintReadWrite, VisibleAnywhere)
	bool bGrind;

	UFUNCTION(Category = Animation, BlueprintCallable)
	void SetGrind(bool Value);
};
