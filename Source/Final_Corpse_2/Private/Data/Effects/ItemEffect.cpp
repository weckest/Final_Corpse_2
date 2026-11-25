// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Effects/ItemEffect.h"

#include "DrawDebugHelpers.h"
#include "Enemies/BaseEnemy.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

void UItemEffect::OnAdded(AActor* Player, int32 Count)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("Adding base Item")));
	}
	this->EffectCount = Count;
}

void UItemEffect::OnCountChanged(int32 NewCount)
{
	this->EffectCount = NewCount;
}

void UItemEffect::OnRemoved(AActor* Player)
{
	bIsAdded = false;
	EffectCount = 0;
}

bool UItemEffect::GetEnemiesInRange(FTransform TargetTransform, float Range, TArray<ABaseEnemy*>& EnemiesInRange)
{		
	float SearchRange = Range * FMath::Clamp(EffectCount - 1, 1, EffectCount);
		
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(Owner->GetWorld(), ABaseEnemy::StaticClass(), Enemies);
		
	for (AActor* EnemyActor : Enemies)
	{
		if (IsValid(EnemyActor))
		{
			if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(EnemyActor))
			{
				FTransform EnemyTransform = Enemy->GetTransform();
					
				float Distance = FVector::Dist(TargetTransform.GetLocation(), EnemyTransform.GetLocation());
				if (Distance <= 0.0f) continue;
				if (Distance < SearchRange)
				{
					EnemiesInRange.Add(Enemy);
				}
			}
		}
	}
		
	if (EnemiesInRange.Num() == 0) return false;
	return true;
}
