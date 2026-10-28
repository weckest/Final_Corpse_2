// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/PlayerStats.h"

#include "Components/InventoryComponent.h"
#include "Data/ItemDataAsset.h"
#include "Data/Effects/ItemEffect.h"


// Sets default values for this component's properties
UPlayerStats::UPlayerStats()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	
	for (auto Stat : BaseStats)
	{
		// CurrentStats.Add(Stat.Key, Stat.Value);
		MaxStats.Add(Stat.Key, Stat.Value);
	}
	// ...
}

float& UPlayerStats::GetBaseStatValue(FGameplayTag StatTag)
{
	return BaseStats[StatTag];
}

float& UPlayerStats::GetCurrentStatValue(FGameplayTag StatTag)
{
	return CurrentStats[StatTag];
}

float& UPlayerStats::GetMaxStatValue(FGameplayTag StatTag)
{
	return MaxStats[StatTag];
}

void UPlayerStats::UpdateCurrentStats(UInventoryComponent* InventoryComponent)
{
	auto Items = InventoryComponent->GetItems();
	
	for (auto Stat : MaxStats)
	{
		if (Stat.Value != BaseStats[Stat.Key])
		{
			MaxStats[Stat.Key] = BaseStats[Stat.Key];
		}
	}
	
	for (auto Item : Items)
	{
		if (Item.Value <= 0) continue;
		for (auto Modifier : Item.Key->StatModifiers)
		{
			float* MaxValue = MaxStats.Find(Modifier.StatTag);
			float Value = 0.0f;
			switch  (Modifier.CalculateOperation)
			{
				case EStatModifierOperation::Add:
					Value = Modifier.Magnitude * Item.Value;
					break;
				case EStatModifierOperation::Multiply:
					Value = FMath::Pow(
						1.0f + Modifier.Magnitude, 
						Item.Value) - 1.0f;
					break;
			}
			
			switch (Modifier.EditOperation)
			{
				case EStatModifierOperation::Add:
					*MaxValue += Value;
					break;
				case EStatModifierOperation::Multiply:
					*MaxValue *= 1.0f + Value;
					break;
			}
		}
		
		for (auto Effect: Item.Key->Effects)
		{
			if (UItemEffect* ItemEffect = Cast<UItemEffect>(Effect))
			{
				if (!ItemEffect->Owner)
				{
					ItemEffect->Owner = this;
				}
			}
		}
	}
}


