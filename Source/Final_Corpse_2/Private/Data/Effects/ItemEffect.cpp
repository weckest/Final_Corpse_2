// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Effects/ItemEffect.h"

void UItemEffect::OnAdded(AActor* Player, int32 Count)
{
	this->EffectCount = Count;
}

void UItemEffect::OnCountChanged(int32 NewCount)
{
	this->EffectCount = NewCount;
}
