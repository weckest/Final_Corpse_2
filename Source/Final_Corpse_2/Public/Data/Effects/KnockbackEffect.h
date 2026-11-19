// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemEffect.h"
#include "KnockbackEffect.generated.h"

/**
 * 
 */
UCLASS()
class FINAL_CORPSE_2_API UKnockbackEffect : public UItemEffect
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Effects")
	float KnockbackForce = 1.0f;
	
	virtual void OnAdded(AActor* Owner, int32 Count) override;
	virtual void OnCountChanged(int32 NewCount) override;
	virtual void OnRemoved(AActor* Owner) override;
	
protected:
	UFUNCTION()
	void Knockback(const FGameplayEventData& GamePlayEventData);
};
