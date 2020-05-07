// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FixPhysics.generated.h"

UCLASS()
class SIMPLETANKGAME_API AFixPhysics : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFixPhysics();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

//I setup this class 'FixPhysics' with the intention of fixing the issues with the tank movement on the surface but could not in time (kept for future development)