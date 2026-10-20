// Fill out your copyright notice in the Description page of Project Settings.


#include "LavaHUD.h"
#include "LavaGameMode.h"
#include "Lava.h"
#include "Kismet/GameplayStatics.h"
#include "RoofHatch.h"
#include "LavaCharacter.h"
#include "InventoryComponent.h"


ALavaHUD::ALavaHUD()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ALavaHUD::DrawHUD()
{
	Super::DrawHUD();

	APlayerController* PlayerController = GetOwningPlayerController();

	if (!PlayerController) return;


	ALavaGameMode* GameMode = GetWorld()->GetAuthGameMode<ALavaGameMode>();

	if (!GameMode) return;

	// Lives
	const FString LivesText = FString::Printf(
		TEXT("Lives: %d"),
		GameMode->GetLivesLeft()
	);

	//Score
	const FString ScoreText = FString::Printf(
		TEXT("Score: %d"),
		GameMode->GetScore()
	);

	// Clock
	const FString TimeText = FString::Printf(
		TEXT("Time: %.1f"),
		GameMode->GetTimeRemaining()
	);

	// Keys
	const FString KeysText = FString::Printf(
		TEXT("Keys: %d / %d"),
		GameMode->GetKeysCollected(),
		GameMode->GetKeysRequired()
	);


	// Find the location of the lava in the level
	ALava* Lava = Cast<ALava>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ALava::StaticClass())
	);

	

	ARoofHatch* RoofHatch = Cast<ARoofHatch>(
		UGameplayStatics::GetActorOfClass(
			GetWorld(),
			ARoofHatch::StaticClass()
		)
	);

	//if (!RoofHatch) return;

	// Get the player's position
	APawn* PlayerPawn = PlayerController->GetPawn();

	if (!PlayerPawn) return;

	ALavaCharacter* LavaCharacter = Cast<ALavaCharacter>(PlayerPawn);



	const float PlayerZ = PlayerPawn->GetActorLocation().Z;
	
	
	

	//Draws a black rectangle behind the text to make it more readable
	DrawRect(FLinearColor(0.0f, 0.0f, 0.0f, 0.6f), 30.0f, 30.0f, 300.0f, 300.0f);

	DrawText(LivesText, FColor::Yellow, LeftMargin, TopMargin, nullptr, TextScale);
	DrawText(ScoreText, FColor::Yellow, LeftMargin, TopMargin + 40.0f, nullptr, TextScale);
	DrawText(TimeText, FColor::Yellow, LeftMargin, TopMargin + 80.0f, nullptr, TextScale);
	if (Lava && RoofHatch)
	{
		const float LavaZ = Lava->GetActorLocation().Z;
		const float RoofZ = RoofHatch->GetActorLocation().Z;

		// Distance between the player and the lava
		const float HeightDifference = PlayerZ - LavaZ;

		// Lava height
		const FString LavaHeightText = FString::Printf(
			TEXT("Lava Height: %.1f"),
			Lava->GetRiseHeight()
		);
		DrawText(LavaHeightText, FColor::Yellow, LeftMargin, TopMargin + 120.0f, nullptr, TextScale);
	}
	DrawText(KeysText, FColor::Yellow, LeftMargin, TopMargin + 160.0f, nullptr, TextScale);

	if (LavaCharacter && LavaCharacter->GetInventory())
	{
		UInventoryComponent* Inventory = LavaCharacter->GetInventory();

		const FText EquippedItemName =Inventory->GetEquippedItemName();

		const int32 CushionCount = Inventory->GetCount(EItemSlot::Cushion);
		const int32 ExtinguisherCount = Inventory->GetCount(EItemSlot::Extinguisher);
		const EItemSlot EquippedSlot = Inventory->GetEquippedSlot();

		const FString CushionText = FString::Printf(
			TEXT("%s Cushion: %d"),
			EquippedSlot == EItemSlot::Cushion ? TEXT(">") : TEXT(" "),
			CushionCount
		);

		const FString ExtinguisherText = FString::Printf(
			TEXT("%s Extinguisher: %d"),
			EquippedSlot == EItemSlot::Extinguisher ? TEXT(">") : TEXT(" "),
			ExtinguisherCount
		);

		const FColor CushionColor =
			EquippedSlot == EItemSlot::Cushion
			? FColor::Green
			: FColor::White;

		const FColor ExtinguisherColor =
			EquippedSlot == EItemSlot::Extinguisher
			? FColor::Green
			: FColor::White;

		DrawText(CushionText,CushionColor,LeftMargin,TopMargin + 200.0f,nullptr,TextScale);
		DrawText(ExtinguisherText,ExtinguisherColor,LeftMargin,TopMargin + 240.0f,nullptr,TextScale);


	}

	//Draws the distance between the player and lava

	// Get the size of the game viewport
	int32 ViewportX = 0;
	int32 ViewportY = 0;

	PlayerController->GetViewportSize(ViewportX, ViewportY);


	// Tracker size
	const float TrackerWidth = 40.0f;
	const float TrackerHeight = 300.0f;

	// How much world space the tracker shows
	const float RangeBelow = 1000.0f;
	const float RangeAbove = 2000.0f;

	// Distance from the top-right corner
	const float TrackerRightMargin = 40.0f;
	const float TrackerTopMargin = 40.0f;

	// Position tracker in the top-right corner
	const float TrackerX = ViewportX - TrackerWidth - TrackerRightMargin;
	const float TrackerTop = TrackerTopMargin;

	

	if (Lava && RoofHatch)
	{
		// Background
		DrawRect(
			FLinearColor(0.05f, 0.05f, 0.05f, 0.8f),
			TrackerX,
			TrackerTop,
			TrackerWidth,
			TrackerHeight
		);
		const float LavaZ = Lava->GetActorLocation().Z;
		const float RoofZ = RoofHatch->GetActorLocation().Z;
		const float TrackerMinZ = FMath::Min(PlayerZ, LavaZ) - 500.0f;
		const float TrackerMaxZ = FMath::Max(PlayerZ, RoofZ) + 500.0f;
		const float TrackerRange = TrackerMaxZ - TrackerMinZ;


		const float PlayerAlpha =
			FMath::Clamp(
				(PlayerZ - TrackerMinZ) / TrackerRange,
				0.0f,
				1.0f
			);

		const float LavaAlpha =
			FMath::Clamp(
				(LavaZ - TrackerMinZ) / TrackerRange,
				0.0f,
				1.0f
			);


		const float PlayerY =
			TrackerTop + TrackerHeight * (1.0f - PlayerAlpha);

		const float LavaY =
			TrackerTop + TrackerHeight * (1.0f - LavaAlpha);


		const float LavaMarkerHeight = 30.0f;

		DrawRect(
			FLinearColor(1.0f, 0.1f, 0.0f, 1.0f),
			TrackerX + 5.0f,
			LavaY - LavaMarkerHeight / 2.0f,
			TrackerWidth - 10.0f,
			LavaMarkerHeight
		);



		const float PlayerMarkerSize = 20.0f;

		DrawRect(
			FLinearColor(0.1f, 0.7f, 1.0f, 1.0f),
			TrackerX + 10.0f,
			PlayerY - PlayerMarkerSize / 2.0f,
			PlayerMarkerSize,
			PlayerMarkerSize
		);

		DrawText(TEXT("PLAYER"), FColor::White, TrackerX - 80.0f, PlayerY - 10.0f, nullptr, 1.0f);
		DrawText(TEXT("LAVA"), FColor::White, TrackerX - 65.0f, LavaY - 10.0f, nullptr, 1.0f);

		const float HeightDifference = PlayerZ - LavaZ;


		const FString GapText = FString::Printf(
			TEXT("Gap: %.0f cm"),
			HeightDifference
		);

		DrawText(GapText, FColor::Yellow, TrackerX - 10.0f, TrackerTop + TrackerHeight + 20.0f, nullptr, 1.0f);


		const float LavaRiseRate = Lava->GetRiseRate();
		float TimeUntilRoof = -1.0f;

		if (LavaRiseRate > 0.0f && RoofZ > LavaZ)
		{
			TimeUntilRoof = (RoofZ - LavaZ) / LavaRiseRate;
		}

		const FString RoofTimeText =
			TimeUntilRoof >= 0.0f
			? FString::Printf(TEXT("Roof in: %.1f s"), TimeUntilRoof)
			: TEXT("Roof reached");

		DrawText(RoofTimeText, FColor::Yellow, TrackerX - 100.0f, TrackerTop + TrackerHeight + 45.0f, nullptr, 1.0f);

		const float RoofAlpha =
			FMath::Clamp(
				(RoofZ - TrackerMinZ) / TrackerRange,
				0.0f,
				1.0f
			);

		const float RoofY = TrackerTop + TrackerHeight * (1.0f - RoofAlpha);

		const float RoofMarkerSize = 16.0f;
		DrawRect(
			FLinearColor(0.2f, 1.0f, 0.2f, 1.0f),
			TrackerX + 12.0f,
			RoofY - RoofMarkerSize / 2.0f,
			RoofMarkerSize,
			RoofMarkerSize
		);
		DrawText(TEXT("ROOF"), FColor::Green, TrackerX - 65.0f, RoofY - 10.0f, nullptr, 1.0f);
	}

	const float CenterX = ViewportX * 0.5f - 25.0f;
	const float CenterY = ViewportY * 0.5f - 50.0f;

	const float CrosshairSize = 10.0f;
	const float CrosshairThickness = 2.0f;

	DrawRect(FLinearColor::White,CenterX -CrosshairThickness / 2.0f,	CenterY - CrosshairSize,CrosshairThickness,CrosshairSize * 2.0f);

	DrawRect(FLinearColor::White,CenterX -CrosshairSize,CenterY - CrosshairThickness / 2.0f,CrosshairSize * 2.0f,CrosshairThickness);


}