#include "BoardPickupActor.h"

#include "NailComponent.h"

ABoardPickupActor::ABoardPickupActor()
{
    NailComponent1 = CreateDefaultSubobject<UNailComponent>(TEXT("NailComponent1"));
    NailComponent1->SetupAttachment(GetRootComponent());

    NailComponent2 = CreateDefaultSubobject<UNailComponent>(TEXT("NailComponent2"));
    NailComponent2->SetupAttachment(GetRootComponent());

    NailComponent3 = CreateDefaultSubobject<UNailComponent>(TEXT("NailComponent3"));
    NailComponent3->SetupAttachment(GetRootComponent());

    NailComponent4 = CreateDefaultSubobject<UNailComponent>(TEXT("NailComponent4"));
    NailComponent4->SetupAttachment(GetRootComponent());
}

ABoardPickupActor* ABoardPickupActor::FindMasterBoard()
{
    ABoardPickupActor* CurrentBoard = this;
    ABoardPickupActor* ParentBoard = this;

    while(IsValid(ParentBoard))
    {
        ParentBoard = Cast<ABoardPickupActor>(CurrentBoard->GetAttachParentActor());
        if(IsValid(ParentBoard))
        {
            CurrentBoard = ParentBoard;
        }
    }

    return CurrentBoard;
}

void ABoardPickupActor::GrabPressed(USceneComponent* AttachTo)
{
    if(!bHasEverBeenPickedUp)
    {
        bHasEverBeenPickedUp = true;
        if(IsValid(SpawnerActor))
        {
            SpawnerActor->ActorPickedUp(SpawnerIndex);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("%s spawner actor is invalid."), *GetNameSafe(this));
        }
    }

    ABoardPickupActor* MasterBoard = FindMasterBoard();
    
    if(this == MasterBoard)
    {
        StaticMeshComponent->SetSimulatePhysics(false);
        StaticMeshComponent->AttachToComponent(AttachTo, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
    }
    else
    {
        MasterBoard->GrabPressed(AttachTo);
    }
}

void ABoardPickupActor::GrabReleased(const bool bSimulatePhysics)
{
    ABoardPickupActor* MasterBoard = FindMasterBoard();

    if(this == MasterBoard)
    {
        DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        if(bSimulatePhysics)
        {
            StaticMeshComponent->SetSimulatePhysics(true);    
        }
    }
    else
    {
        MasterBoard->GrabReleased(bSimulatePhysics);
    }
}
