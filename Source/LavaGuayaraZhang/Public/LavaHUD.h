// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LavaHUD.generated.h"


UCLASS()
class LAVAGUAYARAZHANG_API ALavaHUD : public AHUD
{
    GENERATED_BODY()

public:
    ALavaHUD();

    virtual void DrawHUD() override;

protected:
    UPROPERTY(EditAnywhere, Category = "HUD")
    float TextScale = 1.5f;

    UPROPERTY(EditAnywhere, Category = "HUD")
    float LeftMargin = 50.0f;

    UPROPERTY(EditAnywhere, Category = "HUD")
    float TopMargin = 50.0f;
};