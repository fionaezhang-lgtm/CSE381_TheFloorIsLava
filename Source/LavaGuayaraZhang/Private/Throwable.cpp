// Fill out your copyright notice in the Description page of Project Settings.


#include "Throwable.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
AThrowable::AThrowable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Mesh->SetCollisionEnabled(
		ECollisionEnabled::QueryAndPhysics
	);
	Mesh->SetCollisionObjectType(ECC_PhysicsBody);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
	Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->SetSimulatePhysics(false);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("ProjectileMovement")
	);

	Movement->SetUpdatedComponent(Mesh);
	Movement->InitialSpeed = ThrowSpeed;
	Movement->MaxSpeed = ThrowSpeed;
	Movement->ProjectileGravityScale = 1.0f;
	Movement->bAutoActivate = false;
	Movement->bShouldBounce = false;


}

// Called when the game starts or when spawned
void AThrowable::BeginPlay()
{
	Super::BeginPlay();
	if (Mesh)
	{
		Mesh->OnComponentHit.AddDynamic(
			this, &AThrowable::HandleHit
		);
	}
}

//// Called every frame
//void AThrowable::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void AThrowable::HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit) 
{
	//to be implemented
	
}

void AThrowable::Throw(const FVector& Direction)
{
	SetCarried(false);

	if (AActor* OwnerActor = GetOwner())
	{
		Mesh->IgnoreActorWhenMoving(OwnerActor, true);
	}

	if (AActor* InstigatorActor = GetInstigator())
	{
		Mesh->IgnoreActorWhenMoving(InstigatorActor, true);
	}
	
	
	Movement->Velocity =Direction.GetSafeNormal() * ThrowSpeed;
	Movement->Activate(true);

}

void AThrowable::SetCarried(bool bCarried)
{
	if (!Mesh || !Movement) return;
	

	if (bCarried)
	{
		Movement->StopMovementImmediately();
		Movement->Deactivate();

		Mesh->SetSimulatePhysics(false);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		Mesh->SetSimulatePhysics(false);
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Movement->Activate(true);
	}
}

