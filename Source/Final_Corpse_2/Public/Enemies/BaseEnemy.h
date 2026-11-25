#pragma once

#include "CoreMinimal.h"
#include "Data/GameplayEventData.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

class APlayerCharacter;
class UEnemyStatsComponent;
class UBehaviorTree;
class UAnimMontage;

UCLASS()
class FINAL_CORPSE_2_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseEnemy();
	
	UFUNCTION(BlueprintPure, Category="Enemy|Spawn")
	bool isSpawning() const { return bIsSpawning; };

protected:
	// spawn stuff
	UPROPERTY(EditDefaultsOnly, Category="Enemy|Spawn")
	TObjectPtr<UAnimMontage> SpawnMontage;
	
	UFUNCTION()
	void OnSpawnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	void EndSpawnAnimation();
	
	UPROPERTY(EditDefaultsOnly, Category="Enemy|Spawn")
	float SpawnAnimheightOffset = 0.0f;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UEnemyStatsComponent> EnemyStats;
	
	UPROPERTY(EditDefaultsOnly, Category="Enemy|Attack")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(EditDefaultsOnly, Category="Enemy|Attack", meta = (ClampMin="0"))
	float HitRangeTolerance = 50.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Enemy|Attack", meta = (ClampMin="0", ClampMax="180"))
	float HitHalfAngle = 60.0f;

	virtual void Die();

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category= "Enemy|Attack")
	virtual void Attack();
	
	UFUNCTION(BlueprintCallable, Category= "Enemy|Attack")
	void ApplyAttackHit();
	
	UFUNCTION(BlueprintPure, Category= "Enemy|Attack")
	bool CanAttack() const;
	
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
	
	FVector DefaultMeshLocation;
	
	bool bIsSpawning = false;
	
	float LastAttackTime = -1000.0f;
	
	UPROPERTY()
	TObjectPtr<APlayerCharacter> ListenedPlayer;
	
	UPROPERTY(EditDefaultsOnly, Category="Ai")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	bool bIsDead = false;
};