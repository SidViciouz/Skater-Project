// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SkateBoardPawn.h"
#include "Boarder.h"
#include "TutorialComponent.h"
#include "SkaterController.generated.h"

/**
 * 
 */
UCLASS()
class SKATER_API ASkaterController : public APlayerController
{
	GENERATED_BODY()

public:
	ASkaterController();

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:

	UPROPERTY(Category = Control, VisibleAnywhere)
	ASkateBoardPawn* SkateBoard;

	UPROPERTY(Category = Control, VisibleAnywhere)
	ABoarder* Boarder;

	UPROPERTY(Category = Control, VisibleAnywhere)
	bool bControllingBoarder;

public:

	UFUNCTION(BlueprintCallable)
	void SwitchPossession();
};

