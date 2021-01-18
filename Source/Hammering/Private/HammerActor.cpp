#include "HammerActor.h"

#include "AudioTableRow.h"
#include "BoardPickupActor.h"
#include "Components/BoxComponent.h"
#include "Engine/DataTable.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "HammeringStatics.h"
#include "NailComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

AHammerActor::AHammerActor()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;

    StaticMeshComponent->SetAllUseCCD(true);
    StaticMeshComponent->GetBodyInstance()->bNotifyRigidBodyCollision = true;
    
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    BoxComponent->SetupAttachment(GetRootComponent());
    BoxComponent->SetGenerateOverlapEvents(false);
    BoxComponent->GetBodyInstance()->bNotifyRigidBodyCollision = true;
    BoxComponent->SetCollisionObjectType(ECC_Hammer);
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
    BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    BoxComponent->SetCollisionResponseToChannel(ECC_Nail, ECR_Block);
    BoxComponent->SetAllUseCCD(true);
}

void AHammerActor::BeginPlay()
{
    Super::BeginPlay();

    VelocitySamples.Init(FVector::ZeroVector, VelocitySamplesCount);

    BoxComponent->OnComponentHit.AddDynamic(this, &AHammerActor::OnBoxComponentHit);

    StaticMeshComponent->OnComponentHit.AddDynamic(this, &AHammerActor::OnStaticMeshComponentHit);
}

float AHammerActor::GetAverageVelocity()
{
    return GetAverageVelocityVector().Size();
}

FVector AHammerActor::GetAverageVelocityVector()
{
    FVector Average = FVector::ZeroVector;

    for(const FVector Sample : VelocitySamples)
    {
        Average += Sample;
    }

    return Average / VelocitySamplesCount;
}

void AHammerActor::OnBoxComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if(bShowDebug)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s, %s hit %s, %s"), *GetNameSafe(this), *GetNameSafe(HitComponent), *GetNameSafe(OtherActor), *GetNameSafe(OtherComp));    
    }

    UWorld* World = GetWorld();
    if(!IsValid(World))
    {
        return;
    }

    const float CurrentTime = World->GetTimeSeconds();
    if((CurrentTime - LastHammerHeadHitTime) < CoolDownTime)
    {
        return;
    }

    const float HammerVelocity = FMath::Clamp(GetAverageVelocity(), 0.0f, HammerSoundMaximumVelocity);
    if(HammerVelocity < HammerSoundMinimumVelocity)
    {
        return;
    }

    const float Volume = FMath::Lerp(0.0f, 1.0f, (HammerVelocity - HammerSoundMinimumVelocity) / (HammerSoundMaximumVelocity - HammerSoundMinimumVelocity));
    
    ABoardPickupActor* Board = Cast<ABoardPickupActor>(OtherActor);
    UNailComponent* NailComponent = Cast<UNailComponent>(OtherComp);
    if(IsValid(Board) && IsValid(NailComponent))
    {
        if(HammerVelocity > HammerMinimumDriveVelocity)
        {
            UGameplayStatics::PlaySoundAtLocation(this, NailSound, GetActorLocation(), Volume);
            NailComponent->DriveNail();
            LastHammerHeadHitTime = CurrentTime;
        }
    }
}

void AHammerActor::OnStaticMeshComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if(bShowDebug)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s, %s hit %s, %s"), *GetNameSafe(this), *GetNameSafe(HitComponent), *GetNameSafe(OtherActor), *GetNameSafe(OtherComp));    
    }

    UWorld* World = GetWorld();
    if(!IsValid(World))
    {
        return;
    }

    const float CurrentTime = World->GetTimeSeconds();
    if((CurrentTime - LastHammerBodyHitTime) < CoolDownTime)
    {
        return;
    }

    const float HammerVelocity = FMath::Clamp(GetAverageVelocity(), 0.0f, HammerSoundMaximumVelocity);
    if(HammerVelocity < HammerSoundMinimumVelocity)
    {
        return;
    }

    LastHammerBodyHitTime = CurrentTime;
    
    UPhysicalMaterial* PhysicalMaterial = Hit.PhysMaterial.Get();
    if(!IsValid(PhysicalMaterial) && !IsValid(AudioDataTable))
    {
        UE_LOG(LogTemp, Warning, TEXT("%s physical material %s or audio data table %s invalid."), *GetNameSafe(this), *GetNameSafe(PhysicalMaterial), *GetNameSafe(AudioDataTable));
        return;
    }

    const FName RowName = FName(PhysicalMaterial->GetName());
    FAudioTableRow* Row = AudioDataTable->FindRow<FAudioTableRow>(RowName, "AudioDataTable");
    if(Row == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s unable to get row for name %s."), *GetNameSafe(this), *RowName.ToString());
        return;
    }

    const float Volume = FMath::Lerp(0.0f, 1.0f, (HammerVelocity - HammerSoundMinimumVelocity) / (HammerSoundMaximumVelocity - HammerSoundMinimumVelocity));

    if(bShowDebug)
    {
        UE_LOG(LogTemp, Warning, TEXT("%s playing sound %s at volume %s."), *GetNameSafe(this), *GetNameSafe(Row->Audio), *FString::SanitizeFloat(Volume));    
    }
    UGameplayStatics::PlaySoundAtLocation(this, Row->Audio, Hit.ImpactPoint, Volume);
}

void AHammerActor::Tick(const float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    VelocitySamples.RemoveAt(0, 1, false);
    VelocitySamples.Add(GetVelocity());

    if(bShowDebug)
    {
        UWorld* World = GetWorld();
        if(IsValid(World))
        {
            if(GEngine)
            {
                GEngine->AddOnScreenDebugMessage(INDEX_NONE, 0.0f, FColor::Red, FString::Printf(TEXT("Velocity %f"), GetAverageVelocity()));
            }
            DrawDebugDirectionalArrow(World, GetActorLocation(), GetActorLocation() + GetAverageVelocityVector(), 1.0f, FColor::Red);    
        }
    }
}
