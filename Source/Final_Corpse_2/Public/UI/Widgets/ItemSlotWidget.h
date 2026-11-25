// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlotWidget.generated.h"

class UTextBlock;
class UImage;
class UItemDataAsset;
/**
 * 
 */
UCLASS()
class FINAL_CORPSE_2_API UItemSlotWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetItem(UItemDataAsset* Item, int32 count);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Icon;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Count;
};

