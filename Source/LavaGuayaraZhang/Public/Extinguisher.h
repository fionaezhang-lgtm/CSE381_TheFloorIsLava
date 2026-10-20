// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Throwable.h"
#include "Extinguisher.generated.h"

/**
 * 
 */
UCLASS()
class LAVAGUAYARAZHANG_API AExtinguisher : public AThrowable
{
	GENERATED_BODY()
	

public:
    AExtinguisher();

    virtual void HandleHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
                       UPrimitiveComponent* OtherComp, FVector NormalImpulse,
                       const FHitResult& Hit) override;

protected:
    virtual void BeginPlay() override;

    UFUNCTION() void HandleLavaOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComponent,int32 OtherBodyIndex, bool bFromSweep,const FHitResult& SweepResult );

};
