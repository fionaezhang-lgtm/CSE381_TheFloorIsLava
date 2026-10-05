// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"

void UResultWidget::PlayAgain()
{
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
}


void UResultWidget::SetupResult(bool bGameWon, int32 Score)
{
    bWon = bGameWon;
    FinalScore = Score;

    if (ResultText)
    {
        ResultText->SetText(FText::FromString(bWon ? TEXT("YOU WIN!") : TEXT("GAME OVER")));
    }

    if (ScoreText)
    {
        ScoreText->SetText(FText::Format(
                FText::FromString(TEXT("Score: {0}")),
                FinalScore
                )
        );
    }
}

