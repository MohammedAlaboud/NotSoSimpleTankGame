// Fill out your copyright notice in the Description page of Project Settings.


#include "TankBarrel.h"

void UTankBarrel::RotateBarrel(float speed) //raise and lower tank barrel based on given param
{
  //move barrel the correct amount per frame given max speed for elevation, and frame time (for frame rate independence)
  auto clampedSpeed = FMath::Clamp<float>(speed, -1, 1);
  auto ChangeInMovement = clampedSpeed * maxSpeed * GetWorld()->DeltaTimeSeconds;

  //determine how much to raise/lower barrel making sure not to exceed max/min 
  auto angleToRaise = RelativeRotation.Pitch + ChangeInMovement;
  auto clampedRaisingValue = FMath::Clamp<float>(angleToRaise, minHeight, maxHeight);

  SetRelativeRotation(FRotator(clampedRaisingValue, 0, 0)); //apply rotation
}