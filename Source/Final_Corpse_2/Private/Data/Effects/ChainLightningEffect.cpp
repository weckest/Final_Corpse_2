// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Effects/ChainLightningEffect.h"

#include "DrawDebugHelpers.h"
#include "Character/PlayerCharacter.h"
#include "Enemies/BaseEnemy.h"
#include "Engine/Engine.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void UChainLightningEffect::OnAdded(AActor* Owner, int32 Count)
{
	Super::OnAdded(Owner, Count);
	if (!bIsAdded)
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Owner))
		{
			PlayerCharacter->GamePlayEvent.AddDynamic(this, &UChainLightningEffect::Chain);
			bIsAdded = true;
			
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Chain Lightning Added")));
			}
		}
	}
}

void UChainLightningEffect::OnCountChanged(int32 NewCount)
{
	Super::OnCountChanged(NewCount);
}

void UChainLightningEffect::OnRemoved(AActor* Owner)
{
	Super::OnRemoved(Owner);
}

void UChainLightningEffect::Chain(const FGameplayEventData& EventData)
{
	if (EventData.EventTag != FGameplayTag::RequestGameplayTag(FName("Event.Enemy.Hit")) && EventData.Actor.GetClass() != Owner.GetClass()) return;
	
	
	
	if (ABaseEnemy* TargetEnemy = Cast<ABaseEnemy>(EventData.Target))
	{
		FTransform Transform = TargetEnemy->GetActorTransform();
		
		float SearchRange = Range * FMath::Clamp(EffectCount - 1, 1, EffectCount);
		
		TArray<AActor*> Enemies;
		TArray<ABaseEnemy*> EnemiesInRange;
		UGameplayStatics::GetAllActorsOfClass(TargetEnemy->GetWorld(), ABaseEnemy::StaticClass(), Enemies);
		
		for (AActor* EnemyActor : Enemies)
		{
			if (IsValid(EnemyActor))
			{
				if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(EnemyActor))
				{
					if (Enemy == TargetEnemy) continue;
					FTransform EnemyTransform = Enemy->GetTransform();
					
					float Distance = FVector::Dist(Transform.GetLocation(), EnemyTransform.GetLocation());
					if (Distance < SearchRange)
					{
						EnemiesInRange.Add(Enemy);
					}
				}
			}
		}
		
		if (EnemiesInRange.Num() == 0) return;
		for (int i = 0; i < EffectCount; i++)
		{
			//randomly select an enemy to chain the lightning to
			int TheChosenOne = FMath::RandRange(0, EnemiesInRange.Num() - 1);
			
			ABaseEnemy* ChosenEnemy = EnemiesInRange[TheChosenOne];
			
			FTransform ChosenTransform = EnemiesInRange[TheChosenOne]->GetTransform();
			
			DrawDebugLine(
				TargetEnemy->GetWorld(),
				Transform.GetLocation(),
				ChosenTransform.GetLocation(),
				FColor::Blue,
				false,
				5.0f,
				0,
				3.0f
			);
			
			if (APlayerCharacter* Player = Cast<APlayerCharacter>(EventData.Actor))
			{
				float ChainedDamage = EventData.Magnitude * ChainDamagePercentage;
				Player->GamePlayEvent.Broadcast({FGameplayTag::RequestGameplayTag(FName("Event.Enemy.DamageRequest")), this, ChosenEnemy, ChainedDamage});
			}
		}
	}
}
