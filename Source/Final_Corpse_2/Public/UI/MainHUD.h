// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"

struct FGameplayEventData;
class UPlayerStats;
class UInventoryComponent;
/**
 * 
 */
UCLASS()
class FINAL_CORPSE_2_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;
	
	UPROPERTY()
	TObjectPtr<UPlayerStats> PlayerStats;
	
	UFUNCTION()
	void UpdateStaminaBar(const FGameplayEventData& GameplayEventData);
};
