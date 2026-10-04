// Fill out your copyright notice in the Description page of Project Settings.


#include "LavaGameMode.h"
#include "Engine/Engine.h"

ALavaGameMode::ALavaGameMode()
{
	//stub
}

void ALavaGameMode::ReportKeyCollected()
{
	if (bGameOver) return;

	KeysCollected++;
	Score+= 200;
	GEngine->AddOnScreenDebugMessage(-1,3.f, FColor::Yellow, FSTring::Printf(TEXT("Keys: %d /%d"), 
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
	//stub
	return 0.f;
}