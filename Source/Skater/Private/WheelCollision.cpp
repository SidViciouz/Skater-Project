// Fill out your copyright notice in the Description page of Project Settings.


#include "WheelCollision.h"

void UWheelCollision::SetWheelBoneName(FName NewName)
{
	WheelBoneName = NewName;

	SetCollisionProfileName(FName("OverlapAll"));
	SetSphereRadius(3.0f);
	SetGenerateOverlapEvents(true);
}


FName UWheelCollision::GetWheelBoneName() const
{
	return WheelBoneName;
}


void UWheelCollision::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	++bInTouchCount;
}


void UWheelCollision::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	--bInTouchCount;
}