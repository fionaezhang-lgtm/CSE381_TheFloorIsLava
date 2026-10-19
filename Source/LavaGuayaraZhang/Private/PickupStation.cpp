// Fill out your copyright notice in the Description page of Project Settings.


#include "PickupStation.h"
#include "Components/BoxComponent.h"
#include "Components/PrimitiveComponent.h"
#include "InventoryComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"


// Sets default values
APickupStation::APickupStation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    SetRootComponent(SceneRoot);

    StationMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StationMesh"));
    StationMesh->SetupAttachment(SceneRoot);
    //StationMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision );

    CushionMesh = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("CushionMesh")
    );
    CushionMesh->SetupAttachment(SceneRoot);
    //CushionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    ExtinguisherMesh = CreateDefaultSubobject<UStaticMeshComponent>(
        TEXT("ExtinguisherMesh")
    );
    ExtinguisherMesh->SetupAttachment(SceneRoot);
    //ExtinguisherMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
    Trigger->SetupAttachment(SceneRoot);
    Trigger->SetRelativeLocation(FVector::ZeroVector);
    Trigger->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
    Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Trigger->SetCollisionObjectType(ECC_WorldDynamic);
    Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
    Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    Trigger->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void APickupStation::BeginPlay()
{
    Super::BeginPlay();

    if (!Trigger)
    {
        return;
    }

    Trigger->OnComponentBeginOverlap.AddDynamic(
        this,
        &APickupStation::HandleOverlap
    );

  
}


void APickupStation::EndPlay(const EEndPlayReason::Type Reason)
{
    GetWorldTimerManager().ClearTimer(RestockTimer);
    Super::EndPlay(Reason);
}

void APickupStation::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& Sweep)
{
   
    if (!bAvailable) return;
    if (!OtherActor) return;

    UInventoryComponent* Inventory =
        OtherActor->FindComponentByClass<UInventoryComponent>();

    if (!Inventory|| !Inventory->AddItem(ItemType)) return;

    bAvailable = false;

    if (ItemType == EItemSlot::Cushion)
    {
        if (CushionMesh)
        {
            CushionMesh->SetVisibility(false, true);
        }
    }
    else if (ItemType == EItemSlot::Extinguisher)
    {
        if (ExtinguisherMesh)
        {
            ExtinguisherMesh->SetVisibility(false, true);
        }
    }

    if (RestockSeconds <= 0.0f)
    {
        Restock();
        return;
    }

    GetWorldTimerManager().SetTimer(
        RestockTimer,
        this,
        &APickupStation::Restock,
        RestockSeconds,
        false
    );

}

void APickupStation::Restock()
{
    bAvailable = true;
    if (ItemType == EItemSlot::Cushion)
    {
        CushionMesh->SetVisibility(true, true);
    }
    else
    {
        ExtinguisherMesh->SetVisibility(true, true);
    }
}