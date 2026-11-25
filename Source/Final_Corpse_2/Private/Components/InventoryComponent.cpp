// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"

#include "Data/ItemDataAsset.h"
#include "Data/Effects/ItemEffect.h"
#include "Engine/Engine.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...
	for (auto Item : Items)
	{
		for (UItemEffect* Effect: Item.Key.Get()->Effects)
		{	
			if (!Effect) continue;
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Effect: %s, %d"), *Effect->GetName(), Item.Value));
			}
			Effect->OnAdded(GetOwner(), Item.Value);
			Effects.Add(Effect);
		}
	}
	
}

void UInventoryComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (UItemEffect* Effect : Effects)
	{
		if (Effect)
		{
			Effect->OnRemoved(GetOwner());
		}
	}
	Effects.Empty();

	Super::EndPlay(EndPlayReason);
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UInventoryComponent::AddItem(UItemDataAsset* Item)
{
	
	//JUST TEST CODE: NEED TO ACTUALLY ADD ITEM TO INVENTORY
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan,  Item->GetClass()->GetName());
	}
	return false;
}

int32 UInventoryComponent::GetQuantity(UItemDataAsset* TargetItem) const
{
	return 0;
}

TMap<UItemDataAsset*, int> UInventoryComponent::GetItems() const
{
	TMap<UItemDataAsset*, int> Output;
	for (auto Item : Items)
	{
		Output.Add(Item.Key, Item.Value);
	}
		
	return Output;
}

