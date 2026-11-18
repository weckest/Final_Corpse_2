// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Effects/ItemEffect.h"

#include "Engine/Engine.h"

void UItemEffect::OnAdded(AActor* Player, int32 Count)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Adding base Item")));
	}
	this->EffectCount = Count;
}

void UItemEffect::OnCountChanged(int32 NewCount)
{
	this->EffectCount = NewCount;
}

void UItemEffect::OnRemoved(AActor* Player)
{
	bIsAdded = false;
	EffectCount = 0;
}
