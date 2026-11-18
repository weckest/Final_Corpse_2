// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Effects/BurnEffect.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
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
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Owner))
	{
		PlayerCharacter->GamePlayEvent.RemoveDynamic(this, &UBurnEffect::Burn);
		PlayerCharacter->GamePlayEvent.RemoveDynamic(this, &UBurnEffect::RemoveEnemy);
	}
	Super::OnRemoved(Owner);
}

void UBurnEffect::Burn(const FGameplayEventData& GamePlayEventData)
{
	if (GamePlayEventData.EventTag != FGameplayTag::RequestGameplayTag(FName("Event.Enemy.Hit"))) return;
	
	if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(GamePlayEventData.Target))
	{
		if (UEnemyStatsComponent* EnemyStats = Enemy->GetComponentByClass<UEnemyStatsComponent>())
		{
			FTimerHandle& TimerHandle = BurnTimerHandle.FindOrAdd(Enemy);
			FTimerHandle& IntervalTimerHandle = BurnIntervalTimerHandle.FindOrAdd(Enemy);
			
			if (APlayerCharacter* Player = Cast<APlayerCharacter>(GamePlayEventData.Actor))
			{
				//LIGHT THEM ON FIRE BABY!!!!
				Enemy->GetWorldTimerManager().SetTimer(TimerHandle, [this, Enemy, EnemyStats, &IntervalTimerHandle]()
				{
					Enemy->GetWorldTimerManager().ClearTimer(IntervalTimerHandle);
					EnemyStats->bIsBurning = false;
					StopBurningVfx(Enemy);
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Burning %d"), EnemyStats->bIsBurning));
					}
				}, BurnLength * EffectCount, false);
	
				Enemy->GetWorldTimerManager().SetTimer(IntervalTimerHandle, [this, EnemyStats, GamePlayEventData, Enemy]()
				{
					if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GamePlayEventData.Target))
					{
						float Damage = BurnDamage * PlayerCharacter->GetComponentByClass<UPlayerStats>()->GetMaxStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Damage")));
						if (GEngine)
						{
							GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Burning %f"), Damage));
						}
						PlayerCharacter->GamePlayEvent.Broadcast({FGameplayTag::RequestGameplayTag(FName("Event.Enemy.DamageRequest")), this, Enemy, Damage});
					}
				}, BurnInterval, true);
			
				if (BurnEffect && !EnemyStats->bIsBurning)
				{
					EnemyStats->bIsBurning = true;
					if (UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
						BurnEffect,
						Enemy->GetRootComponent(),
						NAME_None,
						FVector::ZeroVector,
						FRotator::ZeroRotator,
						EAttachLocation::KeepRelativeOffset,
						true
					))
					{
						BurnNiagaraComponents.Add(Enemy, NiagaraComponent);
					}
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
		StopBurningVfx(Enemy);
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

void UBurnEffect::StopBurningVfx(ABaseEnemy* Enemy)
{
	if (TObjectPtr<UNiagaraComponent>* NiagaraComponent = BurnNiagaraComponents.Find(Enemy))
	{
		if (IsValid(NiagaraComponent->Get()))
		{
			// Stop spawning new particles, but let the particles already in the
			// system finish their normal lifetimes before the component is removed.
			NiagaraComponent->Get()->SetAutoDestroy(true);
			NiagaraComponent->Get()->Deactivate();
		}
		BurnNiagaraComponents.Remove(Enemy);
	}
}
