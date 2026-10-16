// Fill out your copyright notice in the Description page of Project Settings.


#include "SprinklerTarget.h"

// Sets default values
ASprinklerTarget::ASprinklerTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASprinklerTarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASprinklerTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

