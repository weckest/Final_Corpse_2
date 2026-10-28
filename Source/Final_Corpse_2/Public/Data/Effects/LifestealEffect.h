// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemEffect.h"
#include "LifestealEffect.generated.h"

/**
 * 
 */
UCLASS()
class FINAL_CORPSE_2_API ULifestealEffect : public UItemEffect
{
	GENERATED_BODY()
	
public:
	void OnAdded(AActor* Owner, int32 Count) override;
	void OnCountChanged(int32 NewCount) override;
	void OnRemoved(AActor* Owner) override;
};
