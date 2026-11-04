#pragma once

#include "CoreMinimal.h"
#include "Data/GameplayEventData.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

class APlayerCharacter;
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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void HandleGameplayEvent(const FGameplayEventData& EventData);
	
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void CheckDeath();
	
	UFUNCTION(BlueprintPure, Category= "Enemy")
	bool isDead()const {return bIsDead;};
	
protected:
	virtual void Die();

	
private:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> ListenedPlayer;
	
	bool bIsDead = false;
};