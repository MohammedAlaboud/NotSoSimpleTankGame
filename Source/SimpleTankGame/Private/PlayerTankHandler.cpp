// Fill out your copyright notice in the Description page of Project Settings.

#include "TankData.h" //used for signalling 0 health
#include "TankAimComponent.h"
#include "PlayerTankHandler.h"

void APlayerTankHandler::BeginPlay() 
{
    Super::BeginPlay();//overriding begin play 
    auto tAimingComponent = GetPawn()->FindComponentByClass<UTankAimComponent>(); //find aiming component
    if(!tAimingComponent) //pointer protection
    {
        return;
    } 
    
    HasAimingComponent(tAimingComponent); //set aiming component
}

void APlayerTankHandler::SetPawn(APawn* InPawn) 
{
    Super::SetPawn(InPawn); //overriding from parent
    if(InPawn) //pointer protection
    {
        auto playerTank = Cast<ATankData>(InPawn); //casting to access methods of ATankData + the object instance of the tank that's broadcasting
        if(!ensure(playerTank))
        { //more protection 
            return;
        }

        //Set up broadcasting to listen when the tank data class signals the 'health depleted' event
        playerTank->onHealthDepleted.AddUniqueDynamic(this, &APlayerTankHandler::currentTankHealthDepleted);
    }
}

void APlayerTankHandler::currentTankHealthDepleted()
{
    StartSpectatingOnly(); //when player's health is depleted, player will go into spectator mode
}

void APlayerTankHandler::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    EstablishAiming();
}

void APlayerTankHandler::EstablishAiming() //move tank turret/barrel towards where aim point intersects the world
{
    if(!GetPawn()) //protecting against crash
    {
        return;
    }

    auto tAimingComponent = GetPawn()->FindComponentByClass<UTankAimComponent>(); //get aiming component (again as it is modified)
    if(!tAimingComponent) //protecting against crash
    {
        return;
    } 

    FVector aimingHitPosition; //hit location from aim linetracing to act as an out parameter

    bool gotHitPosition = GetAimHitPosition(aimingHitPosition); //check if a hit position was found

    if(gotHitPosition) //if aiming at something based on reticle position given (line trace through point)
    {
        //tell posessed tank to aim accordingly
        tAimingComponent->AimAt(aimingHitPosition);

    }
}

bool APlayerTankHandler::GetAimHitPosition(FVector& aimingHitPosition) const  //get world location of what's being aimed by line tracing through aim reticle/point; returns an out param (reference to the position aiming at), true if it hits something
{
    //Find/Set aiming point position (pixel values)
    int32 gameScreenSizeX, gameScreenSizeY; //to store the dimensions of the game viewport
    GetViewportSize(gameScreenSizeX, gameScreenSizeY); //getting the dimensions (rarely causes issues but will check for potential future development)
    auto screenPosition = FVector2D(aimPointXPos * gameScreenSizeX, aimPointYPos * gameScreenSizeY); //determine the aim point on the screen (where to trace from) based on the game viewport size

    //convert screen position of aiming point to a world position being aimed at
    FVector aimDirection;
    if(GetAimDirection(screenPosition, aimDirection)) //if we can get an aiming direcition vector to trace through
    {
        //perform linetracing along aiming direction and determine what is being aimed at (with a set max range)
        return GetAimHitVectorPosition(aimDirection, aimingHitPosition); //pass in position of whats being aimed at as out param

    }
            
    return false; //in case nothing is found
}

bool APlayerTankHandler::GetAimDirection(FVector2D screenPointPosition, FVector& aimDirection) const
{
    FVector playerCameraWorldPosition; //unusual requirement for method call below
    return DeprojectScreenPositionToWorld(screenPointPosition.X, screenPointPosition.Y, playerCameraWorldPosition, aimDirection); //covert 2D position of screen space to 3D world position
}

bool APlayerTankHandler::GetAimHitVectorPosition(FVector aimDirection, FVector& aimHitPosition) const
{
    FHitResult aimHitResult; //data of one hit of a trace
    auto startPosition = PlayerCameraManager->GetCameraLocation(); //where to trace from
    auto endPosition = startPosition + (aimDirection * aimRange); //what to trace to

    //to ignore the player tank from tracing 
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetPawn()); 

    if(GetWorld()->LineTraceSingleByChannel(aimHitResult, startPosition, endPosition, ECollisionChannel::ECC_Visibility, CollisionParams)) //if aiming line tracing works
    {
        //set hit position based on result and return true
        aimHitPosition = aimHitResult.Location;
        return true;
    } 
    //otherwise if it doesn't hit anything set hit position to zero and return false
    aimHitPosition = FVector(0.0); //unsure if this is a suitable default to move to
    return false;
}