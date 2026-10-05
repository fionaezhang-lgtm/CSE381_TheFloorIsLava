// Fill out your copyright notice in the Description page of Project Settings.


#include "LavaHUD.h"
#include "LavaGameMode.h"
#include "Lava.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Font.h"
#include "UObject/ConstructorHelpers.h"

ALavaHUD::ALavaHUD()
{
    PrimaryActorTick.bCanEverTick = false;

    static ConstructorHelpers::FObjectFinder<UFont> FontAsset(
        TEXT("/Game/Fonts/Bremlin_Font.Bremlin_Font")
    );

    if (FontAsset.Succeeded())
    {
        HUD_Font = FontAsset.Object;
    }

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

    // Clock
    const FString TimeText = FString::Printf(
        TEXT("Time: %.1f"),
        GameMode->GetTimeRemaining()
    );


    // Find the location of the lava in the level
    ALava* Lava = Cast<ALava>(
        UGameplayStatics::GetActorOfClass(GetWorld(), ALava::StaticClass())
    );

    if (!Lava) return;

    // Lava height
    const FString LavaHeightText = FString::Printf(
        TEXT("Lava Height: %.1f"),
        Lava->GetRiseHeight()
    );

	//Draws a black rectangle behind the text to make it more readable
    DrawRect( FLinearColor(0.0f, 0.0f, 0.0f, 0.6f),30.0f,30.0f,300.0f,140.0f);

    DrawText(LivesText, FColor::Yellow,LeftMargin,TopMargin, HUD_Font,TextScale);
    DrawText(TimeText, FColor::Yellow, LeftMargin, TopMargin + 40.0f, HUD_Font , TextScale);
    DrawText(LavaHeightText, FColor::Yellow, LeftMargin, TopMargin + 80.0f, HUD_Font, TextScale);


}