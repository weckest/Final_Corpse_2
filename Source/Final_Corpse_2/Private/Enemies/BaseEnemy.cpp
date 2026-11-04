// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BaseEnemy.h"

#include "Character/PlayerCharacter.h"
#include "Enemies/EnemyStatsComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default valu
ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	EnemyStats = CreateDefaultSubobject<UEnemyStatsComponent>(
		TEXT("EnemyStats"));
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (EnemyStats)
	{
		EnemyStats->MaxHealth = EnemyStats->BaseMaxHealth;
		EnemyStats->CurrentHealth = EnemyStats->MaxHealth;
	}
	
	ListenedPlayer = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this,0));
	if (ListenedPlayer)
	{
		ListenedPlayer->GamePlayEvent.AddDynamic(this, &ABaseEnemy::HandleGameplayEvent);
	}

}

void ABaseEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	
	if (ListenedPlayer)
	{
		ListenedPlayer->GamePlayEvent.RemoveDynamic(this, &ABaseEnemy::HandleGameplayEvent);
	}
	Super::EndPlay(EndPlayReason);
}


// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseEnemy::HandleGameplayEvent(const FGameplayEventData& EventData)
{
	static const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("Event.Enemy.DamageRequest"));
	if (EventData.EventTag != DamageTag  || EventData.Target != this) return;
	
	if (bIsDead || !EnemyStats) return;
	
	// brackets for future damage calculations
	float damageDealt = (EventData.Magnitude);
	
	EnemyStats->CurrentHealth -= damageDealt;
	
	ListenedPlayer->GamePlayEvent.Broadcast({FGameplayTag::RequestGameplayTag(FName("Event.Enemy.Damaged")), 
		ListenedPlayer, this, damageDealt});
	
	CheckDeath();
}

void ABaseEnemy::CheckDeath()
{
	if (bIsDead || !EnemyStats ) return;
	
	if (EnemyStats->CurrentHealth <= 0.0f)
	{
		Die();
	}
}

void ABaseEnemy::Die()
{
	bIsDead = true;
	
	if (ListenedPlayer)
	{
		ListenedPlayer->GamePlayEvent.Broadcast({FGameplayTag::RequestGameplayTag(FName("Event.Enemy.Killed")), ListenedPlayer, this, 0.0f});
	}
	
	Destroy();
}

