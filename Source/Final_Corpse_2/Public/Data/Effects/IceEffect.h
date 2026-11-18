// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemEffect.h"
#include "IceEffect.generated.h"

struct FGameplayEventData;
/**
 * 
 */
UCLASS()
class FINAL_CORPSE_2_API UIceEffect : public UItemEffect
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	float SlowAmount = 0.9f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effects")
	float SlowTime = 2.0f;
	
	FTimerHandle SlowTimerHandle;
	
	void OnAdded(AActor* Owner, int32 Count) override;
	void OnCountChanged(int32 NewCount) override;
	void OnRemoved(AActor* Owner) override;
	
	UFUNCTION()
	void RemoveEnemy(const FGameplayEventData& GamePlayEventData) override;
	
protected:
	UFUNCTION()
	void Slow(const FGameplayEventData& GamePlayEventData);
	
	
};
