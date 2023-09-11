// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "CoreMinimal.h"
#include "AnimGraphNode_SkeletalControlBase.h"
#include "AnimNode_SBController.h"
#include "AnimGraphNode_SBController.generated.h"

/**
 * 
 */


UCLASS()
class SKATER_API UAnimGraphNode_SBController : public UAnimGraphNode_SkeletalControlBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FAnimNode_SBController Node;

	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;

protected:

	virtual FText GetControllerDescription() const override;
	virtual const FAnimNode_SkeletalControlBase* GetNode() const override;
};
