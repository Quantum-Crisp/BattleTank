// Copyright Oliver Scott 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankAimComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTankAimComponent();

	void AimAt(FVector HitLocation, float LaunchSpeed);

	// TODO: Add Set Turret Reference

	void SetBarrelReference(UStaticMeshComponent* BarrelToSet);

protected:


private:
	UStaticMeshComponent* Barrel = nullptr;

	void MoveBarrelTowards(FVector TargetLocation);
};
