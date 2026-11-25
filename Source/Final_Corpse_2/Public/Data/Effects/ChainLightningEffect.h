// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemEffect.h"
#include "ChainLightningEffect.generated.h"

/**
 * 
 */
UCLASS()
class FINAL_CORPSE_2_API UChainLightningEffect : public UItemEffect
{
	GENERATED_BODY()
	
public:
	virtual void OnAdded(AActor* Owner, int32 Count) override;
	virtual void OnCountChanged(int32 NewCount) override;
	virtual void OnRemoved(AActor* Owner) override;
	
protected:
	UFUNCTION()
	void Chain(const FGameplayEventData& EventData);
};
