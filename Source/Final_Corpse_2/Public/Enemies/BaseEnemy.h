#pragma once

#include "CoreMinimal.h"
#include "Data/GameplayEventData.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

class APlayerCharacter;
class UEnemyStatsComponent;
class UBehaviorTree;

UCLASS()
class FINAL_CORPSE_2_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEnemyStatsComponent> EnemyStats;
	

	virtual void Die();

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void Attack(){};
	
	UFUNCTION()
	void HandleGameplayEvent(const FGameplayEventData& EventData);
	
	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void CheckDeath();
	
	UFUNCTION(BlueprintPure, Category= "Enemy")
	bool isDead()const {return bIsDead;};
	
	UFUNCTION(BlueprintPure, Category= "Enemy")
	UEnemyStatsComponent* GetEnemyStats() const {return EnemyStats;};
	
	UBehaviorTree* GetBehaviorTree() const {return BehaviorTree;};
	

	
private:
	UPROPERTY()
	TObjectPtr<APlayerCharacter> ListenedPlayer;
	
	UPROPERTY(EditDefaultsOnly, Category="Ai")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	bool bIsDead = false;
};