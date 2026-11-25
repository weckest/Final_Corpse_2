// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class FINAL_CORPSE_2_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget))
	UButton* GO_Button;
	
	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	FName MainMenuLevelName = FName("MainMenu");
	
private:
	UFUNCTION()
	void OnButtonClicked();
	
};
