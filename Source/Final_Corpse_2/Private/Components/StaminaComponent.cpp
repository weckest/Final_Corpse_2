// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StaminaComponent.h"

#include "Abilities/PlayerStats.h"
#include "Character/PlayerCharacter.h"
#include "Data/GameplayEventData.h"
#include "Data/Effects/LifestealEffect.h"


// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner()))
	{
		PlayerCharacter->GamePlayEvent.AddDynamic(this, &UStaminaComponent::HandleUseStamina);
	}
	// ...
	
}


// Called every frame
void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		if (UPlayerStats* PlayerStats = Player->GetComponentByClass<UPlayerStats>())
		{
			float& Stamina = PlayerStats->GetCurrentStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Stamina")));
			const float& MaxStamina = PlayerStats->GetMaxStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Stamina")));
			const float& StaminaRegenRate = PlayerStats->GetMaxStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.StaminaRegenRate")));
			
			//need to adjust stamina regen rate and/or make it accessible to be updated with items
			Stamina = FMath::Clamp(Stamina + (StaminaRegenRate * DeltaTime), 0, MaxStamina); 
		}
	}
}

// EVENTS THAT CALL THIS METHOD SHOULD BE THE ONES HANDLING SOUND AND UI!!!!
void UStaminaComponent::HandleUseStamina(const FGameplayEventData& GameplayEventData)
{
	if (GameplayEventData.EventTag != FGameplayTag::RequestGameplayTag("Event.Stamina.MajorStaminaUsed") || 
		GameplayEventData.EventTag != FGameplayTag::RequestGameplayTag("Event.Stamina.MinorStaminaUsed")) return;
	
	if (const APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner()))
	{
		if (UPlayerStats* PlayerStats = Player->GetComponentByClass<UPlayerStats>())
		{
			float& Stamina = PlayerStats->GetCurrentStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Stamina")));
			const float& MaxStamina = PlayerStats->GetMaxStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Stamina")));
			
			//Sort by stamina level
			if (GameplayEventData.EventTag == FGameplayTag::RequestGameplayTag("Event.Stamina.MajorStaminaUse"))
			{
				Stamina = FMath::Clamp(Stamina - MajorStaminaCost, 0, MaxStamina);
			}

			Stamina = FMath::Clamp(Stamina - MinorStaminaCost, 0, MaxStamina);
		}
	}

	// if (CurrentStamina < 100)
	// {
	// 	OnStaminaUpdated.Broadcast(FText::FromString(TEXT("Not Enough Stamina")), CurrentStamina);
	// 	return;
	// }
	//
	// CurrentStamina = FMath::Clamp(CurrentStamina - 100, 0, MaxStamina);
}
