#include "TrashCanActor.h"

#include "Components/CapsuleComponent.h"
#include "HammerActor.h"
#include "HammeringStatics.h"

ATrashCanActor::ATrashCanActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);
	StaticMeshComponent->SetMobility(EComponentMobility::Static);
	StaticMeshComponent->SetCollisionObjectType(ECC_WorldStatic);
	StaticMeshComponent->SetGenerateOverlapEvents(false);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Pickup, ECR_Block);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Nail, ECR_Block);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(GetRootComponent());
	CapsuleComponent->SetMobility(EComponentMobility::Static);
	CapsuleComponent->SetCapsuleRadius(15.0f);
	CapsuleComponent->SetCapsuleHalfHeight(23.0f);
	CapsuleComponent->SetCollisionObjectType(ECC_Hand);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Pickup, ECR_Overlap);
}

void ATrashCanActor::BeginPlay()
{
	Super::BeginPlay();

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ATrashCanActor::OnCapsuleOverlap);
}

void ATrashCanActor::DestroyAttached(AActor* Actor)
{
	TArray<AActor*> AttachedActors;
	Actor->GetAttachedActors(AttachedActors);

	for(AActor* Child : AttachedActors)
	{
		if(IsValid(Child))
		{
			DestroyAttached(Child);
		}
	}

	Actor->Destroy();
}

void ATrashCanActor::OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AHammerActor* Hammer = Cast<AHammerActor>(OtherActor);
	if(IsValid(Hammer))
	{
		return;
	}

	DestroyAttached(OtherActor);
}
