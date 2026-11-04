// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Data/StatModifier.h"
#include "PlayerStats.generated.h"


class UInventoryComponent;

UENUM(BlueprintType)
enum class EEditType : uint8
{
	Add,
	Subtract,
	Multiply,
	Divide
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatsUpdated, FGameplayTag, StatTag);
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
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats")
	TMap<FGameplayTag, float> MaxStats{};
	
	UPROPERTY(BlueprintAssignable, Category = "PlayerStats")
	FOnPlayerStatsUpdated OnPlayerStatsUpdated;
	
	UFUNCTION(BlueprintCallable, Category = "Stats")
	float& GetBaseStatValue(FGameplayTag StatTag);
	
	UFUNCTION(BlueprintCallable, Category = "Stats")
	float& GetCurrentStatValue(FGameplayTag StatTag);
	
	UFUNCTION(BlueprintCallable, Category = "Stats")
	float& GetMaxStatValue(FGameplayTag StatTag);
	
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpdateMaxStatValue(FGameplayTag StatTag, float Amount, EEditType Type);
	
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpdateCurrentStatValue(FGameplayTag StatTag, float Amount, EEditType Type);
	
	UFUNCTION(BlueprintCallable, Category = "Stats")
	void UpdateCurrentStats(UInventoryComponent* InventoryComponent);
	
	UFUNCTION()
	void StatChanged(FGameplayTag StatTag);
	
};
