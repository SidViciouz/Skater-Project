// Fill out your copyright notice in the Description page of Project Settings.


#include "SkaterController.h"
#include "Kismet/GameplayStatics.h"

ASkaterController::ASkaterController()
{

}

void ASkaterController::BeginPlay()
{
	AActor* BoardActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASkateBoardPawn::StaticClass());
	AActor* BoarderActor = UGameplayStatics::GetActorOfClass(GetWorld(), ABoarder::StaticClass());

	SkateBoard = Cast<ASkateBoardPawn>(BoardActor);
	Boarder = Cast<ABoarder>(BoarderActor);

	Boarder->AddTickPrerequisiteActor(SkateBoard);

	// store boarder reference for animation synchronization
	SkateBoard->Boarder = Boarder;

	bControllingBoarder = false;
	SwitchPossession();
}

void ASkaterController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(FName("Switch"), EInputEvent::IE_Pressed, this, &ASkaterController::SwitchPossession);
}


void ASkaterController::SwitchPossession()
{
	if (bControllingBoarder)
	{
		SkateBoard->Detach();
		SkateBoard->SetActorRotation(FRotator(0, Boarder->GetActorRotation().Yaw, 0));
		SkateBoard->PresetTobeAttached();

		Boarder->PresetToAttachTo();
		Boarder->AttachTo(SkateBoard, FName("Boarder"));

		Possess(SkateBoard);
		Boarder->SetOnBoard(true);

		bControllingBoarder = false;
	}
	else
	{
		Boarder->Detach();
		Boarder->SetActorRotation(FRotator(0, Boarder->GetActorRotation().Yaw, 0));
		Boarder->PresetTobeAttached();

		SkateBoard->PresetToAttachTo();
		SkateBoard->AttachTo(Boarder->GetComponentByClass<USkeletalMeshComponent>(), FName("Board"));

		Possess(Boarder);
		Boarder->SetOnBoard(false);

		bControllingBoarder = true;
	}
}