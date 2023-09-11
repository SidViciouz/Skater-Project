// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Grindable.generated.h"

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE_TwoParams(FGrindSignature, AGrindable*, Grindable, float, Distance);

UDELEGATE()
DECLARE_DYNAMIC_DELEGATE(FGrindOffSignature);

UCLASS()
class SKATER_API AGrindable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrindable();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CheckOverlap();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Category = Components, BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(Category = Components, BlueprintReadWrite, VisibleAnywhere)
	USplineComponent* Spline;

	UPROPERTY(Category = Interaction, BlueprintReadWrite, VisibleAnywhere)
	FGrindSignature OnTouch;

	UPROPERTY(Category = Interaction, BlueprintReadWrite, VisibleAnywhere)
	FGrindOffSignature OnUntouch;
};
