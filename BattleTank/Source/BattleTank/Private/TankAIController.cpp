// Copyright Oliver Scott 2019

#include "TankAIController.h"
#include "TankAimComponent.h"
#include "Tank.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ATankAIController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }

		// Subscribe our method to tank's death event.
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
	}
}

void ATankAIController::OnPossessedTankDeath()
{
	if (!GetPawn()) { return; }
	GetPawn()->DetachFromControllerPendingDestroy();
}

void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//Find the pawn attached to the player controller.
	auto playerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto controlledTank = GetPawn();

	if (!(playerTank && controlledTank)) { return; }

	//TMove towards the player
	MoveToActor(playerTank, AcceptanceRadius);

	//Aim towards the player
	auto AimingComp = controlledTank->FindComponentByClass<UTankAimComponent>();
	AimingComp->AimAt(playerTank->GetActorLocation());

	//Fire if able / ready
	if(AimingComp->GetFiringState() == EFiringState::Locked)
		AimingComp->Fire();
}
