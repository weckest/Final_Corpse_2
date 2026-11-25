// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/Effects/AcidEffect.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "Character/PlayerCharacter.h"
#include "Enemies/BaseEnemy.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"

void UAcidEffect::OnAdded(AActor* Owner, int32 Count)
{
	Super::OnAdded(Owner, Count);
	if (!bIsAdded)
	{
		if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Owner))
		{
			PlayerCharacter->GamePlayEvent.AddDynamic(this, &UAcidEffect::SpawnAcid);
			bIsAdded = true;
			
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Acid Added")));
			}
		}
	}
}

void UAcidEffect::OnCountChanged(int32 NewCount)
{
	Super::OnCountChanged(NewCount);
}

void UAcidEffect::OnRemoved(AActor* Owner)
{
	Super::OnRemoved(Owner);

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(Owner))
	{
		Player->GamePlayEvent.RemoveDynamic(
			this,
			&UAcidEffect::SpawnAcid
		);
	}

	bIsAdded = false;

	ClearAcidInstances();
}

void UAcidEffect::SpawnAcid(const FGameplayEventData& EventData)
{
    // Only accept hits originating from the owner.
	if (EventData.EventTag != FGameplayTag::RequestGameplayTag(FName("Event.Enemy.Hit")) && EventData.Actor.Get() != Owner) return;

    ABaseEnemy* Enemy = Cast<ABaseEnemy>(EventData.Target);
    if (!IsValid(Enemy)) return;

    APlayerCharacter* SourcePlayer = Cast<APlayerCharacter>(EventData.Actor);

    if (!IsValid(SourcePlayer)) return;

    UWorld* World = Enemy->GetWorld();
    if (!World || !AcidEffect) return;

    FVector SpawnLocation = Enemy->GetActorLocation();
	
	SpawnLocation.Z = -8200.0f;

    UNiagaraComponent* AcidComponent =
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            World,
            AcidEffect,
            SpawnLocation,
            FRotator::ZeroRotator,
            FVector(1.0f),
            true,
            true,
            ENCPoolMethod::None
        );
	
	AcidComponent->SetVariableFloat(FName("User.Cylinder Radius002"), AcidRange * 1000.0f);

    if (!AcidComponent) return;

    // Create a unique instance for this puddle.
    const FGuid AcidID = FGuid::NewGuid();

    FAcidInstance Instance;
    Instance.NiagaraComponent = AcidComponent;
    Instance.SourcePlayer = SourcePlayer;
    Instance.Location = SpawnLocation;

    AcidInstances.Add(AcidID, Instance);

    // Start repeating damage timer.
    FTimerDelegate DamageDelegate =
        FTimerDelegate::CreateWeakLambda(
            this,
            [this, AcidID]()
            {
                ApplyAcidDamage(AcidID);
            }
        );

    World->GetTimerManager().SetTimer(
        AcidInstances[AcidID].DamageTimerHandle,
        DamageDelegate,
        AcidInterval,
        true
    );

    // Start expiration timer.
    FTimerDelegate ExpirationDelegate =
        FTimerDelegate::CreateWeakLambda(
            this,
            [this, AcidID]()
            {
                ExpireAcid(AcidID);
            }
        );

    World->GetTimerManager().SetTimer(
        AcidInstances[AcidID].ExpirationTimerHandle,
        ExpirationDelegate,
        AcidLength,
        false
    );
}

void UAcidEffect::ApplyAcidDamage(
	FVector AcidLocation,
	float SearchRange,
	const FGameplayEventData& EventData)
{
	TArray<ABaseEnemy*> Enemies;

	if (!GetEnemiesInRange(FTransform(AcidLocation), SearchRange, Enemies))
		return;

	for (ABaseEnemy* Enemy : Enemies)
	{
		if (!IsValid(Enemy)) continue;

		float Damage = AcidDamage;

		if (APlayerCharacter* Player =
			Cast<APlayerCharacter>(EventData.Actor))
		{
			if (UPlayerStats* PlayerStats =
				Player->GetComponentByClass<UPlayerStats>())
			{
				Damage *= PlayerStats->GetMaxStatValue(
					FGameplayTag::RequestGameplayTag(
						FName("Stats.Damage")
					)
				);
			}
		}

		if (APlayerCharacter* Player = Cast<APlayerCharacter>(Owner))
		{
			Player->GamePlayEvent.Broadcast({
				FGameplayTag::RequestGameplayTag(
					FName("Event.Enemy.DamageRequest")),
				this,
				Enemy,
				Damage
			});
		}
	}
}

void UAcidEffect::ApplyAcidDamage(const FGuid& AcidID)
{
	FAcidInstance* Instance = AcidInstances.Find(AcidID);

	if (!Instance) return;

	APlayerCharacter* SourcePlayer = Instance->SourcePlayer;

	if (!IsValid(SourcePlayer)) return;

	TArray<ABaseEnemy*> Enemies;

	if (!GetEnemiesInRange(
		FTransform(Instance->Location),
		AcidRange,
		Enemies))
	{
		return;
	}

	for (ABaseEnemy* Enemy : Enemies)
	{
		if (!IsValid(Enemy)) continue;

		float Damage = AcidDamage;

		if (UPlayerStats* PlayerStats =
			SourcePlayer->GetComponentByClass<UPlayerStats>())
		{
			Damage *= PlayerStats->GetMaxStatValue(
				FGameplayTag::RequestGameplayTag(
					FName("Stats.Damage")
				)
			);
		}

		SourcePlayer->GamePlayEvent.Broadcast({
			FGameplayTag::RequestGameplayTag(
				FName("Event.Enemy.DamageRequest")
			),
			this,
			Enemy,
			Damage
		});
	}
}

void UAcidEffect::ExpireAcid(const FGuid& AcidID)
{
	FAcidInstance* Instance = AcidInstances.Find(AcidID);

	if (!Instance) return;

	if (APlayerCharacter* Player = Instance->SourcePlayer)
	{
		if (UWorld* World = Player->GetWorld())
		{
			FTimerManager& TimerManager = World->GetTimerManager();

			TimerManager.ClearTimer(Instance->DamageTimerHandle);
			TimerManager.ClearTimer(Instance->ExpirationTimerHandle);
		}
	}

	if (IsValid(Instance->NiagaraComponent))
	{
		Instance->NiagaraComponent->SetAutoDestroy(true);
		Instance->NiagaraComponent->Deactivate();
	}

	AcidInstances.Remove(AcidID);
}

void UAcidEffect::ClearAcidInstances()
{
	for (auto& Pair : AcidInstances)
	{
		FAcidInstance& Instance = Pair.Value;

		if (APlayerCharacter* Player = Instance.SourcePlayer)
		{
			if (UWorld* World = Player->GetWorld())
			{
				FTimerManager& TimerManager = World->GetTimerManager();

				TimerManager.ClearTimer(Instance.DamageTimerHandle);
				TimerManager.ClearTimer(Instance.ExpirationTimerHandle);
			}
		}

		if (IsValid(Instance.NiagaraComponent))
		{
			Instance.NiagaraComponent->DeactivateImmediate();
			Instance.NiagaraComponent->DestroyComponent();
		}
	}

	AcidInstances.Empty();
}
