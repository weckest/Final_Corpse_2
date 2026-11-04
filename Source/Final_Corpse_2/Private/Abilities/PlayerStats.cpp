// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/PlayerStats.h"

#include <string>

#include "Components/InventoryComponent.h"
#include "Data/ItemDataAsset.h"
#include "Data/Effects/ItemEffect.h"
#include "Engine/Engine.h"


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
	
	OnPlayerStatsUpdated.AddDynamic(this, &UPlayerStats::StatChanged);
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

void UPlayerStats::UpdateMaxStatValue(FGameplayTag StatTag, float Amount, EEditType Type)
{
	float& MaxStat = GetMaxStatValue(StatTag);
	
	switch (Type)
	{
		case EEditType::Add:
			MaxStat += Amount;
			break;
		case EEditType::Multiply:
			MaxStat *= Amount;
			break;
		case EEditType::Divide:
			MaxStat /= Amount;
			break;
		case EEditType::Subtract:
			MaxStat -= Amount;
			break;
	}
	
	OnPlayerStatsUpdated.Broadcast(StatTag);
}

void UPlayerStats::UpdateCurrentStatValue(FGameplayTag StatTag, float Amount, EEditType Type)
{
	float& CurrentStat = GetCurrentStatValue(StatTag);
	
	switch (Type)
	{
	case EEditType::Add:
		CurrentStat += Amount;
		break;
	case EEditType::Multiply:
		CurrentStat *= Amount;
		break;
	case EEditType::Divide:
		CurrentStat /= Amount;
		break;
	case EEditType::Subtract:
		CurrentStat -= Amount;
		break;
	}
	
	OnPlayerStatsUpdated.Broadcast(StatTag);
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

void UPlayerStats::StatChanged(FGameplayTag StatTag)
{
	if (GEngine)
	{
		FString DebugMessage = FString::Printf(TEXT("Stat: %s changed to %f"), *StatTag.ToString(), GetMaxStatValue(StatTag));
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, DebugMessage);
	}
}


