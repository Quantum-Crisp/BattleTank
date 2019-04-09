// Copyright Oliver Scott 2019

#include "TankAIController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	auto PlayerTank = GetPlayerTank();

	if (!PlayerTank)
		UE_LOG(LogTemp, Error, TEXT("AIController can't find the player tank."))
	else
		UE_LOG(LogTemp, Warning, TEXT("AIController found player: %s"), *(PlayerTank->GetName()))
}

void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (GetPlayerTank())
	{
		//Move towards the player

		//Aim towards the player
		GetControlledTank()->AimAt(GetPlayerTank()->GetActorLocation());
		//Fire if able / ready

	}

}

ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const
{
	auto playerPawn = GetWorld()->GetFirstPlayerController()->GetPawn(); //Find the pawn attached to the player controller.
	// Protect our pointer (not specifically needed here, but good practice)
	if (!playerPawn)
		return nullptr;
	return Cast<ATank>(playerPawn);
}
