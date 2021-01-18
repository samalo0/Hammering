#include "HammeringPawnAnimInstance.h"

void FHammeringPawnAnimInstanceProxy::PreUpdate(UAnimInstance* InAnimInstance, const float DeltaSeconds)
{
    Super::PreUpdate(InAnimInstance, DeltaSeconds);

    UHammeringPawnAnimInstance* Instance = Cast<UHammeringPawnAnimInstance>(InAnimInstance);
    if(IsValid(Instance))
    {
        if(IsValid(Instance->Pawn))
        {
            const float DesiredValue = Instance->Pawn->GetGripState(Instance->bIsRightHanded); 
            Grip = FMath::FInterpConstantTo(Grip, DesiredValue, DeltaSeconds, Instance->InterpolationSpeed);
        }
    }
}

void UHammeringPawnAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    Pawn = Cast<AHammeringPawn>(TryGetPawnOwner());
}

