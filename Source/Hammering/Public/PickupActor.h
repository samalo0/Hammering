#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickupInterface.h"
#include "PickupActor.generated.h"

UCLASS()
class HAMMERING_API APickupActor : public AActor, public IPickupInterface
{

	GENERATED_BODY()
	
public:	

	APickupActor();

	virtual bool GetHideHandWhenPickedUp() override { return bHideHandWhenPickedUp; }
	
	virtual void GrabPressed(USceneComponent* AttachTo) override;

	virtual void GrabReleased(bool bSimulatePhysics) override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly)
	bool bHideHandWhenPickedUp = true;
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;
	
};
