// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_Push.h"
#include "Boarder.h"
#include "SkateBoardPawn.h"

void UAnimNotify_Push::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (AActor* Actor = MeshComp->GetOwner())
	{
		if (ABoarder* Boarder = Cast<ABoarder>(Actor))
		{
			if (AActor* ParentActor = Boarder->GetAttachParentActor())
			{
				if (ASkateBoardPawn* SkateBoard = Cast<ASkateBoardPawn>(ParentActor))
				{
					SkateBoard->Push();
				}
			}
		}
	}
}