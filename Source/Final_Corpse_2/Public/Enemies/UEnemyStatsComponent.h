
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UEnemyStatsComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class FINAL_CORPSE_2_API UUEnemyStatsComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UUEnemyStatsComponent();

protected:
	// Base stats configured per enemy type
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyStats")
	float BaseMaxHealth = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyStats")
	float BaseAttack = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyStats")
	float BaseDefense = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyStats")
	float BaseMoveSpeed = 300.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyStats")
	float BaseAttackSpeed = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyStats")
	float BaseAttackRange = 150.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyStats")
	float BaseHpRegenRate = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyStats",
		meta = (ClampMin = "0.01"))
	float MinSize = 0.8f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "EnemyStats",
		meta = (ClampMin = "0.01"))
	float MaxSize = 1.2f;

	// Runtime stats
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "EnemyStats")
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "EnemyStats")
	float CurrentHealth = 100.0f;
};
