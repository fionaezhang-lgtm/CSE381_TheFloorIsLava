// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h" 
#include "InputActionValue.h"
#include "TimerManager.h"
#include "LavaCharacter.generated.h"


class UInputMappingContext;
class UInputAction;
class UInputComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class LAVAGUAYARAZHANG_API ALavaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALavaCharacter();

	UFUNCTION()
	void Respawn();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputMappingContext> ThirdPersonContext;

	// Move Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;

	// Jump Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> JumpAction;

	// Look Input Actions
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Tuning")
	float LavaStunTime = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> DebugSpeedLavaAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> DebugAllKeysAction;

	UPROPERTY(EditAnywhere, Category = "Tuning")
	float JumpZVelocity = 600.0f;

	UPROPERTY(EditAnywhere, Category = "Tuning")
	float AirControl = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Tuning")
	float GravityScale = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Tuning")
	int32 MaxJumpCount = 2;

	UPROPERTY(EditAnywhere, Category = "Tuning")
	float SafeLocationInterval = 0.5f;

	FVector LastSafeLocation;

	FTimerHandle SafeLocationTimer;
	FTimerHandle LavaStunTimer;

	void EndLavaStun();



	void UpdateSafeLocation();
	void DebugSpeedLava();
	void DebugAllKeys();

	

	




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Handles 2D Movement Input
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	// Handles Look Input
	UFUNCTION()
	void Look(const FInputActionValue& Value);

	// Third-person camera boom
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<USpringArmComponent> CameraBoom;

	// Follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	TObjectPtr<UCameraComponent> FollowCamera;

	// Third-person field of view
	UPROPERTY(EditAnywhere, Category = Camera)
	float ThirdPersonFieldOfView = 70.0f;


	void HandleLavaTouch();

	bool bInLava = false;

	UPROPERTY(EditAnywhere, Category = "Tuning", meta = (Units = "cm/s"))
	float LavaLaunchSpeed = 900.f;

	UPROPERTY(EditAnywhere, Category = "Tuning", meta = (ClampMin = "0.0", Units = "s"))
	float LavaFreezeTime = 1.0f;

	FTimerHandle RespawnTimer;

	void FinishLavaRespawn();
	

	

};
