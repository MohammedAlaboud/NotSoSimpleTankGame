// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AITankHandler.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLETANKGAME_API AAITankHandler : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Properties From Code") 
 	float navRadius = 7000.0; //distance at which the (AI) enemy tanks will stop moving towards player and continue to fire 


private:
	virtual void BeginPlay() override; 

	virtual void Tick(float DeltaTime) override; 

	virtual void SetPawn(APawn* InPawn) override; //based on pawn class and called when actor is posessed -> set up for listening when event is broadcast from tank

	UFUNCTION()
	void currentTankHealthDepleted(); //called when ai tank health reaches 0
};
