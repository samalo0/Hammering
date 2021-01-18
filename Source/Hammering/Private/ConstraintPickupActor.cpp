#include "ConstraintPickupActor.h"

#include "PhysicsEngine/PhysicsConstraintComponent.h"

AConstraintPickupActor::AConstraintPickupActor()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	PhysicsConstraintComponent = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraintComponent"));
	PhysicsConstraintComponent->SetupAttachment(GetRootComponent());
	PhysicsConstraintComponent->SetAngularSwing1Limit(ACM_Limited, 15.0f);
	PhysicsConstraintComponent->SetAngularSwing2Limit(ACM_Limited, 10.0f);
	PhysicsConstraintComponent->SetAngularTwistLimit(ACM_Locked, 45.0f);
	PhysicsConstraintComponent->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
	PhysicsConstraintComponent->SetAngularVelocityDriveTwistAndSwing(false, true);
	PhysicsConstraintComponent->SetAngularOrientationDrive(true, false);
	PhysicsConstraintComponent->SetAngularDriveParams(2500.0f, 100.0f, 0.0f);
}

void AConstraintPickupActor::GrabPressed(USceneComponent* AttachTo)
{
	SetActorLocationAndRotation(AttachTo->GetComponentLocation(), AttachTo->GetComponentRotation());
	
	if(!StaticMeshComponent->IsSimulatingPhysics())
	{
		StaticMeshComponent->SetSimulatePhysics(true);
	}

	PhysicsConstraintComponent->ConstraintActor1 = this;
	PhysicsConstraintComponent->ConstraintActor2 = AttachTo->GetOwner();
	PhysicsConstraintComponent->SetConstrainedComponents(StaticMeshComponent, NAME_None, Cast<UPrimitiveComponent>(AttachTo), NAME_None);
}

void AConstraintPickupActor::GrabReleased(const bool bSimulatePhysics)
{
	PhysicsConstraintComponent->BreakConstraint();
}
