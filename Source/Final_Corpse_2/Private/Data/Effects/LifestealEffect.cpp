// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Effects/LifestealEffect.h"

#include "Abilities/PlayerStats.h"
#include "Character/PlayerCharacter.h"
#include "Engine/Engine.h"
#include "Engine/Player.h"
#include "GameFramework/Actor.h"

void ULifestealEffect::OnAdded(AActor* Player, int32 Count)
{
	Super::OnAdded(Player, Count);
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Player))
	{
		PlayerCharacter->GamePlayEvent.AddDynamic(this, &ULifestealEffect::Lifesteal);
	}
}

void ULifestealEffect::OnCountChanged(int32 NewCount)
{
	Super::OnCountChanged(NewCount);
}

void ULifestealEffect::OnRemoved(AActor* Player)
{
	Super::OnRemoved(Player);
}

void ULifestealEffect::Lifesteal(const FGameplayEventData& GamePlayEventData)
{
	if (GamePlayEventData.EventTag != FGameplayTag::RequestGameplayTag(FName("Event.Enemy.Damaged"))) return;
	
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(Owner))
	{
		if (UPlayerStats* PlayerStats = Player->GetComponentByClass<UPlayerStats>())
		{
			float& Health = PlayerStats->GetMaxStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Health")));
			float& LifeSteal = PlayerStats->GetMaxStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.LifeSteal")));
			
			Health += GamePlayEventData.Magnitude * LifeSteal;
			//Update the UI?????
		}
	}
}
