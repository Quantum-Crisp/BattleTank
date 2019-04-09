// Copyright Oliver Scott 2019

#include "TankAimComponent.h"
#include "Classes/Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTankAimComponent::UTankAimComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimComponent::SetBarrelReference(UStaticMeshComponent* BarrelToSet)
{
	Barrel = BarrelToSet;
}

void UTankAimComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	if (!Barrel)
		return;

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	//Calculate the OutLaunchVelocity
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(this, OutLaunchVelocity, StartLocation,
		HitLocation, LaunchSpeed, ESuggestProjVelocityTraceOption::DoNotTrace);
	// If we were able to calculate the velocity...
	if (bHaveAimSolution)
	{
		auto AimDirection = OutLaunchVelocity.GetSafeNormal(); //Turn our velocity into unit vector
		MoveBarrelTowards(AimDirection);
	}

}

void UTankAimComponent::MoveBarrelTowards(FVector TargetLocation)
{
	// Work out difference between current rotation and target location
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = TargetLocation.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;
	UE_LOG(LogTemp, Warning, TEXT("AimAsRotator: %s"), *AimAsRotator.ToString())
		
	// Move the barrel the right amount this frame 

	//given max speed and frame time.
}
