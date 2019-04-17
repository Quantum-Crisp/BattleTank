// Copyright Oliver Scott 2019

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	// -1 = Max downward speed, +1 = Max upward speed
	void Elevate(float RelativeSpeed);

private:
	UPROPERTY(EditDefaultsOnly, Category = Setup)
		float MaxDegreesPerSecond = 10; //Sensible Default
	UPROPERTY(EditDefaultsOnly, Category = Setup)
		float MaxElevationDegrees = 40;
	UPROPERTY(EditDefaultsOnly, Category = Setup)
		float MinElevationDegrees = 0;
};
