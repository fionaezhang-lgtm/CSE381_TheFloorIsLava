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
	

	GetWorldTimerManager().SetTimer(
		LevelTimer,
		this,
		&ALavaGameMode::HandleTimeExpired,
		LevelSeconds,
		false
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