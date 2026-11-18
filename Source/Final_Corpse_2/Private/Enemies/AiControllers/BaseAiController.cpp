// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/AiControllers/BaseAiController.h"

#include "TimerManager.h"
#include "Enemies/EnemyStatsComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

const FName ABaseAiController::TargetActor = TEXT("TargetActor");
const FName ABaseAiController::InAttackRange = TEXT("InAttackRange");
const FName ABaseAiController::CanSeeTarget = TEXT("CanSeeTarget");

// Sets default values
ABaseAiController::ABaseAiController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
}

void ABaseAiController::OnPossess(APawn* Pawn)
{
	Super::OnPossess(Pawn);
	
	ControlledEnemy = Cast<ABaseEnemy>(Pawn);
	if (ControlledEnemy && ControlledEnemy->GetBehaviorTree())
	{
		RunBehaviorTree(ControlledEnemy->GetBehaviorTree());
	}
	
}

AActor* ABaseAiController::GetTarget()
{
	if (!CachedTarg)
	{
		CachedTarg = UGameplayStatics::GetPlayerCharacter(this, 0);
	}
	return CachedTarg;
}


bool ABaseAiController::IsTargetInRange(const AActor* Other) const
{
	const UEnemyStatsComponent* Stats = ControlledEnemy ? ControlledEnemy->GetEnemyStats() : nullptr;
	if (!Stats || !Other) return false;
	
	const float Range = Stats->BaseAttackRange;
	return FVector::DistSquared(ControlledEnemy->GetActorLocation(), Other->GetActorLocation()) <= Range * Range;
}


bool ABaseAiController::IsAttackReady() const
{
	const UEnemyStatsComponent* Stats = ControlledEnemy ? ControlledEnemy->GetEnemyStats() : nullptr;
	if (!Stats) return false;
	
	const float Cooldown = 1.0f / FMath::Max(Stats->BaseAttackSpeed, 0.01f);
	return (GetWorld()->GetTimeSeconds() - lastAttackTime) >= Cooldown;
}

void ABaseAiController::MarkAttacked()
{
	lastAttackTime = GetWorld()->GetTimeSeconds();
}




