// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/ProjectileMovementComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TankProjectile.generated.h"

UCLASS()
class SIMPLETANKGAME_API ATankProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATankProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Launch the projectile based on given speed
	void Shoot(float speed);

private:	
	//Movment component in projectile BP
	UProjectileMovementComponent* projectileMovementComp = nullptr;

	//The mesh set for the projectile
	UPROPERTY(VisibleAnywhere, Category = "Properties From Code")
	UStaticMeshComponent* projectileMesh = nullptr;

	//How long the projectile will be in the scene after is it spawned
	UPROPERTY(EditDefaultsOnly, Category = "Properties From Code")
	float timeToDestroy = 11.0f;

	//How much damage to apply to the tank (set tank health/lives to three -> three hits to defeat tank)
	UPROPERTY(EditDefaultsOnly, Category = "Properties From Code")
	float damageAmount = 1.0f; //ApplyRadialDamage requires a float

	//Component to setup area damage
	UPROPERTY(VisibleAnywhere, Category = "Properties From Code")
	URadialForceComponent* projectilePushBack = nullptr;

	UFUNCTION() //collision detection method based on Actor onHit method
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	void SetUpTimer(); //meant to be called to track time till projectile needs to be destroyed if it hasn't already but right now it just destroys the projectile through the use of GetTimerManager function

	//had plans to setup particle system for launching and impact; this is kept here for future development
	UParticleSystemComponent* ParticleSyst = nullptr; 

};
