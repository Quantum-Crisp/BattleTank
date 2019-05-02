// Copyright Oliver Scott 2019

#include "TankAimComponent.h"
#include "Classes/Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "Projectile.h"
#include "TankTurret.h"

// Sets default values for this component's properties
UTankAimComponent::UTankAimComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimComponent::BeginPlay()
{
	//Prevent firing for the first few seconds.
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	if (RoundsLeft <= 0)
		FiringState = EFiringState::OutofAmmo;
	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSecs)
		FiringState = EFiringState::Reloading;
	else if (IsBarrelMoving())
		FiringState = EFiringState::Aiming;
	else
		FiringState = EFiringState::Locked;
}

void UTankAimComponent::Initialise(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

EFiringState UTankAimComponent::GetFiringState() const
{
	return FiringState;
}

int UTankAimComponent::GetRoundsLeft() const
{
	return RoundsLeft;
}

bool UTankAimComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }
	auto BarrelForward = Barrel->GetForwardVector();
	// if forward direction of barrel and aim forward direction are equal, the barrel isnt moving
	return !BarrelForward.Equals(AimDirection, 0.01);
}

void UTankAimComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel && Turret)) { return; }

	FVector OutLaunchVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));
	//Calculate the OutLaunchVelocity
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity(this, OutLaunchVelocity, StartLocation,
		HitLocation, LaunchSpeed, false, 0, 0, ESuggestProjVelocityTraceOption::DoNotTrace);
	// If we were able to calculate the velocity...
	if (bHaveAimSolution)
	{
		AimDirection = OutLaunchVelocity.GetSafeNormal(); //Turn our velocity into unit vector
		MoveBarrelTowards(AimDirection);
	}
}

void UTankAimComponent::MoveBarrelTowards(FVector TargetLocation)
{
	if (!ensure(Barrel && Turret)) { return; }

	// Work out difference between current rotation and target location
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimAsRotator = TargetLocation.Rotation();
	auto DeltaRotator = AimAsRotator - BarrelRotator;
	Barrel->Elevate(DeltaRotator.Pitch);
	if (FMath::Abs(DeltaRotator.Yaw) < 180)
		Turret->Rotate(DeltaRotator.Yaw);
	else
		Turret->Rotate(-DeltaRotator.Yaw);
}

void UTankAimComponent::Fire()
{
	if (FiringState == EFiringState::Aiming || FiringState == EFiringState::Locked)
	{
		if (!ensure(Barrel)) { return; }
		if (!ensure(ProjectileBlueprint)) { return; }
		// Spawn projectile at socket on barrel.
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBlueprint
			, Barrel->GetSocketLocation(FName("Projectile")), Barrel->GetSocketRotation(FName("Projectile")));

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
		RoundsLeft--;
	}
}
