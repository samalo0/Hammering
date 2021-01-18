#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrashCanActor.generated.h"

class UCapsuleComponent;

UCLASS(HideCategories = (Replication))
class HAMMERING_API ATrashCanActor : public AActor
{

	GENERATED_BODY()
	
public:	

	ATrashCanActor();

protected:

	virtual void BeginPlay() override;

	static void DestroyAttached(AActor* Actor);
	
	UFUNCTION()
	void OnCapsuleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComponent;
};
