// Fill out your copyright notice in the Description page of Project Settings.


#include "Final_Corpse_2/Public/Enemies/BaseEnemy.h"
#include "./Enemies/BaseEnemy.h"
#include "./Enemies/UEnemyStatsComponent.h"



// Sets default valu
ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	EnemyStats = CreateDefaultSubobject<UUEnemyStatsComponent>(
		TEXT("EnemyStats"));
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

