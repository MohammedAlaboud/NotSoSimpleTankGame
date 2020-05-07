// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

/**
 * 
 */
UCLASS(meta=(BlueprintSpawnableComponent)) //, hidecategories = ("Collision"))
class SIMPLETANKGAME_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	void RotateBarrel(float speed); //speed value can be between -1 and +1 for lowering and raising

private:
	UPROPERTY(EditDefaultsOnly, Category = "Properties From Code") //EditDefaultsOnly means all of the instances share this value
	float maxSpeed = 10; //speed at which barrel moves (set by testing)

	UPROPERTY(EditDefaultsOnly, Category = "Properties From Code")
	float maxHeight = 40; //maximum point at which barrel can be raised

	UPROPERTY(EditDefaultsOnly, Category = "Properties From Code")
	float minHeight = 0; //minimum point at which barrel can be lowered
	
};
