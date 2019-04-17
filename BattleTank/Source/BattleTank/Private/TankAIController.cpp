// Copyright Oliver Scott 2019

#include "TankAIController.h"
#include "Tank.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//Find the pawn attached to the player controller.
	auto playerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	auto controlledTank = Cast<ATank>(GetPawn());

	if (playerTank)
	{
		//TMove towards the player
		MoveToActor(playerTank, AcceptanceRadius);

		//Aim towards the player
		controlledTank->AimAt(playerTank->GetActorLocation());

		//Fire if able / ready
		controlledTank->Fire();
	}
}
