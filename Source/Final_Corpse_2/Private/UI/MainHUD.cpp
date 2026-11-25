// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHUD.h"

#include "Abilities/PlayerStats.h"
#include "Character/PlayerCharacter.h"
#include "Components/HorizontalBox.h"
#include "Components/InventoryComponent.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "UI/Widgets/ItemSlotWidget.h"

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
				Character->GamePlayEvent.AddDynamic(this, &UMainHUD::UpdatePrimaryAbility);
				Character->GamePlayEvent.AddDynamic(this, &UMainHUD::UpdateSecondaryAbility);
				Character->GamePlayEvent.AddDynamic(this, &UMainHUD::HandleHealthEvent);
			}
		}
		
		if (InventoryComponent)
		{
			InventoryComponent->OnInventoryUpdated.AddDynamic(this, &UMainHUD::RefreshItemBar);
		}
		
		RefreshHealth();
		RefreshItemBar();
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

void UMainHUD::HandleHealthEvent(const FGameplayEventData& GameplayEventData)
{
	static const FGameplayTag HitTag = FGameplayTag::RequestGameplayTag(FName("Event.Player.Hit"));
	static const FGameplayTag HealedTag = FGameplayTag::RequestGameplayTag(FName("Event.Player.Healed"));
	
	if (GameplayEventData.EventTag == HitTag || GameplayEventData.EventTag == HealedTag)
	{
		RefreshHealth();
	}
}

void UMainHUD::RefreshHealth()
{
	if (!PlayerStats || !HealthBar) return;
	
	static const FGameplayTag healthTag = FGameplayTag::RequestGameplayTag(FName("Stats.Health"));
	const float* Current = PlayerStats->CurrentStats.Find(healthTag);
	const float* Max = PlayerStats->MaxStats.Find(healthTag);
	
	if (!Current || !Max || *Max <= 0.f) return;
	
	HealthBar->SetPercent(*Current / *Max);
	
	if (HealthText)
	{
		HealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), *Current, *Max)));
	}
}

void UMainHUD::RefreshItemBar()
{
	if (!InventoryComponent || !Itembar || !ItemSlotClass) return;
	
	Itembar->ClearChildren();
	
	for (const auto& Pair : InventoryComponent->GetItems())
	{
		if (UItemSlotWidget* ItemSlot = CreateWidget<UItemSlotWidget>(this, ItemSlotClass))
		{
			ItemSlot->SetItem(Pair.Key, Pair.Value);
			Itembar->AddChild(ItemSlot);
		}
	}
}
