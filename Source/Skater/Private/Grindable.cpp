// Fill out your copyright notice in the Description page of Project Settings.


#include "Grindable.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/Class.h"
#include "SkateBoardPawn.h"


// Sets default values
AGrindable::AGrindable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Static Mesh"));
	SetRootComponent(StaticMesh);

	Spline = CreateDefaultSubobject<USplineComponent>(FName("Spline"));
	Spline->SetupAttachment(StaticMesh);
}

// Called when the game starts or when spawned
void AGrindable::BeginPlay()
{
	Super::BeginPlay();
	
	if (AActor* BoardActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASkateBoardPawn::StaticClass()))
	{
		if (ASkateBoardPawn* SkateBoard = Cast<ASkateBoardPawn>(BoardActor))
		{
			OnTouch.BindUFunction(SkateBoard, FName("SetGrindable"));
			OnUntouch.BindUFunction(SkateBoard, FName("ResetGrindable"));
		}
	}
}

// Called every frame
void AGrindable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckOverlap();
}

void AGrindable::CheckOverlap()
{
	bool bHit = false;

	if (UWorld* World = GetWorld())
	{
		if (StaticMesh->GetStaticMesh())
		{
			FVector Start = Spline->GetLocationAtDistanceAlongSpline(0, ESplineCoordinateSpace::World);
			FVector End = Spline->GetLocationAtDistanceAlongSpline(Spline->GetSplineLength(), ESplineCoordinateSpace::World);

			FHitResult HitResult;

			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);

			World->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_Camera, FCollisionShape::MakeCapsule(30.0f, 0.0f), QueryParams);

			if (HitResult.bBlockingHit && HitResult.GetActor()->GetClass()->IsChildOf(ASkateBoardPawn::StaticClass()))
			{
				bHit = true;
				OnTouch.ExecuteIfBound(this,HitResult.Distance);
			}
		}
	}
	if (!bHit)
		OnUntouch.ExecuteIfBound();
}