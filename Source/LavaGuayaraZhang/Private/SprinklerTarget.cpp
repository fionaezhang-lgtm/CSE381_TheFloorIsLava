#include "SprinklerTarget.h"
#include "Components/StaticMeshComponent.h"
#include "LavaKey.h"

// Sets default values
ASprinklerTarget::ASprinklerTarget()
{
    PrimaryActorTick.bCanEverTick = false;
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));  // projectile hit events need a blocking surface
    SetRootComponent(Mesh);
}

void ASprinklerTarget::OnHitBy(AActor* Hitter)
{
    if (bHit) return;
    bHit = true;

    if (HitMaterial) { Mesh->SetMaterial(0, HitMaterial); }
    if (IsValid(LinkedKey)) { LinkedKey->Unlock(); }
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

