// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/BaseEnemy.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "BaseAiController.generated.h"

UCLASS()
class FINAL_CORPSE_2_API ABaseAiController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseAiController();
	
	//blackboard stuff
	static const FName TargetActor;
	static const FName InAttackRange;
	static const FName CanSeeTarget;
	
	ABaseEnemy* GetControlledEnemy() const {return ControlledEnemy;};
	
	AActor* GetTarget();
	

	// helpers
	bool IsTargetInRange(const AActor* Other) const;
	bool IsAttackReady() const;
	void MarkAttacked();
	
protected:
	virtual void OnPossess(APawn* Pawn) override;
	
	UPROPERTY()
	TObjectPtr<ABaseEnemy> ControlledEnemy;
	
	UPROPERTY()
	TObjectPtr<AActor> CachedTarg;

private:
	
	FTimerHandle ThinkTimer;
	float lastAttackTime = -1000.0f;
	
};
