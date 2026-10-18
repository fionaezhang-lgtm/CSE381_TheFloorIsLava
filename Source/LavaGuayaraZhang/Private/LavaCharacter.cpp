
#include "LavaCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LavaGameMode.h"
#include "Engine/Engine.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Lava.h"
#include "EngineUtils.h"

// Sets default values
ALavaCharacter::ALavaCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetCharacterMovement()->JumpZVelocity = JumpZVelocity;
	GetCharacterMovement()->AirControl = AirControl;
	GetCharacterMovement()->GravityScale = GravityScale;

	JumpMaxCount = MaxJumpCount;

	// Create camera boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	Inventory = CreateDefaultSubobject<UInventoryComponent>(
		TEXT("Inventory")
	);

	
}




// Called when the game starts or when spawned
void ALavaCharacter::BeginPlay()
{
	Super::BeginPlay();

	//check(GEngine != nullptr);

	GetCharacterMovement()->JumpZVelocity = JumpZVelocity;
	GetCharacterMovement()->AirControl = AirControl;
	GetCharacterMovement()->GravityScale = GravityScale;
	JumpMaxCount = MaxJumpCount;

	InitialSpawnLocation = GetActorLocation();
	LastSafeLocation = InitialSpawnLocation;
	SafeLocationHistory.Add(LastSafeLocation);
	GetWorldTimerManager().SetTimer(
		SafeLocationTimer,
		this,
		&ALavaCharacter::UpdateSafeLocation,
		SafeLocationInterval,
		true
	);

	GetMesh()->CastShadow = true;
	GetMesh()->bCastHiddenShadow = true;

	// Get the player controller for this character
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(ThirdPersonContext, 0);
		}
	}
	
}

// Called every frame
void ALavaCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALavaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Bind Movement Actions
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALavaCharacter::Move);

		// Bind Jump Actions
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Bind Look Actions
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALavaCharacter::Look);

		EnhancedInputComponent->BindAction(
			ThrowAction,
			ETriggerEvent::Started,
			this,
			&ALavaCharacter::ThrowItem
		);

		EnhancedInputComponent->BindAction(
			SwapItemAction,
			ETriggerEvent::Started,
			this,
			&ALavaCharacter::SwapItem
		);

		EnhancedInputComponent->BindAction(
			DebugSpeedLavaAction,
			ETriggerEvent::Started,
			this,
			&ALavaCharacter::DebugSpeedLava
		);

		EnhancedInputComponent->BindAction(
			DebugAllKeysAction,
			ETriggerEvent::Started,
			this,
			&ALavaCharacter::DebugAllKeys
		);

	}

}

void ALavaCharacter::Move(const FInputActionValue& Value)
{

	// 2D Vector of movement values returned from the input action
	const FVector2D MovementValue = Value.Get<FVector2D>();

	// Check if the controller possessing this Actor is valid
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();

		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Forward, MovementValue.Y);
		AddMovementInput(Right, MovementValue.X);
	}
}

void ALavaCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void ALavaCharacter::UpdateSafeLocation()
{
	if (GetCharacterMovement()->IsFalling() || bInLava) return;
	float LavaTopZ = -FLT_MAX;

	for (TActorIterator<ALava> It(GetWorld()); It; ++It)
	{
		LavaTopZ = FMath::Max(LavaTopZ, It->GetLavaTopZ());
	}

	const float CharacterBottomZ =
		GetActorLocation().Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	if (CharacterBottomZ <= LavaTopZ + 10.0f) return;

	LastSafeLocation = GetActorLocation();
	SafeLocationHistory.Add(LastSafeLocation);

	if (SafeLocationHistory.Num() > 100)
	{
		SafeLocationHistory.RemoveAt(0);
	}
}

void ALavaCharacter::Respawn()
{

	FVector RespawnLocation = InitialSpawnLocation;
	bool bFoundSafeLocation = false;
	float LavaTopZ = -FLT_MAX;

	for (TActorIterator<ALava> It(GetWorld()); It; ++It)
	{
		LavaTopZ = FMath::Max(LavaTopZ, It->GetLavaTopZ());
	}

	const float CapsuleHalfHeight =
		GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	for (int32 i = SafeLocationHistory.Num() - 1; i >= 0; --i)
	{
		const FVector Candidate = SafeLocationHistory[i];

		if (Candidate.Z - CapsuleHalfHeight > LavaTopZ + 10.0f)
		{
			RespawnLocation = Candidate;
			bFoundSafeLocation = true;
			break;
		}
	}

	if (!bFoundSafeLocation &&
		InitialSpawnLocation.Z - CapsuleHalfHeight > LavaTopZ + 10.0f)
	{
		RespawnLocation = InitialSpawnLocation;
		bFoundSafeLocation = true;
	}

	if (bFoundSafeLocation)
	{
		SetActorLocation(RespawnLocation, false, nullptr,
			ETeleportType::TeleportPhysics);
	}
	else
	{
		UE_LOG(LogTemp, Error,
			TEXT("No safe respawn location."));
	}
	
	GetCharacterMovement()->StopMovementImmediately();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			LavaStunTime,
			FColor::Red,
			TEXT("You touched the lava.")
		);
	}


	GetCharacterMovement()->DisableMovement();

	GetWorldTimerManager().SetTimer(
		LavaStunTimer,
		this,
		&ALavaCharacter::EndLavaStun,
		LavaStunTime,
		false
	);


}

void ALavaCharacter::EndLavaStun()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Falling);
}


void ALavaCharacter::DebugSpeedLava()
{
	ALavaGameMode* GameMode = GetWorld()->GetAuthGameMode<ALavaGameMode>();

	if (GameMode) GameMode->DebugSpeedUpLava();
	
}

void ALavaCharacter::DebugAllKeys()
{
	ALavaGameMode* GameMode = GetWorld()->GetAuthGameMode<ALavaGameMode>();

	if (GameMode) GameMode->DebugGrantAllKeys();
	
}

void ALavaCharacter::HandleLavaTouch()
{
	UE_LOG(LogTemp, Warning, TEXT("HandleLavaTouch called"));
	if (bInLava) return;
	ALavaGameMode* GameMode = GetWorld()->GetAuthGameMode<ALavaGameMode>();
	

	if (GameMode == NULL) return;
	
	bInLava = true;
	GameMode->ReportLifeLost();

	LaunchCharacter(FVector(0.f,0.f, LavaLaunchSpeed), true , true);

	APlayerController* PC = Cast<APlayerController>(GetController());
	if(PC) {
		DisableInput(PC);
		if (PC->PlayerCameraManager)
		{
			PC->PlayerCameraManager->StartCameraFade(0.f, 1.f, LavaFreezeTime, FLinearColor::Red, false, true);
		}
	}

	GetWorldTimerManager().SetTimer(RespawnTimer, this, &ALavaCharacter::FinishLavaRespawn, LavaFreezeTime, false);
	

	
	
}

void ALavaCharacter::FinishLavaRespawn()
{
	Respawn();

	ALavaGameMode* GM = GetWorld()->GetAuthGameMode<ALavaGameMode>();
	const bool bOver = GM && GM->IsGameOver();

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (PC && !bOver)
	{
		EnableInput(PC);
		if (PC->PlayerCameraManager)
			PC->PlayerCameraManager->StartCameraFade(1.f, 0.f, 0.3f, FLinearColor::Red);
	}
	bInLava = false;
}

void ALavaCharacter::ThrowItem()
{
	if (Inventory)
	{
		Inventory->ThrowEquipped();
	}
}

void ALavaCharacter::SwapItem()
{
	if (!Inventory) return;

	const EItemSlot NewSlot =
		Inventory->GetEquippedSlot() == EItemSlot::Cushion
		? EItemSlot::Extinguisher
		: EItemSlot::Cushion;

	Inventory->EquipSlot(NewSlot);
}