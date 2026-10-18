// Fill out your copyright notice in the Description page of Project Settings.


#include "Cushion.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"


ACushion::ACushion()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ACushion::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACushion::HandleHit(UPrimitiveComponent* HitComp,AActor* OtherActor,
    UPrimitiveComponent* OtherComp,FVector NormalImpulse,const FHitResult& Hit)
{

    if (!OtherActor || OtherActor == this)
    {
        return;
    }

    UE_LOG(LogTemp, Warning,
        TEXT("Cushion hit: %s"),
        *GetNameSafe(OtherActor));

    // Ignore the character who threw the cushion.
    if (OtherActor == GetOwner() || OtherActor == GetInstigator())
    {
        return;
    }

    FloatOnLava();
}

void ACushion::FloatOnLava()
{
    if (Movement)
    {
        Movement->StopMovementImmediately();
        Movement->Deactivate();
    }

    if (Mesh)
    {
        Mesh->SetSimulatePhysics(false);
        Mesh->SetPhysicsLinearVelocity(FVector::ZeroVector);
        Mesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

        Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        Mesh->SetCollisionResponseToAllChannels(ECR_Block);
    }

    SetLifeSpan(DestroyDelay);
}