// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTurret.generated.h"

/**
 * 
 */
UCLASS(meta=(BlueprintSpawnableComponent))
class SIMPLETANKGAME_API UTankTurret : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	void RotateTurret(float speed); //to rotate turrent (-1 to +1 for max range in movement)

private:
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float maxSpeed = 25; //speed at which to rotate turret (balance between fun and realistic)

};

//UE is designed for making humanoid controllable character based meshes (capsule collision), so I was getting several warnings and bugs that was not possible to fix on my end