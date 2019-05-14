// Copyright Oliver Scott 2019

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	//Returns Current Health as a percentage of starting health (between 0 and 1).
	UFUNCTION(BlueprintPure,Category = "Health")
	float GetHealthPercent() const;

	//Called by engine when actor is dealt damage.
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	FTankDelegate OnDeath;
private:	
	// Sets default values for this pawn's properties
	ATank();

	virtual void BeginPlay() override;

	UPROPERTY(EditDEfaultsOnly, Category = "Setup")
	int32 StartingHealth = 100.0f;

	UPROPERTY(VisibleAnywhere,Category = "Health")
	int32 CurrentHealth;
};
