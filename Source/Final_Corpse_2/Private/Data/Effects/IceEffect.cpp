// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Effects/IceEffect.h"

#include "TimerManager.h"
#include "Character/PlayerCharacter.h"
#include "Data/Effects/LifestealEffect.h"
#include "Enemies/BaseEnemy.h"
#include "Enemies/EnemyStatsComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/PawnMovementComponent.h"


void UIceEffect::OnAdded(AActor* Owner, int32 Count)
{
	Super::OnAdded(Owner, Count);
	if (!bIsAdded)
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Owner))
		{
			PlayerCharacter->GamePlayEvent.AddDynamic(this, &UIceEffect::Slow);
			bIsAdded = true;
			
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Ice Added")));
			}
		}
	}
}

void UIceEffect::OnCountChanged(int32 NewCount)
{
	Super::OnCountChanged(NewCount);
}

void UIceEffect::OnRemoved(AActor* Owner)
{
	Super::OnRemoved(Owner);
}

void UIceEffect::Slow(const FGameplayEventData& GamePlayEventData)
{
	if (GamePlayEventData.EventTag != FGameplayTag::RequestGameplayTag(FName("Event.Enemy.Hit"))) return;
	
	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(GamePlayEventData.Actor))
	{
		if (UEnemyStatsComponent* EnemyStats = Enemy->GetComponentByClass<UEnemyStatsComponent>())
		{
			if (UPawnMovementComponent* EnemyMovement = Enemy->GetMovementComponent())
			{
				if (EnemyMovement->Velocity.Length() == EnemyStats->BaseMoveSpeed)
				{
					EnemyMovement->Velocity.Normalize();
					EnemyMovement->Velocity *= EnemyStats->BaseMoveSpeed * 0.5f;
					
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Enemy Slowed")));
					}
					
					Enemy->GetWorldTimerManager().SetTimer(SlowTimerHandle, [this, EnemyStats, EnemyMovement]()
					{
						EnemyMovement->Velocity.Normalize();
						EnemyMovement->Velocity *= EnemyStats->BaseMoveSpeed;
					}, SlowTime, false);
				}
			}
		}
	}
}

void UIceEffect::RemoveEnemy(const FGameplayEventData& GamePlayEventData)
{
	Super::RemoveEnemy(GamePlayEventData);
	if (GamePlayEventData.EventTag != FGameplayTag::RequestGameplayTag(FName("Event.Enemy.Killed"))) return;
	
	
}
