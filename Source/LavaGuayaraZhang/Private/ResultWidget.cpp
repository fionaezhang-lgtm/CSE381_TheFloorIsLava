// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultWidget.h"
#include "Kismet/GameplayStatics.h"

void UResultWidget::PlayAgain()
{
    UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
}


