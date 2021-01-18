#include "SpawnerActor.h"

#include "BoardPickupActor.h"

ASpawnerActor::ASpawnerActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void ASpawnerActor::ActorPickedUp(const int32 Index)
{
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("%s invalid world."), *GetNameSafe(this));
		return;
	}
	
	FTimerHandle TimerHandle;
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "RespawnObject", Index);		
	World->GetTimerManager().SetTimer(TimerHandle, Delegate, RespawnDelay, false);
}

void ASpawnerActor::BeginPlay()
{
	Super::BeginPlay();

	for(int32 Index = 0; Index < Tracking.Num(); Index++)
	{
		if(IsValid(Tracking[Index].InitialActor))
		{
			Tracking[Index].Transform = Tracking[Index].InitialActor->GetActorTransform();
			Tracking[Index].Class = Tracking[Index].InitialActor->GetClass();
			Tracking[Index].InitialActor->SetSpawnerIndex(Index);
			Tracking[Index].InitialActor->SetSpawnerActor(this);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%s actor at index %d is invalid."), *GetNameSafe(this), Index);
		}
	}
}

void ASpawnerActor::RespawnObject(const int32 Index)
{
	if(bShowDebug)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s respawning %d."), *GetNameSafe(this), Index);
	}
	
	UWorld* World = GetWorld();
	if(!IsValid(World))
	{
		UE_LOG(LogTemp, Error, TEXT("%s invalid world."), *GetNameSafe(this));
		return;
	}

	if(Index < 0 || Index >= Tracking.Num())
	{
		UE_LOG(LogTemp, Error, TEXT("%s index %d is invalid."), *GetNameSafe(this), Index);
		return;
	}
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.ObjectFlags |= EObjectFlags::RF_Transient;

	ABoardPickupActor* NewBoard = World->SpawnActor<ABoardPickupActor>(Tracking[Index].Class, Tracking[Index].Transform, SpawnParameters);
	if(IsValid(NewBoard))
	{
		NewBoard->SetSpawnerIndex(Index);
		NewBoard->SetSpawnerActor(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s unable to spawn new board."), *GetNameSafe(this), Index);
	}
}
