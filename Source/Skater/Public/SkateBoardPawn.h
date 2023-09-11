// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ControllerInterface.h"
#include "GameFramework/Pawn.h"
#include "SkateBoardMovementComponent.h"
#include "WheelCollision.h"
#include "Boarder.h"
#include "TutorialComponent.h"
#include "Grindable.h"
#include "Components/CustomPassComponent.h"
#include "SkateBoardPawn.generated.h"

UCLASS()
class SKATER_API ASkateBoardPawn : public APawn, public IControllerInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASkateBoardPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** IControllerInterface interface begin */
	virtual void Detach() override;

	virtual void AttachTo(USceneComponent* SceneComponent, FName SocketName = NAME_None) override;

	virtual void PresetTobeAttached() override;

	virtual void PresetToAttachTo() override;
	/** IControllerInterface interface end */

private:
	UPROPERTY(Category = SkateBoard, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkateSkeletalMesh;

	/** Custom physics movement*/
	UPROPERTY(Category = SkateBoard, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkateBoardMovementComponent> MovementComponent;

	/** Custom Rendering pass */
	UPROPERTY(Category = Rendering, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCustomPassComponent> CustomPass;

	void GrindUpdate(float DeltaTime);

	bool bRootMotion;
	
	void EnableRootMotion();

	void DisableRootMotion();


public:
	/* Name of skate baord skeletal mesh */
	static FName SkateSkeletalMeshName;

	/* Name of movement component mesh */
	static FName MovementComponentName;

	/* Name of Wheel FR */
	static FName WheelFRBoneName;

	/* Name of Wheel FL */
	static FName WheelFLBoneName;

	/* Name of Wheel RR */
	static FName WheelRRBoneName;

	/* Name of Wheel RL */
	static FName WheelRLBoneName;

	UPROPERTY(Category = SkateBoard, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TMap<FName,TObjectPtr<UWheelCollision>> WheelCollider;

	UPROPERTY(Category = Boarder, BlueprintReadWrite, VisibleAnywhere)
	TObjectPtr<ABoarder> Boarder;

	UFUNCTION(BlueprintCallable)
	void SetCustomPhysics(bool Value);

	UFUNCTION(BlueprintCallable)
	void SetLockRotation(bool Value);

	UFUNCTION(BlueprintCallable)
	void SetCollision(bool Value);

	UFUNCTION(BlueprintCallable, Category = Animation)
	float PlayAnimMontage(class UAnimMontage* AnimMontage, float InPlayRate = 1.f, FName StartSectionName = NAME_None);

	UFUNCTION(BlueprintCallable)
	void PlayPush();

	UFUNCTION(BlueprintCallable)
	void Push();

	UPROPERTY(Category = Animation, BlueprintReadWrite, VisibleAnywhere)
	TObjectPtr<UAnimMontage> KickflipMontage;

	UFUNCTION(BlueprintCallable)
	void PlayJump();

	UFUNCTION(BlueprintCallable)
	void Jump();

	UFUNCTION(BlueprintCallable)
	void Turn(float Direction);

	UPROPERTY(Category = Animation, BlueprintReadWrite, EditAnywhere)
	bool bSlowTurnAnim;

	UPROPERTY(Category = Animation, BlueprintReadWrite, EditAnywhere)
	bool bSlowTurnRight;


	// Trick animation

	UPROPERTY(Category = Animation, BlueprintReadWrite, EditAnywhere)
	bool bBoarderSit;

	UFUNCTION(BlueprintCallable)
	void Sit();

	UFUNCTION(BlueprintCallable)
	void Stand();

	UPROPERTY(Category = Animation, BlueprintReadWrite, EditAnywhere)
	bool bOllie;

	UFUNCTION(BlueprintCallable)
	void Ollie();

	UPROPERTY(Category = Animation, BlueprintReadWrite, EditAnywhere)
	bool bGrindable;

	UPROPERTY(Category = Animation, BlueprintReadWrite, EditAnywhere)
	bool bGrind;

	UPROPERTY(Category = Animation, BlueprintReadWrite, VisibleAnywhere)
	float GrindDistance;

	UPROPERTY(Category = Animation, BlueprintReadWrite, VisibleAnywhere)
	float GrindSpeed;

	UPROPERTY(Category = Animation, BlueprintReadWrite, EditAnywhere, Transient)
	TObjectPtr<AGrindable> GridnableObject;

	UFUNCTION(Category = Animation)
	void SetGrindable(AGrindable* Grindable, float Distance);

	UFUNCTION(Category = Animation)
	void ResetGrindable();

	UFUNCTION(Category = Animation)
	void Grind(float Distance);

	UFUNCTION(Category = Animation)
	void GrindOff();

	UPROPERTY(Category = Animation, BlueprintReadWrite, EditAnywhere)
	bool bPowerSlide;

	UFUNCTION(Category = Animation, BlueprintCallable)
	void PowerSlide();

	/** Tutorial UI */

	UPROPERTY(Category = Tutorial, BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTutorialComponent> TutorialComponent;
};
