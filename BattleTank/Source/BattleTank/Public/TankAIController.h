// Copyright Oliver Scott 2019

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Tank.h"
#include "TankAIController.generated.h"

/**
 *
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()

private:
	ATank* GetControlledTank() const;
	ATank* GetPlayerTank() const;
	

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};

