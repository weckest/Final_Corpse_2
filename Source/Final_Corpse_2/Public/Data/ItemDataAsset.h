// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

class UItemEffect;
struct FStatModifier;
class UPlayerStats;
class UTexture2D;



UCLASS()
class FINAL_CORPSE_2_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FName ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	FText Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TArray<FStatModifier> StatModifiers;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	TArray<TSubclassOf<UItemEffect>> Effects;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item")
	float Cost = 100.0f;
};


