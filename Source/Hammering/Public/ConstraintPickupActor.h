#pragma once

#include "CoreMinimal.h"
#include "PickupActor.h"
#include "ConstraintPickupActor.generated.h"

class UPhysicsConstraintComponent;

UCLASS()
class HAMMERING_API AConstraintPickupActor : public APickupActor
{

	GENERATED_BODY()
	
public:	

	AConstraintPickupActor();

	virtual void GrabPressed(USceneComponent* AttachTo) override;

	virtual void GrabReleased(bool bSimulatePhysics) override;
	
protected:

	UPROPERTY(VisibleAnywhere)
	UPhysicsConstraintComponent* PhysicsConstraintComponent;
	
};
