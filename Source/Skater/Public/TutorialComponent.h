// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "TutorialWidget.h"
#include "TutorialData.h"
#include "TutorialComponent.generated.h"


UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FButtonPressSignature, int32, Key);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SKATER_API UTutorialComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTutorialComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	/** Data */

	/** Tutorial quest data table */
	TObjectPtr<UDataTable> TutorialDataTable;

	TArray<FName> TutorialIDs;

	int32 CurrentTutorialIndex;

	FTutorialData* CurrentTutorial;

	/** UI */

	/** Tutorial widget class */
	TSubclassOf<UTutorialWidget> TutorialMainWidgetClass;

	/** Tutorial widget object */
	TObjectPtr<UTutorialWidget> TutorialMainWidget;

	/** Goal widget class */
	TSubclassOf<UGoalWidget> GoalWidgetClass;


public:
	/** Tutorial Delegates */
	FButtonPressSignature OnButtonPress;


	/** Method */

	UFUNCTION(BlueprintCallable)
	void NextTutorial();
};
