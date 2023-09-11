// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/SBoxPanel.h"
#include "Components/VerticalBox.h"
#include "Components/CheckBox.h"
#include "Components/TextBlock.h"
#include "TutorialWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class SKATER_API UGoalWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UCheckBox> CheckBoxRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UTextBlock> TextBlockRef;

	UFUNCTION()
	void SetGoal(const FText& Text);

	UFUNCTION()
	void OnButtonPressed(int32 Key);
};


UCLASS(Abstract)
class SKATER_API UTutorialWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UVerticalBox> GoalBoxRef;

	UFUNCTION()
	void AddGoal(UGoalWidget* Goal);

	UFUNCTION()
	void ClearGoal();

	UFUNCTION()
	UGoalWidget* GetGoal(int32 Index);
};
