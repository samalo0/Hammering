#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PickupInterface.h"
#include "HammeringPawn.generated.h"

class UCameraComponent;
class UMotionControllerComponent;
class USphereComponent;

constexpr float GripOpen = 0.0f;
constexpr float GripClosed = 1.0f;

UCLASS()
class HAMMERING_API AHammeringPawn : public APawn
{

	GENERATED_BODY()

public:

	AHammeringPawn();

	FORCEINLINE float GetGripState(const bool bIsRightHanded) const { return bIsRightHanded? RightGripState : LeftGripState; }
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	virtual void BeginPlay() override;

	static AActor* GetNearestOverlappingPickup(USphereComponent* Component);
	
	void OnGrabLeft();

	void OnGrabRight();

	void OnReleaseLeft();

	void OnReleaseRight();

	void OnTriggerPressLeft() { bLeftTriggerDown = true; }

	void OnTriggerReleaseLeft() { bLeftTriggerDown = false; }

	void OnTriggerPressRight() { bRightTriggerDown = true; }

	void OnTriggerReleaseRight() { bRightTriggerDown = false; }
	
	UPROPERTY(EditAnywhere)
	bool bShowDebug = false;

	IPickupInterface* LeftAttachedPickup;

	float LeftGripState = 0.0f;

	bool bLeftTriggerDown = false;
	
	IPickupInterface* RightAttachedPickup;

	float RightGripState = 0.0f;

	bool bRightTriggerDown = false;
	
#pragma region Components

protected:
	
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* LeftGrabSphere;
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* LeftMesh;

	UPROPERTY(VisibleAnywhere)
	UMotionControllerComponent* LeftMotionController;
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* RightGrabSphere;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* RightMesh;

	UPROPERTY(VisibleAnywhere)
	UMotionControllerComponent* RightMotionController;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootSceneComponent;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* VROffset; 
	
#pragma endregion 
	
};
