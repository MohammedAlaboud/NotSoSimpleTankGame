// Fill out your copyright notice in the Description page of Project Settings.


#include "TankTurret.h"

void UTankTurret::RotateTurret(float speed) //rotate turret based on given speed
{
  //move turrent the correct amount per frame given max speed for movement and frame time (for frame rate independence)
  auto clampedSpeed = FMath::Clamp<float>(speed, -1, 1);
  auto ChangeInRotation = clampedSpeed * maxSpeed * GetWorld()->DeltaTimeSeconds;

  //determine angle to rotate to
  auto angleToMoveTo = RelativeRotation.Yaw + ChangeInRotation; 

  //apply the rotation
  SetRelativeRotation(FRotator(0, angleToMoveTo, 0));
}