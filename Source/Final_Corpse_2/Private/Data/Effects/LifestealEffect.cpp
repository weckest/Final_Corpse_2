// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Effects/LifestealEffect.h"

#include "Character/PlayerCharacter.h"
#include "GameFramework/Actor.h"

void ULifestealEffect::OnAdded(AActor* Owner, int32 Count)
{
	Super::OnAdded(Owner, Count);
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(Owner))
	{
		// Player->OnGameplayEvent.AddDynamic
	}
}

void ULifestealEffect::OnCountChanged(int32 NewCount)
{
	Super::OnCountChanged(NewCount);
}

void ULifestealEffect::OnRemoved(AActor* Owner)
{
	Super::OnRemoved(Owner);
}
