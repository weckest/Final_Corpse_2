// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemEffect.h"
#include "AcidEffect.generated.h"

class APlayerCharacter;
class UNiagaraComponent;
class UNiagaraSystem;
/**
 * 
 */
USTRUCT()
struct FAcidInstance
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> NiagaraComponent = nullptr;

	UPROPERTY()
	TObjectPtr<APlayerCharacter> SourcePlayer = nullptr;

	FVector Location = FVector::ZeroVector;

	FTimerHandle DamageTimerHandle;
	FTimerHandle ExpirationTimerHandle;
};
UCLASS()
class FINAL_CORPSE_2_API UAcidEffect : public UItemEffect
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Effects")
	float AcidDamage = 1.0f;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	float AcidInterval = 0.5f;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	float AcidLength = 2.0f;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	float AcidRange = 1000.0f;
	
	UPROPERTY(EditAnywhere, Category = "Effects")
	TObjectPtr<UNiagaraSystem> AcidEffect;
	
	UPROPERTY()
	TMap<FGuid, FAcidInstance> AcidInstances;
	
	
	virtual void OnAdded(AActor* Owner, int32 Count) override;
	virtual void OnCountChanged(int32 NewCount) override;
	virtual void OnRemoved(AActor* Owner) override;
	
protected:
	UFUNCTION()
	void SpawnAcid(const FGameplayEventData& EventData);
	void ApplyAcidDamage(FVector AcidLocation, float SearchRange, const FGameplayEventData& EventData);
	
private:
	void ApplyAcidDamage(
		const FGuid& AcidID
	);

	void ExpireAcid(
		const FGuid& AcidID
	);

	void ClearAcidInstances();
};
