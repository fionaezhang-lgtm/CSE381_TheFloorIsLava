// Fill out your copyright notice in the Description page of Project Settings.


#include "LavaGameMode.h"
#include "Engine/Engine.h"
#include "LavaHUD.h"
#include "ResultWidget.h"
#include "Lava.h"
#include "Kismet/GameplayStatics.h"
#include "RoofHatch.h"

ALavaGameMode::ALavaGameMode()
{
	HUDClass = ALavaHUD::StaticClass();

}

void ALavaGameMode::ReportKeyCollected()
{
	if (bGameOver) return;

	KeysCollected++;
	Score+= 200;
	GEngine->AddOnScreenDebugMessage(-1,3.f, FColor::Yellow, FString::Printf(TEXT("Keys: %d /%d"), 
	KeysCollected, KeysRequired));

}

void ALavaGameMode::ReportLifeLost()
{
	if (bGameOver) return;
	LivesLeft--;
	Score -= 100;

	if (LivesLeft <= 0)
	{
		EndGame(false);
	}

	
}

void ALavaGameMode::ReportHatchReached()
{
	if (bGameOver) return;

	if (HasAllKeys())
	{
		const float TimeRemaining = GetTimeRemaining();
		Score += FMath::Max(0, FMath::FloorToInt(TimeRemaining));

		EndGame(true);
	}
}

void ALavaGameMode::BeginPlay()
{
	Super::BeginPlay();

	LivesLeft = StartingLives;

	

	GetWorldTimerManager().SetTimer(
		LevelTimer,
		this,
		&ALavaGameMode::HandleTimeExpired,
		LevelSeconds,
		false
	);

	LavaActor = Cast<ALava>(UGameplayStatics::GetActorOfClass(GetWorld(), ALava::StaticClass()));
	HatchActor = Cast<ARoofHatch>(UGameplayStatics::GetActorOfClass(GetWorld(), ARoofHatch::StaticClass()));
	GetWorldTimerManager().SetTimer(LavaCheckTimer, this, &ALavaGameMode::CheckLavaOverHatch, 0.25f, true);
}

void ALavaGameMode::CheckLavaOverHatch()
{
	if (bGameOver || !LavaActor || !HatchActor) return;
	if (LavaActor->GetActorLocation().Z > HatchActor->GetActorLocation().Z)
	{
		EndGame(false);
	}
}

void ALavaGameMode::EndPlay(const EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
	//stub
}

void ALavaGameMode::EndGame(bool bWon)
{
	if (bGameOver) return;

	bGameOver = true;

	//Stops the clock
	GetWorldTimerManager().ClearTimer(LevelTimer);

	if (LavaActor) LavaActor->SetRiseRate(0.f);

	if (ResultWidgetClass)
	{
		UResultWidget* ResultWidget = CreateWidget<UResultWidget>(
			GetWorld(),
			ResultWidgetClass
		);

		if (ResultWidget)
		{
			ResultWidget->SetupResult(bWon, Score);
			ResultWidget->AddToViewport();

			APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

			if (PlayerController)
			{
				PlayerController->bShowMouseCursor = true;

				FInputModeUIOnly InputMode;
				InputMode.SetWidgetToFocus(ResultWidget->TakeWidget());

				PlayerController->SetInputMode(InputMode);
			}

		}
	}

	ClearTimer(LavaCheckTimer);

}

void ALavaGameMode::HandleTimeExpired()
{
	if (bGameOver) return;

	//Ends the game if time runs out
	
	EndGame(false);
	
}

float ALavaGameMode::GetTimeRemaining() const
{
	return GetWorldTimerManager().GetTimerRemaining(LevelTimer);
}


int32 ALavaGameMode::GetLivesLeft() const
{
	return LivesLeft;
}

void ALavaGameMode::DebugGrantAllKeys()
{

	if (bGameOver) return;

	KeysCollected = KeysRequired;
	Score = KeysRequired * 200;

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.0f,
		FColor::Green,
		TEXT("DEBUG: All keys given.")
	);
}

void ALavaGameMode::DebugSpeedUpLava()
{
	ALava* Lava = Cast<ALava>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ALava::StaticClass())
	);

	if (Lava)
	{
		Lava->SetRiseRate(200.0f);

		GEngine->AddOnScreenDebugMessage(
			-1,
			3.0f,
			FColor::Red,
			TEXT("Lava speed increased.")
		);
	}
}