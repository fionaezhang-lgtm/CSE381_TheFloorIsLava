#include "Lava.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "LavaCharacter.h"



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
	UE_LOG(LogTemp, Warning, TEXT("Lava start Z=%f, box extent=%s"), StartZ, *Volume->GetScaledBoxExtent().ToString());

	
	Volume->OnComponentBeginOverlap.AddDynamic(this, &ALava::HandleOverlap);
	
}

// Called every frame
void ALava::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldOffset(FVector(0.f,0.f,RiseRate * DeltaTime));

	TArray<AActor*> Overlapping;
	Volume->GetOverlappingActors(Overlapping, ALavaCharacter::StaticClass());
	for (AActor* Actor : Overlapping)
	{
		if (ALavaCharacter* Character = Cast<ALavaCharacter>(Actor))
		{
			Character->HandleLavaTouch();
		}
	}

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
	UE_LOG(LogTemp, Warning, TEXT("Lava overlapped by %s"), *GetNameSafe(OtherActor));
	ALavaCharacter *character = Cast<ALavaCharacter>(OtherActor);
	
	if (!character) return;

	character->HandleLavaTouch();


}


float ALava::GetRiseRate() const
{
	return RiseRate;
}
