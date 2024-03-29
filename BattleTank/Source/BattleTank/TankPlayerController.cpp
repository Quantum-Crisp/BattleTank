// Copyright Oliver Scott 2019

#include "TankPlayerController.h"
#include "TankAimComponent.h"
#include "Tank.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (!GetPawn()) { return; }
	auto AimingComp = GetPawn()->FindComponentByClass<UTankAimComponent>();
	if(ensure(AimingComp))
		FoundAimComponent(AimingComp);
}

void ATankPlayerController::SetPawn(APawn *InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }

		// Subscribe our method to tank's death event.
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossessedTankDeath);
	}
}

void ATankPlayerController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
	AimTowardsCrosshair();
}

void ATankPlayerController::OnPossessedTankDeath()
{
	if (!GetPawn()) { return; }
	StartSpectatingOnly();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetPawn()) { return; } //For example if not possessing.
	// Only do this function if we "own" a tank.
	auto AimingComp = GetPawn()->FindComponentByClass<UTankAimComponent>();
	if (!ensure(AimingComp)) { return; }

	FVector hitLocation; // An out parameter.
	bool bGotHitLocation = GetSightRayHitLocation(hitLocation);
	if (bGotHitLocation)
	{
		AimingComp->AimAt(hitLocation);
	}
}

// Get world location from crosshair(where are we looking), true if we hit something
bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	//Find Crosshair Position in pixel co-ordinates.
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenLocation = FVector2D((ViewportSizeX*CrosshairXLocation), (ViewportSizeY*CrosshariYLocation));

	// "De-project" the screen position of crosshair to a world direction
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection))
	{
		// Create a ray trace along the direction
		return GetLookVectorHitLocation(LookDirection, OutHitLocation);
	}
	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& LookDirection) const
{
	FVector CameraWorldLocation; //This is discarded (not needed).
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, CameraWorldLocation, LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& OutHitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + (LookDirection * LineTraceRange);

	if (GetWorld()->LineTraceSingleByChannel(HitResult,StartLocation,EndLocation,ECollisionChannel::ECC_Camera))
	{
		OutHitLocation = HitResult.Location;
		return true; //Line Trace Succeeds.
	}
	OutHitLocation = FVector(0.0);
	return false; //Line Trace Fails.
}