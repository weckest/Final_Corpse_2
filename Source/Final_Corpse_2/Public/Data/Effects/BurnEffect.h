// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemEffect.h"
#include "Enemies/BaseEnemy.h"
#include "BurnEffect.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class APlayerCharacter;
struct FGameplayEventData;
/**
 * 
 */
UCLASS()
class FINAL_CORPSE_2_API UBurnEffect : public UItemEffect
{
	GENERATED_BODY()
	
public:
	~UBurnEffect();
	UPROPERTY(EditAnywhere, Category = "Effects")
	float BurnDamage = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	float BurnInterval = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	float BurnLength = 2.0f;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<UNiagaraSystem> BurnEffect;
	
	UPROPERTY(Transient)
	TMap<TObjectPtr<ABaseEnemy>, FTimerHandle> BurnTimerHandle{};
	
	UPROPERTY(Transient)
	TMap<TObjectPtr<ABaseEnemy>, FTimerHandle> BurnIntervalTimerHandle{};
	
	UPROPERTY(Transient)
	TMap<TObjectPtr<ABaseEnemy>, TObjectPtr<UNiagaraComponent>> BurnNiagaraComponents{};
	
	virtual void OnAdded(AActor* Owner, int32 Count) override;
	virtual void OnCountChanged(int32 NewCount) override;
	virtual void OnRemoved(AActor* Owner) override;
	
	UFUNCTION()
	virtual void RemoveEnemy(const FGameplayEventData& GamePlayEventData) override;
	
protected:
	UFUNCTION()
	void Burn(const FGameplayEventData& GamePlayEventData);
	void StopBurningVfx(ABaseEnemy* Enemy);
	
	
};
