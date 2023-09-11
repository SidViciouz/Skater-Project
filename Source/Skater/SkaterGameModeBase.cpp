// Copyright Epic Games, Inc. All Rights Reserved.


#include "SkaterGameModeBase.h"
#include "SkaterController.h"

ASkaterGameModeBase::ASkaterGameModeBase()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = ASkaterController::StaticClass();
}