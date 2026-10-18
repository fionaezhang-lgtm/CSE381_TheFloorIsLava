// Fill out your copyright notice in the Description page of Project Settings.


#include "Extinguisher.h"
#include "Lava.h"
#include "Components/StaticMeshComponent.h"

AExtinguisher::AExtinguisher()
{
    
    PrimaryActorTick.bCanEverTick = false;
}

void AExtinguisher::BeginPlay()
{
    Super::BeginPlay();

    if (Mesh)
    {
        Mesh->SetGenerateOverlapEvents(true);
        Mesh->OnComponentBeginOverlap.AddDynamic( this,&AExtinguisher::HandleLavaOverlap );
    }
}

void AExtinguisher::HandleLavaOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComponent,int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(ALava::StaticClass()))
    {
        Destroy();
    }
}