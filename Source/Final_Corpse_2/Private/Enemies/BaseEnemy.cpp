// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/BaseEnemy.h"

#include "Character/PlayerCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "EdGraph/EdGraphNode.h"
#include "Enemies/EnemyStatsComponent.h"
#include "Enemies/AiControllers/BaseAiController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "opensubdiv/far/error.h"


// Sets default valu
ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = false;

	EnemyStats = CreateDefaultSubobject<UEnemyStatsComponent>(
		TEXT("EnemyStats"));
	
	AIControllerClass = ABaseAiController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	
	GetCharacterMovement()->DefaultLandMovementMode = MOVE_NavWalking;
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::OnlyTickPoseWhenRendered;
	GetMesh()->bEnableUpdateRateOptimizations = true;
	GetMesh()->SetGenerateOverlapEvents(false);
	
	bUseControllerRotationYaw = false;
	UCharacterMovementComponent* Move = GetCharacterMovement();
	
	Move->bOrientRotationToMovement = true;
	Move->bUseControllerDesiredRotation = true;
	Move->RotationRate = FRotator(0.f, 540.f, 0.f);
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

void ABaseEnemy::Attack()
{
	if (!CanAttack()) return;
	
	LastAttackTime = GetWorld()->GetTimeSeconds();
	
	if (AttackMontage)
	{
		const float MontageDuration = PlayAnimMontage(AttackMontage);
		UE_LOG(LogTemp, Warning, TEXT("Montage Duration: %f"), MontageDuration);
		PlayAnimMontage(AttackMontage);
	} else
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackMontage is not set for %s"), *GetName());
		ApplyAttackHit();
	}
}

void ABaseEnemy::ApplyAttackHit()
{
	if (bIsDead || !EnemyStats) return;
	
	if (!ListenedPlayer)
	{
		ListenedPlayer = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this,0));
		if (!ListenedPlayer) return;
	}
	
	const FVector ToPlayer = ListenedPlayer->GetActorLocation() - GetActorLocation();
	const float Reach = EnemyStats->BaseAttackRange + HitRangeTolerance;
	if (ToPlayer.SizeSquared() > Reach * Reach) return;
	
	const float MinDot = FMath::Cos(FMath::DegreesToRadians(HitHalfAngle));
	if (FVector::DotProduct(GetActorForwardVector().GetSafeNormal2D(), ToPlayer.GetSafeNormal2D()) < MinDot) return;
	
	static const FGameplayTag DamageRequestTag = FGameplayTag::RequestGameplayTag(FName("Event.Player.DamageRequest"));
	ListenedPlayer->GamePlayEvent.Broadcast({
	DamageRequestTag,
	this,
	ListenedPlayer,
	EnemyStats->BaseAttack});
}

bool ABaseEnemy::CanAttack() const
{
	if (bIsDead || !EnemyStats) return false;
	
	const float Cooldown = 1.0f / FMath::Max(EnemyStats->BaseAttackSpeed, 0.01f);
	return GetWorld()->GetTimeSeconds() - LastAttackTime >= Cooldown;
}

void ABaseEnemy::HandleGameplayEvent(const FGameplayEventData& EventData)
{
	static const FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag(FName("Event.Enemy.DamageRequest"));
	if (EventData.EventTag != DamageTag  || EventData.Target != this) return;
	
	if (bIsDead || !EnemyStats) return;
	
	ListenedPlayer->GamePlayEvent.Broadcast({FGameplayTag::RequestGameplayTag(FName("Event.Enemy.Hit")), 
		EventData.Actor, this, 0.0f});
	
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

