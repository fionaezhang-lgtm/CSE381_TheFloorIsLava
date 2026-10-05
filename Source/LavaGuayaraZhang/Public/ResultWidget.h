// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class LAVAGUAYARAZHANG_API UResultWidget : public UUserWidget
{
	GENERATED_BODY()
	


public:

	UFUNCTION(BlueprintCallable, Category = "Result")
	void PlayAgain();

	UFUNCTION(BlueprintPure, Category = "Result")
	bool IsGameWon() const { return bWon; }

	UFUNCTION(BlueprintPure, Category = "Result")
	int32 GetFinalScore() const { return FinalScore; }

	void SetupResult(bool bGameWon, int32 Score);

private: 

	bool bWon = false;
	int32 FinalScore = 0;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ResultText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ScoreText;
};

