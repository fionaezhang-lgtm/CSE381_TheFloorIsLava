// LavaGameMode.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/Engine.h"
#include "LavaGameMode.generated.h"



class UResultWidget;
class ALava;
class ARoofHatch;

UCLASS()
class LAVAGUAYARAZHANG_API ALavaGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ALavaGameMode();

    /** A key was picked up. The key itself does not know what that means. */
    UFUNCTION(BlueprintCallable, Category = "Rules")
    void ReportKeyCollected();

    /** The character touched lava. */
    UFUNCTION(BlueprintCallable, Category = "Rules")
    void ReportLifeLost();

    /** The player reached the hatch. The hatch does not check the keys itself. */
    UFUNCTION(BlueprintCallable, Category = "Rules")
    void ReportHatchReached();

    UFUNCTION(BlueprintPure, Category = "Rules")
    bool HasAllKeys() const { return KeysCollected >= KeysRequired; }

    UFUNCTION(BlueprintPure, Category = "Rules")
    int32 GetKeysCollected() const { return KeysCollected; }

    UFUNCTION(BlueprintPure, Category = "Rules")
    float GetTimeRemaining() const;


    UFUNCTION(BlueprintPure, Category = "Rules")
    int32 GetLivesLeft() const;

    UFUNCTION(BlueprintPure, Category = "Rules")
    int32 GetScore() const;

    UFUNCTION(BlueprintPure, Category = "Rules")
    int32 GetKeysRequired() const { return KeysRequired; }


    //This one isn't implmented since the lava isn't rising yet. 
    UFUNCTION(BlueprintCallable, Category = "Debug")
    void DebugSpeedUpLava();

    UFUNCTION(BlueprintCallable, Category = "Debug")
    void DebugGrantAllKeys();

    UFUNCTION(BlueprintCallable, Category = "Rules")
    void RestartRun();

    UFUNCTION(BlueprintCallable, Category = "Debug")
    void DebugJumpToBuilding2();
    
    
    bool IsGameOver() const { return bGameOver; }



protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type Reason) override;

    void AdvanceToNextBuilding();

    void EndGame(bool bWon);
    void HandleTimeExpired();


    UPROPERTY(EditDefaultsOnly, Category = "Tuning", meta = (ClampMin = "1"))
    int32 KeysRequired = 3;

    UPROPERTY(EditDefaultsOnly, Category = "Tuning", meta = (ClampMin = "1"))
    int32 StartingLives = 3;

    UPROPERTY(EditDefaultsOnly, Category = "Tuning", meta = (ClampMin = "0.0", Units = "s"))
    float LevelSeconds = 240.f;

    /** Assign your WBP_Result child of this on BP_LavaGameMode. */
     
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UResultWidget> ResultWidgetClass;

    UPROPERTY(BlueprintReadOnly, Category = "Rules")
    int32 KeysCollected = 0;

    UPROPERTY(BlueprintReadOnly, Category = "Rules")
    int32 LivesLeft = 3;

    

    bool bGameOver = false;
    FTimerHandle LevelTimer;

    void CheckLavaOverHatch();
    UPROPERTY() TObjectPtr<ALava> LavaActor;
    UPROPERTY() TObjectPtr<ARoofHatch> HatchActor;
    FTimerHandle LavaCheckTimer;

    UPROPERTY(EditDefaultsOnly, Category = "Levels")
    FName FirstBuildingLevel = TEXT("Building1");

    UPROPERTY(EditDefaultsOnly, Category = "Levels")
    FName SecondBuildingLevel = TEXT("Building2");

    UPROPERTY(EditDefaultsOnly, Category = "Tuning", meta = (ClampMin = "1.0"))
    float SecondBuildingLavaMultiplier = 1.6f;
};