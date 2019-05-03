// Copyright Oliver Scott 2019

#include "Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//Add collision mesh component to the Blueprint.
	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	SetRootComponent(CollisionMesh);
	CollisionMesh->SetNotifyRigidBodyCollision(true);
	CollisionMesh->SetVisibility(false);
	//Add the LaunchBlast particle effect to the Blueprint.
	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	LaunchBlast->SetupAttachment(CollisionMesh);
	//Add the Impact Blast particle effect to the Blueprint.
	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	ImpactBlast->SetupAttachment(CollisionMesh);
	ImpactBlast->bAutoActivate = false;
	// Add the movement component to the Blueprint.
	ProjectileMoveComponent = CreateDefaultSubobject<UProjectileMovementComponent>(FName("MoveComponent"));
	ProjectileMoveComponent->bAutoActivate = false;
	//Add the radial force component to the Blueprint.
	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force"));
	ExplosionForce->SetupAttachment(CollisionMesh);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}

void AProjectile::LaunchProjectile(float LaunchSpeed)
{
	ProjectileMoveComponent->SetVelocityInLocalSpace(FVector::ForwardVector*LaunchSpeed);
	ProjectileMoveComponent->Activate();
}

void AProjectile::OnHit(UPrimitiveComponent*HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	LaunchBlast->Deactivate();
	ImpactBlast->Activate();
	ExplosionForce->FireImpulse();
}
