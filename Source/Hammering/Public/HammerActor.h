#pragma once

#include "CoreMinimal.h"
#include "ConstraintPickupActor.h"
#include "HammerActor.generated.h"

class UBoxComponent;
class UDataTable;

UCLASS()
class HAMMERING_API AHammerActor : public AConstraintPickupActor
{

	GENERATED_BODY()

public:

	AHammerActor();

	virtual void Tick(float DeltaSeconds) override;
	
protected:

	virtual void BeginPlay() override;

	float GetAverageVelocity();
	
	FVector GetAverageVelocityVector();

	UFUNCTION()
	void OnBoxComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
    void OnStaticMeshComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(EditDefaultsOnly)
	UDataTable* AudioDataTable;
	
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* BoxComponent;

	UPROPERTY(EditDefaultsOnly)
	float CoolDownTime = 0.2f;
	
	UPROPERTY(EditDefaultsOnly)
	float HammerMinimumDriveVelocity = 200.0f;

	UPROPERTY(EditDefaultsOnly)
	float HammerSoundMinimumVelocity = 20.0f;

	UPROPERTY(EditDefaultsOnly)
	float HammerSoundMaximumVelocity = 300.0f;

	float LastHammerBodyHitTime = 0.0f;
	
	float LastHammerHeadHitTime = 0.0f;
	
	UPROPERTY(EditDefaultsOnly)
	USoundBase* NailSound;
	
	UPROPERTY(EditAnywhere)
	bool bShowDebug = false;
	
	TArray<FVector> VelocitySamples;

	UPROPERTY(EditDefaultsOnly)
	int32 VelocitySamplesCount = 5;
	
};

