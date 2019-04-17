// Copyright Oliver Scott 2019

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankAIController.generated.h"

//Forward Declaration
class ATank;

UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

private:
	//The min distance the AI can get to the player (in cm)
	float AcceptanceRadius = 3000;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};

