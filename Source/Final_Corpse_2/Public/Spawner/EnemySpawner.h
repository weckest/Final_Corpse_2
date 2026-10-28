// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/BaseEnemy.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

class UBillboardComponent;

/**
 * One enemy option the spawner can pick: which class to spawn, how likely it is
 * to be picked relative to the other options in the same tier, and how many to spawn at once.
 */
USTRUCT(BlueprintType)
struct FEnemySpawnEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TSubclassOf<ABaseEnemy> EnemyClass;

	// Relative weight within the tier. Two entries at 1 and 1 = 50/50; 3 and 1 = 75/25.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (ClampMin = "0.0"))
	float SelectionWeight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (ClampMin = "1"))
	int32 MinGroupSize = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (ClampMin = "1"))
	int32 MaxGroupSize = 1;
};

/**
 * Everything about one tier: how likely this tier is to be picked on each spawn,
 * and the list of enemy options inside it.
 */
USTRUCT(BlueprintType)
struct FEnemyTierConfig
{
	GENERATED_BODY()

	// Relative chance versus the other tiers. Low 70, Mid 25, Elite 5 = 70% / 25% / 5%.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (ClampMin = "0.0"))
	float SpawnChance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	TArray<FEnemySpawnEntry> EnemyEntries;
};

UCLASS()
class FINAL_CORPSE_2_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	AEnemySpawner();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> Root;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBillboardComponent> Billboard;
#endif

	// The spawn pool. EditDefaultsOnly: set it once in BP_EnemySpawner and every placed spawner
	// shares it. It can't be overridden per spawner in the level, so they can't drift out of sync.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning|Low Tier")
	FEnemyTierConfig LowTier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning|Mid Tier")
	FEnemyTierConfig MidTier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawning|Elite Tier")
	FEnemyTierConfig EliteTier;

	// Average seconds between spawn waves.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (ClampMin = "0.1"))
	float SpawnInterval = 5.0f;

	// Each wave's delay is randomized by up to this many seconds either side of SpawnInterval.
	// 0 = perfectly regular. e.g. Interval 5, Variance 1.5 -> each wave comes 3.5 to 6.5 s after the last.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (ClampMin = "0.0"))
	float SpawnIntervalVariance = 1.0f;

	// Wait a random 0..SpawnInterval before the first wave, so spawners that start together don't fire together.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	bool bRandomizeFirstSpawn = true;

	// Group members are scattered randomly within this radius (cm) around the spawner.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (ClampMin = "0.0"))
	float GroupSpawnRadius = 300.0f;

	// Start spawning automatically on BeginPlay. Turn off to drive it with StartSpawning/StopSpawning.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawning")
	bool bAutoStart = true;

	FTimerHandle SpawnTimerHandle;

	virtual void BeginPlay() override;

	// Picks a tier, then an enemy in that tier, then spawns a group of it.
	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void SpawnEnemy();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void StartSpawning();

	UFUNCTION(BlueprintCallable, Category = "Spawning")
	void StopSpawning();

	// Called by the timer: spawns one wave, then schedules the next.
	void HandleSpawnTimer();

	// Sets a one-shot timer for the next wave after Delay seconds.
	void ScheduleNextSpawn(float Delay);

	// SpawnInterval with SpawnIntervalVariance applied.
	float GetNextInterval() const;

	// Returns the tier picked by SpawnChance, or nullptr if no tier can spawn anything.
	const FEnemyTierConfig* PickTier() const;

	// Returns an entry picked by SelectionWeight, or nullptr if the tier has no valid entries.
	static const FEnemySpawnEntry* PickEntry(const FEnemyTierConfig& Tier);

	ABaseEnemy* SpawnEnemyAtTransform(TSubclassOf<ABaseEnemy> EnemyClass, const FTransform& SpawnTransform);
};
