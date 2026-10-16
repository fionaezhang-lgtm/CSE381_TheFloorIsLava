// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupStation.h"

// Sets default values
APickupStation::APickupStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APickupStation::BeginPlay()
{
	//Super::BeginPlay();
	
}
//
//// Called every frame
//void APickupStation::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void APickupStation::EndPlay(const EEndPlayReason::Type Reason)
{

}

void APickupStation::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& Sweep)
{


}

void APickupStation::Restock()
{

}