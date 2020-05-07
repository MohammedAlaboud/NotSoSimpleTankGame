// Fill out your copyright notice in the Description page of Project Settings.

#include "TankData.h" //used for signalling 0 health
#include "TankAimComponent.h"
#include "AITankHandler.h" //depends on movement component from pathfinding

void AAITankHandler::BeginPlay() 
{
    Super::BeginPlay();//overriding begin play 
}

void AAITankHandler::SetPawn(APawn* InPawn) //overriding from parent
{
    Super::SetPawn(InPawn);
    if(InPawn) //pointer protection
    {
        auto AITank = Cast<ATankData>(InPawn); //casting to access methods of ATankData + the object instance of the tank that's broadcasting
        if(!ensure(AITank)) //protecting against crash (sometime I use ensure rather than directly checking for debugging purposes, ensure lets me know which protection specifically failed or was going to fail)
        { 
            return;
        }

        //Set up broadcasting to listen when the tank data class signals the 'health depleted' event
        AITank->onHealthDepleted.AddUniqueDynamic(this, &AAITankHandler::currentTankHealthDepleted);
    }
}

void  AAITankHandler::currentTankHealthDepleted()
{
    if(!GetPawn()) //protecting against crash
    {
        return;
    }

    GetPawn()->DetachFromControllerPendingDestroy(); //method to detach the pawn from the controller without issue causing the ai tank to stop moving/firing
}

// Called every frame
void AAITankHandler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    //find player tank
    auto player = GetWorld()->GetFirstPlayerController()->GetPawn(); //find and get player pawn in world
    //get ai possessed tank
    auto possessedTank = GetPawn();

    if(!player || !possessedTank) //pointer protection
    {
        return;
    } 
    
    //move towards player (Nav method)
    MoveToActor(player, navRadius); //radius acceptance in cm?
        
    //aim towards player
    auto aimComponent = possessedTank->FindComponentByClass<UTankAimComponent>();
    aimComponent->AimAt(player->GetActorLocation());

    //fire if ready (maybe also make it fire when positioning barrel for future testing)
    if(aimComponent->DetermineIfShouldFire() == EAimState::Ready /*|| aimComponent->DetermineIfShouldFire() == EAimState::PositioningBarrel*/)
    {
        aimComponent->FireProjectile(); //fire where barrel is pointing
    }
}