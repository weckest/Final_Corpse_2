// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Data/StatModifier.h"
#include "PlayerStats.generated.h"


class UInventoryComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FINAL_CORPSE_2_API UPlayerStats : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerStats();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	TMap<FGameplayTag, float> BaseStats{};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	TMap<FGameplayTag, float> CurrentStats{};
	
	UFUNCTION(BlueprintCallable, Category = "Stats")
	float& GetStatValue(FGameplayTag StatTag);
	
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpdateCurrentStats(UInventoryComponent* InventoryComponent);
	
};
