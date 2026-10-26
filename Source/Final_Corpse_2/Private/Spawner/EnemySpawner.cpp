// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/EnemySpawner.h"

#include "TimerManager.h"
#include "Engine/World.h"


// Sets default values
AEnemySpawner::AEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off in the editor if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerDelegate SpawnDelegate;
	SpawnDelegate.BindUFunction(this, FName("SpawnEnemy"));
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, SpawnDelegate, SpawnInterval, true);
	
}

void AEnemySpawner::SpawnEnemy()
{
	if (!GetWorld())
	{
		return;
	}
	
	const float TotalChance =
		LowTier.SpawnChance +
		MidTier.SpawnChance +
		EliteTier.SpawnChance;

	if (TotalChance <= 0.0f)
	{
		return;
	}

	const float Roll = FMath::FRandRange(0.0f, TotalChance);

	const FEnemyTierConfig* SelectedTier = nullptr;

	if (Roll < LowTier.SpawnChance)
	{
		SelectedTier = &LowTier;
	}
	else if (Roll < LowTier.SpawnChance + MidTier.SpawnChance)
	{
		SelectedTier = &MidTier;
	}
	else
	{
		SelectedTier = &EliteTier;
	}

	// Now that we have the tier, flip a coin to select one of its two entries.
	if (SelectedTier && SelectedTier->EnemyEntries.Num() >= 2)
	{
		const int32 CoinFlip = FMath::RandRange(0, 1);
		const FEnemySpawnEntry& Entry =
			SelectedTier->EnemyEntries[CoinFlip];

	
		if (Entry.EnemyClass)
		{
			int32 GroupSize = FMath::RandRange(
				Entry.MinGroupSize,
				Entry.MaxGroupSize);

			for (int32 i = 0; i < GroupSize; ++i)
			{
				FVector SpawnLocation = GetActorLocation();
				FRotator SpawnRotation = GetActorRotation();
				GetWorld()->SpawnActor<ABaseEnemy>(
					Entry.EnemyClass,
					SpawnLocation,
					SpawnRotation);
			}
		}
	}
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
