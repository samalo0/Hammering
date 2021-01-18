#include "HammeringPawn.h"

#include "Camera/CameraComponent.h"
#include "HammeringStatics.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "MotionControllerComponent.h"
#include "PickupInterface.h"
#include "Components/SphereComponent.h"

AHammeringPawn::AHammeringPawn()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	SetRootComponent(RootSceneComponent);
	
	VROffset = CreateDefaultSubobject<USceneComponent>(TEXT("VROffset"));
	VROffset->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(VROffset);

	LeftMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftMotionController"));
	LeftMotionController->SetupAttachment(VROffset);
	LeftMotionController->MotionSource = FName("Left");
	LeftMotionController->SetCollisionProfileName("NoCollision");
	LeftMotionController->SetGenerateOverlapEvents(false);
	
	LeftMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftMesh"));
	LeftMesh->SetupAttachment(LeftMotionController);
	LeftMesh->SetCastShadow(false);
	
	LeftGrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LeftGrabSphere"));
	LeftGrabSphere->SetupAttachment(LeftMesh);
	LeftGrabSphere->SetSphereRadius(7.0f);
	LeftGrabSphere->ShapeColor = FColor::Blue;
	LeftGrabSphere->SetCollisionObjectType(ECC_Hand);
	LeftGrabSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LeftGrabSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	LeftGrabSphere->SetCollisionResponseToChannel(ECC_Pickup, ECR_Overlap);
	
	RightMotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightMotionController"));
	RightMotionController->SetupAttachment(VROffset);
	RightMotionController->MotionSource = FName("Right");
	RightMotionController->SetCollisionProfileName("NoCollision");
	RightMotionController->SetGenerateOverlapEvents(false);

	RightMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightMesh"));
	RightMesh->SetupAttachment(RightMotionController);
	RightMesh->SetCastShadow(false);
	
	RightGrabSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RightGrabSphere"));
	RightGrabSphere->SetupAttachment(RightMesh);
	RightGrabSphere->SetSphereRadius(7.0f);
	RightGrabSphere->ShapeColor = FColor::Red;
	RightGrabSphere->SetCollisionObjectType(ECC_Hand);
	RightGrabSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RightGrabSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	RightGrabSphere->SetCollisionResponseToChannel(ECC_Pickup, ECR_Overlap);
}

void AHammeringPawn::BeginPlay()
{
	Super::BeginPlay();

	UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	
	if(bShowDebug)
	{
		LeftGrabSphere->SetHiddenInGame(false);
		RightGrabSphere->SetHiddenInGame(false);
	}
}

AActor* AHammeringPawn::GetNearestOverlappingPickup(USphereComponent* Component)
{
	AActor* NearestPickup = nullptr;

	TArray<AActor*> OverlappingActors;
	Component->GetOverlappingActors(OverlappingActors);

	float ShortestDistance = TNumericLimits<float>::Max();
	for(AActor* Actor : OverlappingActors)
	{
		if(Actor->GetClass()->ImplementsInterface(UPickupInterface::StaticClass()))
		{
			const float Distance = FVector::Dist(Component->GetComponentLocation(), Actor->GetActorLocation());
			if(Distance < ShortestDistance)
			{
				ShortestDistance = Distance;
				NearestPickup = Actor;
			}
		}
	}

	return NearestPickup;
}

void AHammeringPawn::OnGrabLeft()
{
	LeftGripState = GripClosed;

	AActor* NearestPickup = GetNearestOverlappingPickup(LeftGrabSphere);

	LeftAttachedPickup = Cast<IPickupInterface>(NearestPickup);
	if(LeftAttachedPickup != nullptr)
	{
		LeftAttachedPickup->GrabPressed(LeftGrabSphere);
		if(LeftAttachedPickup->GetHideHandWhenPickedUp())
		{
			LeftMesh->SetHiddenInGame(true);
		}
		
		if(LeftAttachedPickup == RightAttachedPickup)
		{
			RightAttachedPickup = nullptr;
			RightGripState = GripOpen;
			RightMesh->SetHiddenInGame(false);
		}
	}
}

void AHammeringPawn::OnGrabRight()
{
	RightGripState = GripClosed;
	
	AActor* NearestPickup = GetNearestOverlappingPickup(RightGrabSphere);

	RightAttachedPickup = Cast<IPickupInterface>(NearestPickup);
	if(RightAttachedPickup != nullptr)
	{
		RightAttachedPickup->GrabPressed(RightGrabSphere);
		if(RightAttachedPickup->GetHideHandWhenPickedUp())
		{
			RightMesh->SetHiddenInGame(true);
		}

		if(RightAttachedPickup == LeftAttachedPickup)
		{
			LeftAttachedPickup = nullptr;
			LeftGripState = GripOpen;
			LeftMesh->SetHiddenInGame(false);
		}
	}
}

void AHammeringPawn::OnReleaseLeft()
{
	LeftGripState = GripOpen;

	if(LeftAttachedPickup != nullptr)
	{
		LeftAttachedPickup->GrabReleased(bLeftTriggerDown);
		LeftAttachedPickup = nullptr;
		LeftMesh->SetHiddenInGame(false);
	}
}

void AHammeringPawn::OnReleaseRight()
{
	RightGripState = GripOpen;

	if(RightAttachedPickup != nullptr)
	{
		RightAttachedPickup->GrabReleased(bRightTriggerDown);
		RightAttachedPickup = nullptr;
		RightMesh->SetHiddenInGame(false);
	}
}

void AHammeringPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("GrabLeft", EInputEvent::IE_Pressed, this, &AHammeringPawn::OnGrabLeft);
	PlayerInputComponent->BindAction("GrabLeft", EInputEvent::IE_Released, this, &AHammeringPawn::OnReleaseLeft);

	PlayerInputComponent->BindAction("GrabRight", EInputEvent::IE_Pressed, this, &AHammeringPawn::OnGrabRight);
	PlayerInputComponent->BindAction("GrabRight", EInputEvent::IE_Released, this, &AHammeringPawn::OnReleaseRight);

	PlayerInputComponent->BindAction("TriggerLeft", EInputEvent::IE_Pressed, this, &AHammeringPawn::OnTriggerPressLeft);
	PlayerInputComponent->BindAction("TriggerLeft", EInputEvent::IE_Released, this, &AHammeringPawn::OnTriggerReleaseLeft);
	
	PlayerInputComponent->BindAction("TriggerRight", EInputEvent::IE_Pressed, this, &AHammeringPawn::OnTriggerPressRight);
	PlayerInputComponent->BindAction("TriggerRight", EInputEvent::IE_Released, this, &AHammeringPawn::OnTriggerReleaseRight);

}

