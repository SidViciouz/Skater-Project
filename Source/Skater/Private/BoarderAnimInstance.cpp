// Fill out your copyright notice in the Description page of Project Settings.


#include "BoarderAnimInstance.h"

void UBoarderAnimInstance::NativeInitializeAnimation()
{
	if (AActor* Actor = GetOwningActor())
	{
		Boarder = Cast<ABoarder>(Actor);
	}
}

void UBoarderAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if (Boarder)
	{
		CurrentSpeed = Boarder->MovementState.CurrentSpeed;
		bIsFalling = Boarder->MovementState.bIsFalling;
		bIsJumping = Boarder->MovementState.bIsJumping;

		bOnBoard = Boarder->bOnBoard;

		bTurnSlowAnim = Boarder->bTurnSlowAnim;
		bSit = Boarder->bSit;
		bOllie = Boarder->bOllie;
		bGrind = Boarder->bGrind;
	}
}