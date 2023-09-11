// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "WheelCollision.generated.h"

/**
 * 
 */
UCLASS()
class SKATER_API UWheelCollision : public USphereComponent
{
	GENERATED_BODY()

public:

	UPROPERTY(Category = Wheel, VisibleAnywhere, BlueprintReadOnly)
	FName WheelBoneName;

	UPROPERTY(Category = Wheel, VisibleAnywhere, BlueprintReadOnly)
	int32 bInTouchCount;

	UFUNCTION()
	void SetWheelBoneName(FName NewName);

	UFUNCTION()
	FName GetWheelBoneName() const;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
