// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/BaseEnemy.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

/**
 * a struct that holds the configuration for a single enemy spawn option
 * holds the enemy class, the weight for selection, and the min/max group sizes.
 */
USTRUCT(BlueprintType)
struct FEnemySpawnEntry
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawning",
		meta = (ClampMin = "0.0"))
	float SelectionWeight = 1.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<ABaseEnemy> EnemyClass;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawning",
		meta = (ClampMin = "1"))
	int32 MinGroupSize = 1;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawning",
		meta = (ClampMin = "1"))
	int32 MaxGroupSize = 1;
};

/**
 * Struct to hold configs for enemy tier
 *  holds the spawn chance for the tier and a list of enemy options with their weights and group sizes.
 */
USTRUCT(BlueprintType)
struct FEnemyTierConfig
{
	GENERATED_BODY()

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawning",
		meta = (ClampMin = "0.0"))
	float SpawnChance = 0.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawning")
	TArray<FEnemySpawnEntry> EnemyEntries;
};

UCLASS()
class FINAL_CORPSE_2_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	AEnemySpawner();

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawning|Low Tier")
	FEnemyTierConfig LowTier;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawning|Mid Tier")
	FEnemyTierConfig MidTier;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Spawning|Elite Tier")
	FEnemyTierConfig EliteTier;

	UPROPERTY(EditInstanceOnly, Category = "Spawning")
	float SpawnInterval = 5.0f;

	FTimerHandle SpawnTimerHandle;

	virtual void BeginPlay() override;

	UFUNCTION()
	void SpawnEnemy();

	// TODO: In SpawnEnemy:
	// 1. Roll once across the three tier chances.
	// 2. Select one valid enemy option within the chosen tier by weight.
	// 3. Roll a group size between that option's MinGroupSize and MaxGroupSize.
	// 4. Spawn the group with SpawnActor<ABaseEnemy>.
	// 5. Pass a different transform for each group member so they do not overlap.
	//
	// TODO: Add a small GroupSpawnRadius property for the group offsets.
	// TODO: Add a helper such as SpawnEnemyAtTransform(Class, Transform).


public:
	virtual void Tick(float DeltaTime) override;
};
