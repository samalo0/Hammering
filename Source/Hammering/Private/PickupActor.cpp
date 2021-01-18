#include "PickupActor.h"

#include "HammeringStatics.h"

APickupActor::APickupActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetCollisionObjectType(ECC_Pickup);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->SetGenerateOverlapEvents(true);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Pickup, ECR_Block);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Hand, ECR_Overlap);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Nail, ECR_Overlap);
	StaticMeshComponent->SetAllUseCCD(true);	
	StaticMeshComponent->SetSimulatePhysics(true);
}

void APickupActor::GrabPressed(USceneComponent* AttachTo)
{
	StaticMeshComponent->SetSimulatePhysics(false);
	StaticMeshComponent->AttachToComponent(AttachTo, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void APickupActor::GrabReleased(const bool bSimulatePhysics)
{
	StaticMeshComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	if(bSimulatePhysics)
	{
		StaticMeshComponent->SetSimulatePhysics(true);	
	}
}
