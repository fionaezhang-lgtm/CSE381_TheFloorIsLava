// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h" 
#include "InputActionValue.h"
#include "LavaCharacter.generated.h"


class UInputMappingContext;
class UInputAction;
class UInputComponent;

UCLASS()
class LAVAGUAYARAZHANG_API ALavaCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALavaCharacter();

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

	// Third Person camera
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> ThirdPersonCameraComponent;

	// Offset for the third-person camera
	UPROPERTY(EditAnywhere, Category = Camera)
	FVector ThirdPersonCameraOffset = FVector(-300.0f, 0.0f, 150.0f);

	// Third-person primitives field of view
	UPROPERTY(EditAnywhere, Category = Camera)
	float ThirdPersonFieldOfView = 70.0f;

	// Third-person primitives view scale
	UPROPERTY(EditAnywhere, Category = Camera)
	float ThirdPersonScale = 0.6f;

	//// Third-person mesh, visible only to the owning player
	//UPROPERTY(VisibleAnywhere, Category = Mesh)
	//TObjectPtr<USkeletalMeshComponent> ThirdPersonMeshComponent;


};
