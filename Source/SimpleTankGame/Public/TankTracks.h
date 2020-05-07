// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTracks.generated.h"

/**
 * 
 */
UCLASS(meta=(BlueprintSpawnableComponent)) //can be used in bps
class SIMPLETANKGAME_API UTankTracks : public UStaticMeshComponent
{
	GENERATED_BODY()

public: 
	UFUNCTION(BlueprintCallable, Category = Input) 
	void setDrivingForce(float additionalDrivingForce); //set movement force value for tank - normalized between -1 and 1 (0 and 1 for + and -) 
	
	UPROPERTY(EditDefaultsOnly) //max force per track 
	float maxForce = 40000000.0; //based on F=ma where the mass is set to 40000 and acceleration assumed to be 10ms^-2 (value is huge due to big mass and how UE measures force)

private:
	UTankTracks();

	virtual void BeginPlay() override;

	void MitigateSidewayMovement(); //ensure tank does not slip sideways when moving

	UFUNCTION() //ue magic
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit); //collision detection

	void applyDrivingForce(); //apply force to tracks to move the tank

	float currentDrivingForceToApply = 0; //amount of force to apply 
};
