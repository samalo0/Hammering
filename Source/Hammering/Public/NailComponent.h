#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "NailComponent.generated.h"

/**
 * Child class of static mesh component, which contains nail specific functionality (drive).
 */
UCLASS()
class HAMMERING_API UNailComponent : public UStaticMeshComponent
{

	GENERATED_BODY()

public:

	UNailComponent();

	void DriveNail();

protected:

	void DisableNail();

	// How far to drive the nail when it is hit by the hammer.
	UPROPERTY(EditDefaultsOnly)
	FVector DriveDelta = FVector(0.0f, 0.0f, -6.0f);
	
};
