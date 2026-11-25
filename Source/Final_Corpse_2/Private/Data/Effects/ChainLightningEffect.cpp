// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Effects/ChainLightningEffect.h"

#include "Character/PlayerCharacter.h"
#include "Engine/Engine.h"

void UChainLightningEffect::OnAdded(AActor* Owner, int32 Count)
{
	Super::OnAdded(Owner, Count);
	if (!bIsAdded)
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Owner))
		{
			PlayerCharacter->GamePlayEvent.AddDynamic(this, &UChainLightningEffect::Chain);
			bIsAdded = true;
			
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Chain Lightning Added")));
			}
		}
	}
}

void UChainLightningEffect::OnCountChanged(int32 NewCount)
{
	Super::OnCountChanged(NewCount);
}

void UChainLightningEffect::OnRemoved(AActor* Owner)
{
	Super::OnRemoved(Owner);
}

void UChainLightningEffect::Chain(const FGameplayEventData& EventData)
{
	if (EventData.EventTag != FGameplayTag::RequestGameplayTag(FName("Event.Enemy.Hit")) && Cast<UChainLightningEffect>(EventData.Actor.GetClass())) return;
	
	GetWorld()->Actors
}
