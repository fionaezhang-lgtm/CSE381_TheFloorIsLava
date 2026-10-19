// Fill out your copyright notice in the Description page of Project Settings.


#include "LavaGameMode.h"
#include "Engine/Engine.h"
#include "LavaHUD.h"
#include "ResultWidget.h"
#include "Lava.h"
#include "Kismet/GameplayStatics.h"
#include "RoofHatch.h"
#include "GameFramework/PlayerController.h"

ALavaGameMode::ALavaGameMode()
{
	HUDClass = ALavaHUD::StaticClass();

}

void ALavaGameMode::ReportKeyCollected()
{
	if (bGameOver) return;
	if (ULavaGameInstance* Run = GetGameInstance<ULavaGameInstance>()) { Run->AddScore(200); }
	KeysCollected++;
	
	GEngine->AddOnScreenDebugMessage(-1,3.f, FColor::Yellow, FString::Printf(TEXT("Keys: %d /%d"), 
	KeysCollected, KeysRequired));

}

void ALavaGameMode::ReportLifeLost()
{
	if (bGameOver) return;
    LivesLeft--;
    if (ULavaGameInstance* Run = GetGameInstance<ULavaGameInstance>()) { Run->AddScore(-100); }
    if (LivesLeft <= 0) { EndGame(false); }

}

void ALavaGameMode::ReportHatchReached()
{
	if (bGameOver) return;

	if (HasAllKeys())
	{
		LavaGameInstance* Run = GetGameInstance<ULavaGameInstance>();
        if (Run) { Run->AddScore(FMath::Max(0, FMath::FloorToInt(GetTimeRemaining()))); }

        const bool bLastBuilding = !Run || Run->BuildingIndex >= 1;
        if (bLastBuilding) { EndGame(true); }
        else { AdvanceToNextBuilding(); }
	}
	else if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Orange,
			FString::Printf(TEXT("Hatch locked: %d / %d keys"), KeysCollected, KeysRequired));
	}
}

void ALavaGameMode::AdvanceToNextBuilding()
{
    bGameOver = true;   // freeze the rules 
    GetWorldTimerManager().ClearTimer(LevelTimer);
    GetWorldTimerManager().ClearTimer(LavaCheckTimer);

    if (ULavaGameInstance* Run = GetGameInstance<ULavaGameInstance>()) { Run->BuildingIndex = 1; }

    UGameplayStatics::OpenLevel(this, SecondBuildingLevel);
}

void ALavaGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly());
		PC->SetPause(false);
	}

	LivesLeft = StartingLives;

	

	GetWorldTimerManager().SetTimer(
		LevelTimer,
		this,
		&ALavaGameMode::HandleTimeExpired,
		LevelSeconds,
		false
	);

	LavaActor = Cast<ALava>(UGameplayStatics::GetActorOfClass(GetWorld(), ALava::StaticClass()));

	if (LavaActor)
	{
		const ULavaGameInstance* Run = GetGameInstance<ULavaGameInstance>();
		if (Run && Run->BuildingIndex >= 1)
		{
			LavaActor->SetRiseRate(LavaActor->GetRiseRate() * SecondBuildingLavaMultiplier);
		}
	}
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
			ResultWidget->SetupResult(bWon, GetScore());
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

	GetWorldTimerManager().ClearTimer(LavaCheckTimer);

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

	const int32 Missing = KeysRequired - KeysCollected;
	if (Missing <= 0) return;

	KeysCollected = KeysRequired;
	Run->AddScore(Missing * 200)

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

void ALavaGameMode::RestartRun()
{
    if (ULavaGameInstance* Run = GetGameInstance<ULavaGameInstance>()) { Run->ResetRun(); }
    UGameplayStatics::OpenLevel(this, FirstBuildingLevel);
}

int32 ALavaGameMode::GetScore() const
{
    const ULavaGameInstance* Run = GetGameInstance<ULavaGameInstance>();
    return Run ? Run->Score : 0;
}

void ALavaGameMode::DebugJumpToBuilding2()
{
    if (ULavaGameInstance* Run = GetGameInstance<ULavaGameInstance>())
    {
        Run->BuildingIndex = 1;
        Run->Score = 800;
    }
    UGameplayStatics::OpenLevel(this, SecondBuildingLevel);
}