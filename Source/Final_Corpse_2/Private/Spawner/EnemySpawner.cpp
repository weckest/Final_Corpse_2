// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/EnemySpawner.h"

#include "Components/BillboardComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"


namespace
{
	// An entry is usable if it has a class set and a non-zero weight.
	bool IsEntryValid(const FEnemySpawnEntry& Entry)
	{
		return Entry.EnemyClass != nullptr && Entry.SelectionWeight > 0.0f;
	}

	// A tier can be picked only if its chance is above zero and it has at least one usable entry,
	// so an empty tier never "eats" a spawn wave.
	bool CanTierSpawn(const FEnemyTierConfig& Tier)
	{
		if (Tier.SpawnChance <= 0.0f)
		{
			return false;
		}

		for (const FEnemySpawnEntry& Entry : Tier.EnemyEntries)
		{
			if (IsEntryValid(Entry))
			{
				return true;
			}
		}
		return false;
	}
}


AEnemySpawner::AEnemySpawner()
{
	// Spawning is driven by a timer, so the spawner never needs to tick.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

#if WITH_EDITORONLY_DATA
	// Editor-only icon so the spawner is visible in the viewport; it doesn't exist in packaged builds.
	Billboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	if (Billboard)
	{
		Billboard->SetupAttachment(Root);
	}
#endif
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (bAutoStart)
	{
		StartSpawning();
	}
}

void AEnemySpawner::StartSpawning()
{
	// Random first delay de-syncs spawners that all start on the same frame.
	const float FirstDelay = bRandomizeFirstSpawn
		? FMath::FRandRange(0.0f, SpawnInterval)
		: GetNextInterval();

	ScheduleNextSpawn(FirstDelay);
}

void AEnemySpawner::HandleSpawnTimer()
{
	SpawnEnemy();
	ScheduleNextSpawn(GetNextInterval());
}

void AEnemySpawner::ScheduleNextSpawn(float Delay)
{
	if (UWorld* World = GetWorld())
	{
		// One-shot timer re-armed after every wave, so each wave can get its own random interval.
		// SetTimer requires a delay > 0, so clamp to a tiny minimum.
		World->GetTimerManager().SetTimer(
			SpawnTimerHandle, this, &AEnemySpawner::HandleSpawnTimer, FMath::Max(Delay, 0.01f), false);
	}
}

float AEnemySpawner::GetNextInterval() const
{
	const float Jitter = FMath::FRandRange(-SpawnIntervalVariance, SpawnIntervalVariance);
	return FMath::Max(SpawnInterval + Jitter, 0.1f);
}

void AEnemySpawner::StopSpawning()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(SpawnTimerHandle);
	}
}

const FEnemyTierConfig* AEnemySpawner::PickTier() const
{
	const FEnemyTierConfig* Tiers[] = { &LowTier, &MidTier, &EliteTier };

	float TotalChance = 0.0f;
	for (const FEnemyTierConfig* Tier : Tiers)
	{
		if (CanTierSpawn(*Tier))
		{
			TotalChance += Tier->SpawnChance;
		}
	}

	if (TotalChance <= 0.0f)
	{
		return nullptr;
	}

	// Roll once, then walk the tiers subtracting each chance until the roll lands inside one.
	float Roll = FMath::FRandRange(0.0f, TotalChance);
	const FEnemyTierConfig* LastValid = nullptr;

	for (const FEnemyTierConfig* Tier : Tiers)
	{
		if (!CanTierSpawn(*Tier))
		{
			continue;
		}

		LastValid = Tier;
		if (Roll < Tier->SpawnChance)
		{
			return Tier;
		}
		Roll -= Tier->SpawnChance;
	}

	// Floating-point safety net: a roll of exactly TotalChance falls through the loop.
	return LastValid;
}

const FEnemySpawnEntry* AEnemySpawner::PickEntry(const FEnemyTierConfig& Tier)
{
	float TotalWeight = 0.0f;
	for (const FEnemySpawnEntry& Entry : Tier.EnemyEntries)
	{
		if (IsEntryValid(Entry))
		{
			TotalWeight += Entry.SelectionWeight;
		}
	}

	if (TotalWeight <= 0.0f)
	{
		return nullptr;
	}

	// Same weighted roll as PickTier, but across the entries in this tier.
	float Roll = FMath::FRandRange(0.0f, TotalWeight);
	const FEnemySpawnEntry* LastValid = nullptr;

	for (const FEnemySpawnEntry& Entry : Tier.EnemyEntries)
	{
		if (!IsEntryValid(Entry))
		{
			continue;
		}

		LastValid = &Entry;
		if (Roll < Entry.SelectionWeight)
		{
			return &Entry;
		}
		Roll -= Entry.SelectionWeight;
	}

	return LastValid;
}

void AEnemySpawner::SpawnEnemy()
{
	const FEnemyTierConfig* Tier = PickTier();
	if (!Tier)
	{
		return;
	}

	const FEnemySpawnEntry* Entry = PickEntry(*Tier);
	if (!Entry)
	{
		return;
	}

	// Guard against Min > Max being typed into the details panel.
	const int32 MinSize = FMath::Min(Entry->MinGroupSize, Entry->MaxGroupSize);
	const int32 MaxSize = FMath::Max(Entry->MinGroupSize, Entry->MaxGroupSize);
	const int32 GroupSize = FMath::RandRange(MinSize, MaxSize);

	const FVector Center = GetActorLocation();
	const FRotator Facing = GetActorRotation();

	for (int32 i = 0; i < GroupSize; ++i)
	{
		// Scatter each member on a flat circle around the spawner so they don't stack.
		const FVector2D Offset = FMath::RandPointInCircle(GroupSpawnRadius);
		const FVector Location = Center + FVector(Offset.X, Offset.Y, 0.0f);

		SpawnEnemyAtTransform(Entry->EnemyClass, FTransform(Facing, Location));
	}
}

ABaseEnemy* AEnemySpawner::SpawnEnemyAtTransform(TSubclassOf<ABaseEnemy> EnemyClass, const FTransform& SpawnTransform)
{
	UWorld* World = GetWorld();
	if (!World || !EnemyClass)
	{
		return nullptr;
	}

	FActorSpawnParameters Params;
	Params.Owner = this;
	// If the spot overlaps something, nudge the enemy to a free spot instead of silently not spawning it.
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	return World->SpawnActor<ABaseEnemy>(EnemyClass, SpawnTransform, Params);
}
