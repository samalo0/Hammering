#include "NailComponent.h"

#include "BoardPickupActor.h"
#include "HammeringStatics.h"

UNailComponent::UNailComponent()
{
   PrimaryComponentTick.bCanEverTick = false;
   PrimaryComponentTick.bStartWithTickEnabled = false;

   SetCollisionObjectType(ECC_Nail);
   SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
   SetGenerateOverlapEvents(true);
   SetCollisionResponseToAllChannels(ECR_Ignore);
   SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
   SetCollisionResponseToChannel(ECC_Hammer, ECR_Block);
   SetCollisionResponseToChannel(ECC_Pickup, ECR_Block);
}

void UNailComponent::DisableNail()
{
   SetCollisionEnabled(ECollisionEnabled::NoCollision);
   SetGenerateOverlapEvents(false);
   SetCastShadow(false);
}

void UNailComponent::DriveNail()
{
   SetCollisionResponseToChannel(ECC_Pickup, ECR_Overlap);

   AddRelativeLocation(DriveDelta);

   TArray<AActor*> OverlappingActors;
   GetOverlappingActors(OverlappingActors, ABoardPickupActor::StaticClass());

   ABoardPickupActor* Owner = Cast<ABoardPickupActor>(GetOwner());
   ABoardPickupActor* OwnerMaster = Owner->FindMasterBoard();
   
   for(AActor* OverlappingActor : OverlappingActors)
   {
      ABoardPickupActor* Touching = Cast<ABoardPickupActor>(OverlappingActor);
      ABoardPickupActor* TouchingMaster = Touching->FindMasterBoard();

      if(IsValid(Touching) && Touching != Owner && Touching != OwnerMaster && OwnerMaster != TouchingMaster)
      {
         OwnerMaster->AttachToActor(Touching, FAttachmentTransformRules(EAttachmentRule::KeepWorld, true));
         break;
      }
   }

   DisableNail();
}

