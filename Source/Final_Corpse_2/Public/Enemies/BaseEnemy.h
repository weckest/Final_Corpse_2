#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

class UEnemyStatsComponent;

UCLASS()
class FINAL_CORPSE_2_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEnemyStatsComponent> EnemyStats;

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};