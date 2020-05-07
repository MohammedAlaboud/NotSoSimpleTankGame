// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/NavMovementComponent.h"
#include "TankNavMovementComponent.generated.h"

class UTankTracks;

/**
 * 
 */
//to drive tracks maiinly for AI navigation
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) ) 
class SIMPLETANKGAME_API UTankNavMovementComponent : public UNavMovementComponent
{
	GENERATED_BODY()
	
public:	
	UFUNCTION(BlueprintCallable, Category = Setup)
	void Initialise(UTankTracks* setTrackL, UTankTracks* setTrackR); //setting the tracks for the tanks
	
	UFUNCTION(BlueprintCallable, Category = Input)
	void moveForward(float desiredMovement); //to move the tank based on given param value (instead of setting move forward and move backwards methods, negative values are given to move tank backwards)

	UFUNCTION(BlueprintCallable, Category = Input)
	void moveRight(float desiredMovement); //to turn the tank based on given param value (instead of setting turn right and turn left methods, negative values are given to turn tank left)

private:
	//references to tank tracks
	UTankTracks* trackL = nullptr;
	UTankTracks* trackR = nullptr;

	//called from AI Nav Movement Component based code to determine how to move AI
	virtual void RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) override;

};
