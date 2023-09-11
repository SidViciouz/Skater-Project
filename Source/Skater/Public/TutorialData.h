// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/DataTable.h"
#include "TutorialData.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EGoal : uint8
{
	PressButton UMETA(DisplayName = "PressButton"),
	ReachLocation UMETA(DisplayName = "ReachLocation"),
};


USTRUCT(BlueprintType)
struct SKATER_API FTutorialData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tutorial)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Tutorial)
	EGoal Goal;
};
