// Copyright Oliver Scott 2019

#include "TankAimComponent.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"

// Sets default values for this component's properties
UTankAimComponent::UTankAimComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UTankAimComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	if (!BarrelToSet) return;
	Barrel = BarrelToSet;
}

void UTankAimComponent::SetTurretReference(UTankTurret* TurretToSet)
{
	if (!TurretToSet) return;
	Turret = TurretToSet;
}

void UTankAimComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	if (!Barrel || !Turret)
		return;

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	//Calculate the OutLaunchVelocity
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(this, OutLaunchVelocity, StartLocation,
		HitLocation, LaunchSpeed, false, 0, 0, ESuggestProjVelocityTraceOption::DoNotTrace);
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
	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(DeltaRotator.Yaw);
}
