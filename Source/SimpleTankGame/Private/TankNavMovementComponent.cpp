// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTracks.h"
#include "TankNavMovementComponent.h"

void UTankNavMovementComponent::Initialise(UTankTracks* setTrackL, UTankTracks* setTrackR)
{
    //setting the tracks (if issue occurs related to tracks being set, check tank bp)
	trackL = setTrackL;
    trackR = setTrackR;
}

void UTankNavMovementComponent::RequestDirectMove(const FVector& MoveVelocity, bool bForceMaxSpeed) //function based on AI navigation from UE
{ 
    //overriding this method from ai nav movement componenet but replacing functionality rather than extending from it (no super)
    auto tankForwardDirection = GetOwner()->GetActorForwardVector().GetSafeNormal(); //get direction tank is facing as unit vector
    auto AIForwardDirection = MoveVelocity.GetSafeNormal(); //vector for the direction the AI tank should move. safe normal will normalize it to values between -1 and 1

    //forward value will also take into account backward value as given parameters are normalized between -1 and +1
    auto requiredForwardMovement = FVector::DotProduct(tankForwardDirection, AIForwardDirection);
    moveForward(requiredForwardMovement); 

    //right turn value will also take into account left turn value as given parameters are normalized between -1 and +1
    auto requiredTurnMovement = FVector::CrossProduct(tankForwardDirection, AIForwardDirection).Z;
    moveRight(requiredTurnMovement);
}


void UTankNavMovementComponent::moveForward(float desiredMovement) //setup for ai tanks to move forwards and backwards
{
    if(!trackL || !trackR) //protecting against crash
	{
		return;
	}

    //call functions to apply force to move tank from tracks class
    trackL->setDrivingForce(desiredMovement);
    trackR->setDrivingForce(desiredMovement);
}

void UTankNavMovementComponent::moveRight(float desiredMovement) //setup for ai tanks to turn right and left
{
    if(!trackL || !trackR)
	{
		return;
	}

    //call functions to apply force to move tank from tracks class
    trackL->setDrivingForce(desiredMovement);
    trackR->setDrivingForce(-desiredMovement);
}