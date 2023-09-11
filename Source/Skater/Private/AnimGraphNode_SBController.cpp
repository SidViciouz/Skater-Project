// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimGraphNode_SBController.h"

#define LOCTEXT_NAMESPACE "SBNamespace"

FText UAnimGraphNode_SBController::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	FText NodeTitle;
	if (TitleType == ENodeTitleType::ListView || TitleType == ENodeTitleType::MenuTitle)
	{
		NodeTitle = GetControllerDescription();
	}
	else
	{
		// we don't have any run-time information, so it's limited to print  
		// anymore than what it is it would be nice to print more data such as 
		// name of bones for wheels, but it's not available in Persona
		NodeTitle = FText(LOCTEXT("AnimGraphNode_SBController_Title", "SkateBoard Controller"));
	}
	return NodeTitle;
}

FText UAnimGraphNode_SBController::GetControllerDescription() const
{
	return LOCTEXT("AnimGraphNode_SBController", "Skateboard Controller for skateBoardPawn");
}

const FAnimNode_SkeletalControlBase* UAnimGraphNode_SBController::GetNode() const
{
	return &Node;
}


#undef LOCTEXT_NAMESPACE