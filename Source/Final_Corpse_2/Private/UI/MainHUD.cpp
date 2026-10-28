// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHUD.h"

#include "Abilities/PlayerStats.h"
#include "Character/PlayerCharacter.h"
#include "Components/InventoryComponent.h"
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
