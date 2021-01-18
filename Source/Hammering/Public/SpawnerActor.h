#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnerActor.generated.h"

class ABoardPickupActor;

USTRUCT()
struct FSpawnTracking
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	ABoardPickupActor* InitialActor;

	UClass* Class;
	FTransform Transform;	
};

UCLASS(HideCategories = (Rendering, Replication, Collision, Input, Actor, LOD, Cooking))
class HAMMERING_API ASpawnerActor : public AActor
{

	GENERATED_BODY()
	
public:	

	ASpawnerActor();

	void ActorPickedUp(int32 Index);
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void RespawnObject(int32 Index);
	
	UPROPERTY(EditDefaultsOnly)
	float RespawnDelay = 3.0f;

	UPROPERTY(EditAnywhere)
	bool bShowDebug = false;
	
	UPROPERTY(EditInstanceOnly)
	TArray<FSpawnTracking> Tracking;
};
