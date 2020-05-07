// Fill out your copyright notice in the Description page of Project Settings.

#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TankProjectile.h"

// Sets default values
ATankProjectile::ATankProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//setting up the mesh as the root component and enabling required properties to detect and see the mesh
	projectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("MeshForCollisionDetection"));
	SetRootComponent(projectileMesh);
	projectileMesh->SetNotifyRigidBodyCollision(true);
	projectileMesh->SetVisibility(true);

	//setup the movement component
	projectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(FName("ProjectileMovementComponent"));
	projectileMovementComp->bAutoActivate = false; //to prevent projectile from being shot on construction
	
	//setup the area damage caused by projectile impact/collision
	projectilePushBack = CreateDefaultSubobject<URadialForceComponent>(FName("ProjectilePush"));
	projectilePushBack->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform); //to make sure that this occurs at the same position as the static mesh
}

// Called when the game starts or when spawned
void ATankProjectile::BeginPlay()
{
	Super::BeginPlay();
	projectileMesh->OnComponentHit.AddDynamic(this, &ATankProjectile::OnHit); 
}

// Called every frame
void ATankProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATankProjectile::Shoot(float speed)
{
	if(!projectileMovementComp) //pointer protection
	{
		return;
	}
	projectileMovementComp->SetVelocityInLocalSpace(FVector::ForwardVector * speed); //launch where it was oriented to travel to at a specified speed
	projectileMovementComp->Activate(); //deactivated at construction to avoid moving on construction, but need to activate to move it on shooting
}

void ATankProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	projectilePushBack->FireImpulse(); //radial force method call to apply area force

	//getting a wierd error when destoying root component, so I swap the pushback as the root component then destroy the mesh itself
	SetRootComponent(projectilePushBack); 
	projectileMesh->DestroyComponent();

	//apply damage in the area
	UGameplayStatics::ApplyRadialDamage(this, damageAmount, GetActorLocation(), projectilePushBack->Radius, UDamageType::StaticClass(), TArray<AActor*>());

	//setup a timer to destory spawned projectile components in the scene after a certain time 
	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ATankProjectile::SetUpTimer, timeToDestroy, false);
}

void ATankProjectile::SetUpTimer()
{
	Destroy(); //destroy the projectile when timer set up above expires
}