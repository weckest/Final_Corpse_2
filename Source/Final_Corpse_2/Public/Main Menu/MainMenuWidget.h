// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UWidgetSwitcher;
class UButton;
/**
 * 
 */
UCLASS()
class FINAL_CORPSE_2_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	UWidget* MainMenuPanel;
	
	UPROPERTY(meta = (BindWidget))
	UWidget* SettingsPanel;
	
	UPROPERTY(meta = (BindWidget))
	UWidget* CreditsPanel;
	
	UPROPERTY(meta = (BindWidget))
	UButton* PlayButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* SettingsButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* CreditsButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* SettingsBackButton;
	
	UPROPERTY(meta = (BindWidget))
	UButton* CreditsBackButton;
	
	
	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	FName GameplayLevelName = FName("CityMap");
	
private:
	UFUNCTION()
	void OnPlayClicked();
	
	UFUNCTION()
	void OnSettingsClicked();
	
	UFUNCTION()
	void OnCreditsClicked();
	
	UFUNCTION()
	void OnBackClicked();
	
	UFUNCTION()
	void OnQuitClicked();
	
};
