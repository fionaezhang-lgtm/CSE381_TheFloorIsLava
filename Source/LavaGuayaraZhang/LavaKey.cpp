#include "LavaKey.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "LavaCharacter.h"
#include "LavaGameMode.h"



// Sets default values
ALavaKey::ALavaKey()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Key"));
	Mesh -> SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(Mesh);

	PickupRange = CreateDefaultSubobject<USphereComponent>(TEXT("Pickup"));
	PickupRange->SetupAttachment(Mesh);
	PickupRange->InitSphereRadius(75.0f);
	PickupRange->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	BobHeight = 25.0f;
	BobSpeed = 5.0f;

}

// Called when the game starts or when spawned
void ALavaKey::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
	PickupRange->OnComponentBeginOverlap.AddDynamic(this, &ALavaKey::HandleOverlap);
	
}

// Called every frame
void ALavaKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator spinRotation = FRotator(0.0f, SpinRate * DeltaTime, 0.0f);
	AddActorLocalRotation(spinRotation);

	FVector CurrentLocation = GetActorLocation();
	float RunningTIme = GetGameTimeSinceCreation();

	CurrentLocation.Z = StartLocation.Z + (FMath::Sin(RunningTIme * BobSpeed) * BobHeight);
	SetActorLocation(CurrentLocation);

}

void ALavaKey::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& Sweep){
	if (!Cast<ALavaCharacter>(OtherActor)) return;

	ALavaGameMode* GameMode = GetWorld()->GetAuthGameMode<ALavaGameMode>();
	if (!GameMode) return;

	GameMode->ReportKeyCollected();
	Destroy();
}

