// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialComponent.h"
#include "SkaterController.h"


// Sets default values for this component's properties
UTutorialComponent::UTutorialComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	static ConstructorHelpers::FObjectFinder<UDataTable> TutorialDataAsset(TEXT("/Script/Engine.DataTable'/Game/QuestSystem/TutorialData.TutorialData'"));
	static ConstructorHelpers::FClassFinder<UTutorialWidget> TutorialWidgetAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/QuestSystem/TutorialUI.TutorialUI_C'"));
	static ConstructorHelpers::FClassFinder<UGoalWidget> GoalWidgetAsset(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/QuestSystem/GoalCheckBoxUI.GoalCheckBoxUI_C'"));


	TutorialDataTable = TutorialDataAsset.Object;
	TutorialMainWidgetClass = TutorialWidgetAsset.Class;
	GoalWidgetClass = GoalWidgetAsset.Class;
}


// Called when the game starts
void UTutorialComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	TutorialMainWidget = CreateWidget<UTutorialWidget>(GetWorld(), TutorialMainWidgetClass);
	TutorialMainWidget->AddToViewport(9999);

	// Cache tutorial IDs
	TArray<FName> RowNames = TutorialDataTable->GetRowNames();
	for (auto& RowName : RowNames)
	{
		TutorialIDs.AddUnique(RowName);
	}

	CurrentTutorialIndex = -1;
	NextTutorial();
}


// Called every frame
void UTutorialComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}



void UTutorialComponent::NextTutorial()
{
	if (CurrentTutorialIndex < TutorialIDs.Num() - 1)
	{
		++CurrentTutorialIndex;

		FName TutorialName = TutorialIDs[CurrentTutorialIndex];

		CurrentTutorial = reinterpret_cast<FTutorialData*>(TutorialDataTable->GetRowMap()[TutorialName]);

		UGoalWidget* GoalWidget = CreateWidget<UGoalWidget>(GetWorld(), GoalWidgetClass);

		GoalWidget->SetGoal(CurrentTutorial->Description);

		TutorialMainWidget->AddGoal(GoalWidget);

		OnButtonPress.AddDynamic(GoalWidget, &UGoalWidget::OnButtonPressed);
	}
}