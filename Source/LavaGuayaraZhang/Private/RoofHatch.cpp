// Fill out your copyright notice in the Description page of Project Settings.


#include "RoofHatch.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "LavaCharacter.h"
#include "LavaGameMode.h"
#include "TimerManager.h"


// Sets default values
ARoofHatch::ARoofHatch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Hatch = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hatch"));
	SetRootComponent(Hatch);

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerVolume"));
	TriggerVolume->SetupAttachment(Hatch);
	TriggerVolume->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

}

// Called when the game starts or when spawned
void ARoofHatch::BeginPlay()
{
	Super::BeginPlay();

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this,&ARoofHatch::HandleOverlap);
	
}

void ARoofHatch::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& Sweep)
{
    ALavaCharacter* Character = Cast<ALavaCharacter>(OtherActor);

    if (!Character)
    {
        return;
    }

    ALavaGameMode* GameMode =
        GetWorld()->GetAuthGameMode<ALavaGameMode>();

    if (GameMode)
    {
        GameMode->ReportHatchReached();
    }
}

// Called every frame
void ARoofHatch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

