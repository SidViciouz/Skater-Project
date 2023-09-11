#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "CustomPassComponent.generated.h"

class FCustomPassSceneProxy;


class ENGINE_API FCustomPassData
{
public:
	bool bEnabled;
};


UCLASS()
class ENGINE_API UCustomPassComponent : public UPrimitiveComponent
{
	GENERATED_BODY()

protected:
	
	/** Called to send dynamic data for this component to the rendering thread */
	virtual void SendRenderDynamicData_Concurrent() override;

public:
	
	FCustomPassData CustomPassData;

	UFUNCTION(Category = Rendering, BlueprintCallable)
	void SetPassEnabled(bool bEnabled);
};
