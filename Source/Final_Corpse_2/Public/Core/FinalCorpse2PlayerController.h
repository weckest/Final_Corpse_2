// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FinalCorpse2PlayerController.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class FINAL_CORPSE_2_API AFinalCorpse2PlayerController : public APlayerController
{
	GENERATED_BODY()
	
	protected:
	UPROPERTY(EditAnywhere, Category = "Input")
	TArray<UInputMappingContext*> DefaultMappingContexts;
	
	virtual void SetupInputComponent() override;
	
};
