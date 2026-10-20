// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Throwable.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
    EquippedSlot = EItemSlot::Cushion;

    FInventorySlot* Slot = Slots.Find(EquippedSlot);
   
    RefreshHeldActor();

}


/** Returns false when that slot is already full. */
bool UInventoryComponent::AddItem(EItemSlot Slot)
{
	FInventorySlot* InventorySlot = Slots.Find(Slot);

    if (!InventorySlot) return false;
    const int32 MaxCapacity = (Slot == EItemSlot::Extinguisher) ? 2 : InventorySlot->Capacity;

    if (InventorySlot->Count >= MaxCapacity) return false;

	++InventorySlot->Count;

	if (Slot == EquippedSlot)
	{
		RefreshHeldActor();
	}

	return true;

}

void UInventoryComponent::EquipSlot(EItemSlot Slot)
{
	if (!Slots.Contains(Slot)) return;
    if (EquippedSlot == Slot) return;
	EquippedSlot = Slot;
	RefreshHeldActor();


}

void UInventoryComponent::ThrowEquipped()
{
    FInventorySlot* InventorySlot = Slots.Find(EquippedSlot);

    if (!InventorySlot ||InventorySlot->Count <= 0 ||
        !InventorySlot->ItemClass ||!GetWorld())
    {
        return;
    }

    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || !Character->GetMesh()) return;

    USkeletalMeshComponent* CharacterMesh = Character->GetMesh();
    if (!CharacterMesh->DoesSocketExist(HandSocketName))
    {
        return;
    }

    const FTransform HandTransform =CharacterMesh->GetSocketTransform(HandSocketName);

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Character;
    SpawnParams.Instigator = Character;
    SpawnParams.SpawnCollisionHandlingOverride =ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AThrowable* Projectile = GetWorld()->SpawnActor<AThrowable>(
        InventorySlot->ItemClass,
        HandTransform,
        SpawnParams
    );

    if (!Projectile) return;
    FVector Direction = Character->GetActorForwardVector();

    if (AController* Controller = Character->GetController())
    {
        Direction = Controller->GetControlRotation().Vector();
    }

    if (IsValid(HeldActor))
    {
        HeldActor->Destroy();
        HeldActor = nullptr;
    }

    Projectile->Throw(Direction);
    --InventorySlot->Count;

    RefreshHeldActor();
}

int32 UInventoryComponent::GetCount(EItemSlot Slot) const
{
	const FInventorySlot* InventorySlot = Slots.Find(Slot);
	return InventorySlot ? InventorySlot->Count : 0;
}

void UInventoryComponent::RefreshHeldActor()
{
    if (IsValid(HeldActor))
    {
        HeldActor->Destroy();
        HeldActor = nullptr;
    }

    FInventorySlot* InventorySlot = Slots.Find(EquippedSlot);

    if (!InventorySlot)
    {
        
        return;
    }

    UE_LOG(LogTemp, Warning,
        TEXT("Inventory: Count=%d, ItemClass=%s"),
        InventorySlot->Count,
        *GetNameSafe(InventorySlot->ItemClass.Get()));

    if (InventorySlot->Count <= 0 || !InventorySlot->ItemClass)
    {
       
        return;
    }

    ACharacter* Character = Cast<ACharacter>(GetOwner());

    if (!Character || !Character->GetMesh() || !GetWorld())
    {
        
        return;
    }

    USkeletalMeshComponent* CharacterMesh = Character->GetMesh();

    if (!CharacterMesh->DoesSocketExist(HandSocketName))
    {
       
        return;
    }

    const FTransform HandTransform =
        CharacterMesh->GetSocketTransform(HandSocketName);

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = Character;
    SpawnParams.SpawnCollisionHandlingOverride =
        ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    HeldActor = GetWorld()->SpawnActor<AThrowable>(
        InventorySlot->ItemClass,
        HandTransform,
        SpawnParams
    );

    if (!HeldActor)
    {
        return;
    }

    HeldActor->SetCarried(true);

    const bool bAttached = HeldActor->AttachToComponent(
        CharacterMesh,
        FAttachmentTransformRules::SnapToTargetNotIncludingScale,
        HandSocketName
    );

    HeldActor->SetActorRelativeLocation(HeldItemLocation);
    HeldActor->SetActorRelativeRotation(HeldItemRotation);
    HeldActor->SetActorRelativeScale3D(HeldItemScale);

    UE_LOG(LogTemp, Warning,
        TEXT("Inventory: Spawned %s; attached=%s"),
        *GetNameSafe(HeldActor),
        bAttached ? TEXT("true") : TEXT("false"));

}

FText UInventoryComponent::GetEquippedItemName() const
{
    switch (EquippedSlot)
    {
    case EItemSlot::Cushion:
        return FText::FromString(TEXT("Cushion"));

    case EItemSlot::Extinguisher:
        return FText::FromString(TEXT("Extinguisher"));

    default:
        return FText::FromString(TEXT("None"));
    }
}

void UInventoryComponent::DebugRefillInventory()
{

    FInventorySlot* CushionSlot = Slots.Find(EItemSlot::Cushion);
    FInventorySlot* ExtinguisherSlot = Slots.Find(EItemSlot::Extinguisher);

    if (CushionSlot)
    {
        CushionSlot->Count = CushionSlot->Capacity;
    }

    if (ExtinguisherSlot)
    {
        ExtinguisherSlot->Count = 2;
    }

    RefreshHeldActor();

}