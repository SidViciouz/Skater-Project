// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ControllerInterface.generated.h"

/**
 * 
 */

UINTERFACE()
class SKATER_API UControllerInterface : public UInterface
{
	GENERATED_BODY()
};


class IControllerInterface
{
	GENERATED_BODY()

public:
	/** Detach this actor from parent */
	virtual void Detach() {}

	/** Attach this actor to other actor */
	virtual void AttachTo(AActor* Actor, FName SocketName) {}

	/** Attach this actor to other component */
	virtual void AttachTo(USceneComponent* SceneComponent, FName SocketName) {}

	/** prepare for this actor to be attached from other actor */
	virtual void PresetTobeAttached() {}

	/** prepare for this actor to attach to other actor */
	virtual void PresetToAttachTo() {}
};
