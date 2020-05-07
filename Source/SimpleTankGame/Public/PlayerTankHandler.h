// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerTankHandler.generated.h" //must be last include

class UTankAimComponent;

/**
 * 
 */
UCLASS() //helps player aim and moves barrel towards correct position depending on where player aims
class SIMPLETANKGAME_API APlayerTankHandler : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void currentTankHealthDepleted(); //called when tank's health reaches 0
	

protected:
	//function to set aiming component in bp (component found on play)
	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void HasAimingComponent(UTankAimComponent* aimComponentRef);


private:	

	virtual void BeginPlay() override; 
	virtual void Tick(float DeltaTime) override; 

	void SetPawn(APawn* InPawn); //contributes to setup for listening to broadcast event from tank

	void EstablishAiming(); //figures out where the tank is aiming based on given reticle point and player perspective
	bool GetAimHitPosition(FVector& aimingHitPosition) const; //get the position being aimed at
	bool GetAimDirection(FVector2D screenPointPosition, FVector& aimDirection) const; //get the direction to aim to based on player perspective
	bool GetAimHitVectorPosition(FVector aimDirection, FVector& aimHitPosition) const; //work out the transform vector that will be needed to launch the projectile form and correctly hit the position being aimed at 

	//manually setting the reticle location based on where it was placed in the player interface bp
	UPROPERTY(EditDefaultsOnly) 
	float aimPointXPos = 0.5;

	UPROPERTY(EditDefaultsOnly) 
	float aimPointYPos = 0.3333;

	UPROPERTY(EditDefaultsOnly) 
	float aimRange = 1000000; //range at which line tracing for aiming will work

};
