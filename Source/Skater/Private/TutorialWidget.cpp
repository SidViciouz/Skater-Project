// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialWidget.h"

void UGoalWidget::SetGoal(const FText& Text)
{
	if(TextBlockRef)
		TextBlockRef->SetText(Text);
}

void UGoalWidget::OnButtonPressed(int32 Key)
{
	if (Key == 'W')
	{
		if (CheckBoxRef)
		{
			CheckBoxRef->SetCheckedState(ECheckBoxState::Checked);

		}
	}
}

void UTutorialWidget::AddGoal(UGoalWidget* Goal)
{
	if(GoalBoxRef)
		GoalBoxRef->AddChild(Goal);
}

UGoalWidget* UTutorialWidget::GetGoal(int32 Index)
{
	if (GoalBoxRef)
		return Cast<UGoalWidget>(GoalBoxRef->GetChildAt(Index));
	
	return nullptr;
}

void UTutorialWidget::ClearGoal()
{
	if (GoalBoxRef)
		GoalBoxRef->ClearChildren();
}