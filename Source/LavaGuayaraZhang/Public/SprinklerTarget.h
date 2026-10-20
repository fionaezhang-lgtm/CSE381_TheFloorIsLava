// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SprinklerTarget.generated.h"

class UStaticMeshComponent;
class UMaterialInterface;
class ALavaKey;

UCLASS()
class LAVAGUAYARAZHANG_API ASprinklerTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASprinklerTarget();

	void OnHitBy(AActor *Hitter);

	UFUNCTION(BlueprintPure, Category = "Target")
	bool WasHit() const {return bHit;}

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(EditInstanceOnly, Category = "Target")
	TObjectPtr<UMaterialInterface> HitMaterial;

	bool bHit = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
