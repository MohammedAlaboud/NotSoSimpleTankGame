// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h" //needs access to tank barrel method
#include "TankTurret.h"
#include "TankProjectile.h"
#include "TankAimComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTankAimComponent::UTankAimComponent() //Constructor
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true; 
}

// Called when the game starts
void UTankAimComponent::BeginPlay()
{
	Super::BeginPlay();

	timeSinceLastFire = FPlatformTime::Seconds(); //reset fire time at start so that all tanks start reloading rather than start loaded
}

// Called every frame
void UTankAimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//setting up reload time  
	bool canFire = (FPlatformTime::Seconds() - timeSinceLastFire) < fireRate;

	//setting aiming states
	if(remainingAmmo <= 0) //if tank has no ammo(sometimes value can be negative during testing so better to check for negative values)
	{
		aimState = EAimState::OutOfAmmo;
	}
	else if(canFire) //if tank reloaded 
	{
		aimState = EAimState::Reloading;
	}
	else if(CheckIfBarrelBeingPositioned()) //if barrel is still moving to aiming position
	{
		aimState = EAimState::PositioningBarrel;
	}
	else //if tank is ready to fire (and will hit whats being aimed at)
	{
		aimState = EAimState::Ready;
	}
	
}

void UTankAimComponent::Initialise(UTankBarrel* setBarrel, UTankTurret* setTurret) 
{
	//setting thr turret and barrel
	tankBarrel = setBarrel;
	tankTurret = setTurret;
}

bool UTankAimComponent::CheckIfBarrelBeingPositioned()
{
	//if no barrel is found (pointer protection)
	if(!tankBarrel)
	{
		return false; //then it shouldn't be moving
	}

	auto barrelForwardVector = tankBarrel->GetForwardVector(); //get direciton barrel is facing

	return !barrelForwardVector.Equals(desiredAimDirection,  0.01); //if vectors are equal (or within 10 cm min I think based on .01)
}

void UTankAimComponent::AimAt(FVector atPosition)
{
	if(!tankBarrel) //pointer protection to avoid crash 
	{
		return;
	}

	if(!ensure(tankBarrel)) //ensure for debugging purposes to determine where nullptrs are being passed
	{
		return;
	}

	FVector projectileLaunchVelocity; //speed at which to launch
	FVector startPosition = tankBarrel->GetSocketLocation(FName("ProjectileOriginSocket")); //shoot from the setup socket for the barrel

	//UE's method for determiig the velocity to launch at based on aiming and target location to hit the target
	bool foundAimingSolution = UGameplayStatics::SuggestProjectileVelocity(this, projectileLaunchVelocity, startPosition, atPosition, projectileSpeed, false, 0, 0, ESuggestProjVelocityTraceOption::DoNotTrace);

	//if we calculate the luanch velocity (as out param) without issue
	if(foundAimingSolution) 
	{
		desiredAimDirection = projectileLaunchVelocity.GetSafeNormal(); //turn it into unit vector to know direction to launch 
		MoveTankBarrelTo(desiredAimDirection); //position tank barrel to aim corrrectly

	} 
	//else do nothing

}

void UTankAimComponent::MoveTankBarrelTo(FVector aimVector) //will move the tank barrel to aim towards desired position
{
	if(!tankBarrel || !tankTurret) //protecting against crash
	{
		return;
	}

	if(!ensure(tankBarrel) || !ensure(tankTurret)) //to determine what would've caused a crash (kept for future reference)
	{
		return;
	}

	//compute differences between barrel rotation and the aim direction rotation
	auto tankBarrelRotation = tankBarrel->GetForwardVector().Rotation(); //get rotation from direction barrel is facing
	auto aimRotation = aimVector.Rotation(); //aim rotation values
	auto rotationDifference =  aimRotation - tankBarrelRotation; //difference in rotation

	//only rotate the barrel up and down, the turret controls left and right movement
	tankBarrel->RotateBarrel(rotationDifference.Pitch); 

	//ensure that the barrel rotates the faster/closer direction
	if((FMath::Abs(rotationDifference.Yaw)) < 180)
	{
		tankTurret->RotateTurret(rotationDifference.Yaw);
	}
	else 
	{
		tankTurret->RotateTurret(-rotationDifference.Yaw);
	}
	
}

void UTankAimComponent::FireProjectile()
{
	if(!tankBarrel || !projectileBP) //protecting against crash
	{
		return;
	}

	if(aimState == EAimState::PositioningBarrel || aimState == EAimState::Ready) //pointer protection and checking that enough time passed before it is should be possible to fire again
	{
		if(!ensure(tankBarrel)) //ensures used toi determien where it would've crashed and which line is causing an issue in the game
		{
			return;
		}
		else if (!ensure(projectileBP))
		{
			return;
		}

		//spawn projectile making sure it faces the same direction that the barrel is aiming
		auto projectile = GetWorld()->SpawnActor<ATankProjectile>(projectileBP, tankBarrel->GetSocketLocation(FName("ProjectileOriginSocket")), tankBarrel->GetSocketRotation(FName("ProjectileOriginSocket")));

		if(!projectile) //more protecting
		{
			return;
		}

		projectile->Shoot(projectileSpeed); //shoot projectile
		timeSinceLastFire = FPlatformTime::Seconds(); //reset fire time
		remainingAmmo--; //decrement ammo
	}
}

//to determine if the tank should be firing by checking the aimState (getter method for AI class)
EAimState UTankAimComponent::DetermineIfShouldFire() const
{
	return aimState; //getter for aimState
}

int32 UTankAimComponent::CountRemainingAmmo() 
{
	return remainingAmmo; //getter for ramining ammo
}
