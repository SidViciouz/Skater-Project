// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Boarder.h"
#include "BoarderAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SKATER_API UBoarderAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	/** UAnimInstance interface begin */
	virtual void NativeInitializeAnimation() override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	/** UAnimInstance interface end */

	TObjectPtr<ABoarder> Boarder;

	UPROPERTY(Category = Movement, BlueprintReadWrite, VisibleAnywhere)
	float CurrentSpeed;

	UPROPERTY(Category = Movement, BlueprintReadWrite, VisibleAnywhere)
	bool bIsFalling;

	UPROPERTY(Category = Movement, BlueprintReadWrite, VisibleAnywhere)
	bool bIsJumping;

	UPROPERTY(Category = Movement, BlueprintReadWrite, VisibleAnywhere)
	bool bOnBoard;

	UPROPERTY(Category = OnBoardAnim, BlueprintReadWrite, VisibleAnywhere)
	bool bTurnSlowAnim;

	UPROPERTY(Category = OnBoardAnim, BlueprintReadWrite, VisibleAnywhere)
	bool bSit;

	UPROPERTY(Category = OnBoardAnim, BlueprintReadWrite, VisibleAnywhere)
	bool bOllie;

	UPROPERTY(Category = OnBoardAnim, BlueprintReadWrite, VisibleAnywhere)
	bool bGrind;
};
