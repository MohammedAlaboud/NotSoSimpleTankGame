// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TankAimComponent.generated.h"

//Enum for aiming states to help change color of the aiming point/reticle and determine when to fire
UENUM()
enum class EAimState: uint8 {Reloading, PositioningBarrel, Ready, OutOfAmmo};

class UTankBarrel;
class UTankTurret;
class ATankProjectile;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLETANKGAME_API UTankAimComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFUNCTION(BlueprintCallable, Category = "Properties From Code")
	void Initialise(UTankBarrel* setBarrel, UTankTurret* setTurret); //to set the turrent and barrel

	void AimAt(FVector atPosition); //to determine where to aim 

	UFUNCTION(BlueprintCallable, Category = "Shooting") 
	void FireProjectile(); //fires the projectile

	EAimState DetermineIfShouldFire() const; //to determine when the AI tanks should shoot

	UFUNCTION(BlueprintCallable, Category = "Shooting")
	int32 CountRemainingAmmo(); //to determine remaining ammo for the tanks

protected:
	UPROPERTY(BlueprintReadOnly, Category = "State") 
	EAimState aimState = EAimState::Reloading; //tanks start reloading so as not to shoot at start

private:
	// Sets default values for this component's properties
	UTankAimComponent(); 

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void MoveTankBarrelTo(FVector desiredAimDirection); //for moving the tank's barrel based on where the player is aiming

	bool CheckIfBarrelBeingPositioned(); //to determine if barrel is still being positions or not

	//for setting barrel and turret
	UTankBarrel* tankBarrel = nullptr;
	UTankTurret* tankTurret = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Shooting") 
	float projectileSpeed = 5500.0; //speed at which to shoot projectile

	UPROPERTY(EditDefaultsOnly, Category = "Properties From Code") //EditDefaultsOnly (shared between all instances) may need to be used to ensure all tanks have the same fire rate
	TSubclassOf<ATankProjectile> projectileBP; //the projectile bp required by the tank's aiming componet to know what to fire

	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
	float fireRate = 3; //time it takes (in seconds) before it is possible to shoot again

	double timeSinceLastFire = 0; //keep track of time it took since projectile was last fired; double type because FPlatformTime::Seconds() requires double

	FVector desiredAimDirection; //direction at which to aim 

	UPROPERTY(EditDefaultsOnly, Category = "Shooting")
	int32 remainingAmmo = 10; //how much ammo the player starts with
};
