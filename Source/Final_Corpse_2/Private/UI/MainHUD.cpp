// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHUD.h"

#include "Abilities/PlayerStats.h"
#include "Character/PlayerCharacter.h"
#include "Components/InventoryComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"

void UMainHUD::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (APawn* Pawn = PC->GetPawn())
		{
			InventoryComponent = Pawn->FindComponentByClass<UInventoryComponent>();
			PlayerStats = Pawn->FindComponentByClass<UPlayerStats>();
		}
		
		if (PlayerStats)
		{
			AActor* Player = PlayerStats->GetOwner();
			if (APlayerCharacter* Character = Cast<APlayerCharacter>(Player))
			{
				Character->GamePlayEvent.AddDynamic(this, &UMainHUD::UpdateStaminaBar);
			}
		}
	}
}

void UMainHUD::UpdateStaminaBar(const FGameplayEventData& GameplayEventData)
{
	if (GameplayEventData.EventTag != FGameplayTag::RequestGameplayTag(FName("Event.Stamina.Changed"))) return;
	
	//update the stamina bar TO-DO
}

void UMainHUD::UpdatePrimaryAbility(const FGameplayEventData& GameplayEventData)
{
	if (GameplayEventData.EventTag != FGameplayTag::RequestGameplayTag(FName("Event.Player.Ability.Primary.Ready"))) return;
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Primary Ready"));
	}
}

void UMainHUD::UpdateSecondaryAbility(const FGameplayEventData& GameplayEventData)
{
	if (GameplayEventData.EventTag != FGameplayTag::RequestGameplayTag(FName("Event.Player.Ability.Secondary.Ready"))) return;
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Secondary Ready"));
	}
}
