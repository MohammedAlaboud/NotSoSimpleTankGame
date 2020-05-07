// Fill out your copyright notice in the Description page of Project Settings.

#include "TankData.h"

// Sets default values
ATankData::ATankData()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; //set to false as it doesn't need to tick. This is just a data class
}

void  ATankData::BeginPlay()
{
	Super::BeginPlay();
	tankRemainingHealth = tankHealth; //all tanks start with a set value for health
}

float ATankData::TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser)
{
	int32 damageAmountInt = FPlatformMath::RoundToInt(DamageAmount); //amount of damage to apply to the tank  (had to convert to int from float due to comparisson errors)
	auto currentTankHit = FMath::Clamp(damageAmountInt, 0, tankRemainingHealth); //make sure to not damage tank more than required (for testing and error protection purposes)

	tankRemainingHealth -= currentTankHit; //decrement health when tank takes damage

	if(tankRemainingHealth <= 0) //when tank's healh reaches 0
	{
		onHealthDepleted.Broadcast(); //signal  whichever classes that are listening about this event
	}

	return currentTankHit; //get the damage value applied when the current tank was hit
}	
