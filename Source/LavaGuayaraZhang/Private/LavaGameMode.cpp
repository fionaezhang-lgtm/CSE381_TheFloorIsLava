// Fill out your copyright notice in the Description page of Project Settings.


#include "LavaGameMode.h"

ALavaGameMode::ALavaGameMode()
{
	//stub
}

void ALavaGameMode::ReportKeyCollected()
{
	//stub
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
	//stub
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