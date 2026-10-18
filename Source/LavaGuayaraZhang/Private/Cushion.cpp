// Fill out your copyright notice in the Description page of Project Settings.


#include "Cushion.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TimerManager.h"
#include "Lava.h"


ACushion::ACushion()
{
	PrimaryActorTick.bCanEverTick = true;
}

 void ACushion::Tick(float DeltaTime) 
{
     Super::Tick(DeltaTime);

     if (LavaActor)
     {
         AddActorWorldOffset(
             FVector(0.0f, 0.0f, LavaActor->GetRiseRate() * DeltaTime),
             true
         );
     }
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

    SetActorScale3D(FVector(5.0f, 5.0f, 5.0f));

    FRotator FlatRotation = GetActorRotation();
    FlatRotation.Pitch = 0.0f;
    FlatRotation.Roll = 0.0f;
    SetActorRotation(FlatRotation);
    AddActorWorldOffset(FVector(0.0f, 0.0f, -30.0f));

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

void ACushion::SetLavaActor(ALava* InLava)
{
    LavaActor = InLava;
}