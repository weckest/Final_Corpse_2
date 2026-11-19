// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Effects/KnockbackEffect.h"

#include "Character/PlayerCharacter.h"
#include "Enemies/BaseEnemy.h"
#include "Engine/Engine.h"

class ABaseEnemy;

void UKnockbackEffect::OnAdded(AActor* Owner, int32 Count)
{
	Super::OnAdded(Owner, Count);
	if (!bIsAdded)
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Owner))
		{
			PlayerCharacter->GamePlayEvent.AddDynamic(this, &UKnockbackEffect::Knockback);
			bIsAdded = true;
			
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Knockback Added")));
			}
		}
	}
}

void UKnockbackEffect::OnCountChanged(int32 NewCount)
{
	Super::OnCountChanged(NewCount);
}

void UKnockbackEffect::OnRemoved(AActor* Owner)
{
	Super::OnRemoved(Owner);
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Owner))
	{
		PlayerCharacter->GamePlayEvent.RemoveDynamic(this, &UKnockbackEffect::Knockback);
	}
}

void UKnockbackEffect::Knockback(const FGameplayEventData& GamePlayEventData)
{
	if (GamePlayEventData.EventTag != FGameplayTag::RequestGameplayTag(FName("Event.Enemy.Damaged"))) return;
	
	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GamePlayEventData.Actor))
	{
		if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(GamePlayEventData.Target))
		{

			// Push away from the player on the horizontal plane, independent of
			// the enemy's current facing or AI movement direction.
			FVector Direction = Enemy->GetActorLocation() - PlayerCharacter->GetActorLocation();
			Direction = Direction.GetSafeNormal();
			Direction.Z += 0.5f;
			Enemy->LaunchCharacter(Direction * KnockbackForce * EffectCount, true, true);
		}
	}
}
