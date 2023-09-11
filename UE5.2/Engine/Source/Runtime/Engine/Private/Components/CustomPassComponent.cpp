#include "Components/CustomPassComponent.h"

void UCustomPassComponent::SendRenderDynamicData_Concurrent()
{
	Super::SendRenderDynamicData_Concurrent();

	GetWorld()->Scene->UpdateCustomPassData(this);
}

void UCustomPassComponent::SetPassEnabled(bool bEnabled)
{
	CustomPassData.bEnabled = bEnabled;

	MarkRenderDynamicDataDirty();
}