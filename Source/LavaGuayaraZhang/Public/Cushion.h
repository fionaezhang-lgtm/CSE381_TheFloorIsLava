// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Throwable.h"
#include "Cushion.generated.h"

/**
 * 
 */
UCLASS()
class LAVAGUAYARAZHANG_API ACushion : public AThrowable
{
	GENERATED_BODY()

public:
	ACushion();
	void FloatOnLava();

protected:
	virtual void BeginPlay() override;

	virtual void HandleHit(UPrimitiveComponent* HitComp,AActor* OtherActor,
		UPrimitiveComponent* OtherComp,FVector NormalImpulse,const FHitResult& Hit) override;

	UPROPERTY(EditDefaultsOnly , BlueprintReadOnly,Category = "Cushion")
	float DestroyDelay = 10.0f;

private:
	
	
};
