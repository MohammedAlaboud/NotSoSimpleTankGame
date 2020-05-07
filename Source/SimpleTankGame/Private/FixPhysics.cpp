// Fill out your copyright notice in the Description page of Project Settings.

#include "FixPhysics.h"

// Sets default values
AFixPhysics::AFixPhysics()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFixPhysics::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFixPhysics::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//I setup this class 'FixPhysics' with the intention of fixing the issues with the tank movement on the surface but could not in time (kept for future development)