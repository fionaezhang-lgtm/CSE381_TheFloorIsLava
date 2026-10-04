// Fill out your copyright notice in the Description page of Project Settings.


#include "LavaGameMode.h"
#include "Engine/Engine.h"
#include "LavaHUD.h"

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
	//stub
}

void ALavaGameMode::ReportHatchReached()
{
	//stub
}

void ALavaGameMode::BeginPlay()
{
	Super::BeginPlay();

	LivesLeft = StartingLives;
	TimeRemaining = LevelSeconds;

	GetWorldTimerManager().SetTimer(
		LevelTimer,
		this,
		&ALavaGameMode::UpdateTimeRemaining,
		1.0f,
		true
	);

}

void ALavaGameMode::EndPlay(const EEndPlayReason::Type Reason)
{
	Super::EndPlay(Reason);
	//stub
}

void ALavaGameMode::EndGame(bool bWon)
{
	//stub
}

void ALavaGameMode::HandleTimeExpired()
{
	//stub
}

float ALavaGameMode::GetTimeRemaining() const
{
	return TimeRemaining;
}

void ALavaGameMode::UpdateTimeRemaining()
{
	TimeRemaining -= 1.0f;

	if (TimeRemaining <= 0.0f)
	{
		TimeRemaining = 0.0f;
		HandleTimeExpired();
	}
}

int32 ALavaGameMode::GetLivesLeft() const
{
	return LivesLeft;
}