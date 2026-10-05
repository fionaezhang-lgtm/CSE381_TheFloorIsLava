#include "Lava.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "LavaCharacter.h"

#include "Components/CapsuleComponent.h"

// Sets default values
ALava::ALava()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Surface = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LavaSurface"));
	Surface->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Surface->SetMobility(EComponentMobility::Movable);
	SetRootComponent(Surface);

	Volume = CreateDefaultSubobject<UBoxComponent>(TEXT("LavaVolume"));
	Volume->SetupAttachment(Surface);
	Volume->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	


}

// Called when the game starts or when spawned
void ALava::BeginPlay()
{
	Super::BeginPlay();

	StartZ = GetActorLocation().Z;
	Volume->OnComponentBeginOverlap.AddDynamic(this, &ALava::HandleOverlap);
	
}

// Called every frame
void ALava::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldOffset(FVector(0.f,0.f,RiseRate * DeltaTime));

}

float ALava::GetRiseHeight() const
{
	return GetActorLocation().Z - StartZ;
}

void ALava::SetRiseRate(float NewRiseRate)
{
	RiseRate = NewRiseRate;
}

void ALava::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& Sweep)
{
	if (!Cast<ALavaCharacter>(OtherActor)) return;
	//if (OtherComp != Character->GetCapsuleComponent()) return;


	UE_LOG(LogTemp, Warning, TEXT("Lava Touched"));

}


