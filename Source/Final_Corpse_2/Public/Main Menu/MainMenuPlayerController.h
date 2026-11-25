// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainMenuPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FINAL_CORPSE_2_API AMainMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	
private:
	UPROPERTY()
	UUserWidget* MainMenuWidget;
};
