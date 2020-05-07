// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/PrimitiveComponent.h"
#include "TankTracks.h"

UTankTracks::UTankTracks()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UTankTracks::BeginPlay()
{
    Super::BeginPlay();
    OnComponentHit.AddDynamic(this, &UTankTracks::OnHit); //collision detection
}

//based on OnHit from bp and used to make sure no movement forces are apply to the tank when its tracks are not in contact with the terrain
void UTankTracks::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	//apply movement to tank tracks
    applyDrivingForce();

    //apply sideways friction force
    MitigateSidewayMovement();

    //resetting driving force to apply to make sure the tank can stop
    currentDrivingForceToApply = 0;
}

void UTankTracks::MitigateSidewayMovement() //reduce slipping 
{
    
    //find out speed of tank as it moves sideways
    auto sideMovementSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());

    //compute value to reduce sideways slipping (with frame rate independence)
    auto sideMovementToReduce = -sideMovementSpeed / (GetWorld()->GetDeltaSeconds()) * GetRightVector();

	auto tankBase = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent()); //casting to get access to methods

    //compute force to act as friction (based on F=ma)  when tank moves sideways (dividing by two as this is being applied one for each track)
    auto friction = (tankBase->GetMass() * sideMovementToReduce) / 2;

    //apply to the tank movements
    tankBase->AddForce(friction);
}

void UTankTracks::setDrivingForce(float additionalDrivingForce) //set movement force value for tank (should be power not force...)
{
    //based on input so it is normalized
    currentDrivingForceToApply = FMath::Clamp<float>(currentDrivingForceToApply + additionalDrivingForce, -1.0, 1.0);
}

void UTankTracks::applyDrivingForce()
{
    auto forceVector = GetForwardVector() * currentDrivingForceToApply * maxForce;//force applied to move tank
    auto positionToApplyForce = GetComponentLocation(); //based on tracks and not the base (yet)
    auto tankBase = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent()); 
    tankBase->AddForceAtLocation(forceVector, positionToApplyForce); //apply specific force at location
}