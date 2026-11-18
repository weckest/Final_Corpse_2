// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Effects/BurnEffect.h"

#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "Character/PlayerCharacter.h"
#include "Enemies/BaseEnemy.h"
#include "Enemies/EnemyStatsComponent.h"
#include "Engine/Engine.h"

UBurnEffect::~UBurnEffect()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Burning REMOVED")));
	}
}

void UBurnEffect::OnAdded(AActor* Owner, int32 Count)
{
	Super::OnAdded(Owner, Count);
	if (!bIsAdded)
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Owner))
		{
			PlayerCharacter->GamePlayEvent.AddDynamic(this, &UBurnEffect::Burn);
			PlayerCharacter->GamePlayEvent.AddDynamic(this, &UBurnEffect::RemoveEnemy);
			bIsAdded = true;
			
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Burn Added")));
			}
		}
	}
}

void UBurnEffect::OnCountChanged(int32 NewCount)
{
	Super::OnCountChanged(NewCount);
}

void UBurnEffect::OnRemoved(AActor* Owner)
{
	Super::OnRemoved(Owner);
}

void UBurnEffect::Burn(const FGameplayEventData& GamePlayEventData)
{
	if (GamePlayEventData.EventTag != FGameplayTag::RequestGameplayTag(FName("Event.Enemy.Hit"))) return;
	
	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(GamePlayEventData.Actor))
	{
		if (UEnemyStatsComponent* EnemyStats = Enemy->GetComponentByClass<UEnemyStatsComponent>())
		{
			if (!EnemyStats->bIsBurning)
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Applying Burning")));
				}
				FTimerHandle& TimerHandle = BurnTimerHandle.FindOrAdd(Enemy);
				FTimerHandle& IntervalTimerHandle = BurnIntervalTimerHandle.FindOrAdd(Enemy);

				Enemy->GetWorldTimerManager().SetTimer(TimerHandle, [this, Enemy, EnemyStats, &IntervalTimerHandle]()
				{
					Enemy->GetWorldTimerManager().ClearTimer(IntervalTimerHandle);
					EnemyStats->bIsBurning = false;
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Burning %d"), EnemyStats->bIsBurning));
					}
				}, BurnLength * EffectCount, false);
		
				Enemy->GetWorldTimerManager().SetTimer(IntervalTimerHandle, [this, EnemyStats, GamePlayEventData, Enemy]()
				{
					EnemyStats->bIsBurning = true;
					if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GamePlayEventData.Target))
					{
						float Damage = BurnDamage * PlayerCharacter->GetComponentByClass<UPlayerStats>()->GetMaxStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Damage")));
						if (GEngine)
						{
							GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Burning %f"), Damage));
						}
						PlayerCharacter->GamePlayEvent.Broadcast({FGameplayTag::RequestGameplayTag(FName("Event.Enemy.DamageRequest")), PlayerCharacter, Enemy, Damage});
					}
				}, BurnInterval, true);
				
				if (BurnEffect)
				{
					UNiagaraFunctionLibrary::SpawnSystemAttached(
						BurnEffect,
						Enemy->GetRootComponent(),
						NAME_None,
						FVector::ZeroVector,
						FRotator::ZeroRotator,
						EAttachLocation::KeepRelativeOffset,
						true
					);
				}
			}
		}
	}
}

void UBurnEffect::RemoveEnemy(const FGameplayEventData& GamePlayEventData)
{
	Super::RemoveEnemy(GamePlayEventData);
	if (GamePlayEventData.EventTag != FGameplayTag::RequestGameplayTag(FName("Event.Enemy.Killed"))) return;
	
	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(GamePlayEventData.Target))
	{
		FTimerHandle* TimerHandle = BurnTimerHandle.Find(Enemy);
		FTimerHandle* IntervalTimerHandle = BurnIntervalTimerHandle.Find(Enemy);
		
		if (TimerHandle && IntervalTimerHandle)
		{
			Enemy->GetWorldTimerManager().ClearTimer(*TimerHandle);
			BurnTimerHandle.FindAndRemoveChecked(Enemy);
			
			Enemy->GetWorldTimerManager().ClearTimer(*IntervalTimerHandle);
			BurnIntervalTimerHandle.FindAndRemoveChecked(Enemy);
		}
		
		if (UEnemyStatsComponent* EnemyStats = Enemy->GetComponentByClass<UEnemyStatsComponent>())
		{
			if (EnemyStats->bIsBurning)
			{
				EnemyStats->bIsBurning = false;
			}
		}
	}
}
