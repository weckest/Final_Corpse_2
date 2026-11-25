// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"

class UHorizontalBox;
class UItemSlotWidget;
class UTextBlock;
class UProgressBar;
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
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> HealthText;
	
	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UHorizontalBox> Itembar;
	
	UPROPERTY(EditDefaultsOnly, Category="HUD")
	TSubclassOf<UItemSlotWidget> ItemSlotClass;
	
private:
	UPROPERTY()
	TObjectPtr<UInventoryComponent> InventoryComponent;
	
	UPROPERTY()
	TObjectPtr<UPlayerStats> PlayerStats;
	
	UFUNCTION()
	void UpdateStaminaBar(const FGameplayEventData& GameplayEventData);
	
	UFUNCTION()
	void UpdatePrimaryAbility(const FGameplayEventData& GameplayEventData);
	
	UFUNCTION()
	void UpdateSecondaryAbility(const FGameplayEventData& GameplayEventData);
	
	UFUNCTION()
	void HandleHealthEvent(const FGameplayEventData& GameplayEventData);
	
	void RefreshHealth();
	
	UFUNCTION()
	void RefreshItemBar();
};
