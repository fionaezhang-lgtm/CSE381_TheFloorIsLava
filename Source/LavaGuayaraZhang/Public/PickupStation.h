// PickupStation.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "InventoryComponent.h"   // for EItemSlot
#include "PickupStation.generated.h"


class UBoxComponent;
class UPrimitiveComponent;
class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class LAVAGUAYARAZHANG_API APickupStation : public AActor
{
    GENERATED_BODY()

public:
    APickupStation();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type Reason) override;

    UFUNCTION()
    void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& Sweep);

    void Restock();

    UPROPERTY(EditAnywhere, Category = "Pickup")
    EItemSlot ItemType = EItemSlot::Cushion;

    UPROPERTY(EditAnywhere, Category = "Tuning", meta = (ClampMin = "0.0", Units = "s"))
    float RestockSeconds = 15.f;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    TObjectPtr<UBoxComponent> Trigger;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> StationMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> ExtinguisherMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> CushionMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USceneComponent> SceneRoot;

    bool bAvailable = true;
    FTimerHandle RestockTimer;
};