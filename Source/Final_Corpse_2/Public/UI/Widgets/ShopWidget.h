// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/ItemDataAsset.h"
#include "ShopWidget.generated.h"

class AShop;
/**
 * 
 */
UCLASS()
class FINAL_CORPSE_2_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	UFUNCTION(BlueprintImplementableEvent, Category = "Shop")
	void setupShop(const TArray<UItemDataAsset*>& Items);
	
	UPROPERTY(BlueprintReadOnly, Category = "Shop")
	TObjectPtr<AShop> OwningShop;
};
