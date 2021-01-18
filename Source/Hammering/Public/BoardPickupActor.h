#pragma once

#include "CoreMinimal.h"
#include "PickupActor.h"
#include "SpawnerActor.h"

#include "BoardPickupActor.generated.h"

class UNailComponent;

UCLASS()
class HAMMERING_API ABoardPickupActor : public APickupActor
{

	GENERATED_BODY()

public:

	ABoardPickupActor();

	ABoardPickupActor* FindMasterBoard();

	virtual void GrabPressed(USceneComponent* AttachTo) override;

	virtual void GrabReleased(bool bSimulatePhysics) override;

	void SetSpawnerActor(ASpawnerActor* Spawner) { SpawnerActor = Spawner;}
	
	void SetSpawnerIndex(const int32 Index) { SpawnerIndex = Index; }
	
protected:

	bool bHasEverBeenPickedUp = false;
	
	UPROPERTY(VisibleAnywhere)
	UNailComponent* NailComponent1;

	UPROPERTY(VisibleAnywhere)
	UNailComponent* NailComponent2;

	UPROPERTY(VisibleAnywhere)
	UNailComponent* NailComponent3;

	UPROPERTY(VisibleAnywhere)
	UNailComponent* NailComponent4;

	UPROPERTY(Transient)
	ASpawnerActor* SpawnerActor;
	
	int32 SpawnerIndex = -1;
	
};
