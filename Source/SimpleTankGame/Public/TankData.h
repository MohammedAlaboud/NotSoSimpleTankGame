// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankData.generated.h" //must be last include

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDataDelegate); //allow this class to broadcast to the Player and AI classes

UCLASS()
class SIMPLETANKGAME_API ATankData : public APawn
{
	GENERATED_BODY()

	// Sets default values for this pawn's properties
	ATankData();

public:
	//method based from AActor and will be called whenever apply damage is called from other classes, this is used to apply damage to the tank
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;	

	FTankDataDelegate onHealthDepleted; //the event that will notify the AI and Player classes that the health of the tank reached 0 (those classes have been implemented to listen for this signal)

private:
	virtual void BeginPlay() override;

	//Health properties of the tank
	UPROPERTY(EditDefaultsOnly, Category = "Properties From Code")
	int32 tankHealth = 3; //I wanted to setup a system based on lives rather than health, but instead the found the TakeDamage method in the UE docs which is based off of a health oriented system (rather than lives)
	//tank can take three hits from the projectile (or projectile's area damage)

	UPROPERTY(VisibleAnywhere, Category = "Properties From Code")
	int32 tankRemainingHealth; //set when game is played as setting it here causes issues when trying to modify it in the cpp file

};