// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Gunslinger/PassiveAirstrike.h"

#include "Abilities/PlayerStats.h"
#include "Character/PlayerCharacter.h"
#include "Engine/Engine.h"
#include "GameFramework/PawnMovementComponent.h"


// Sets default values for this component's properties
UPassiveAirstrike::UPassiveAirstrike()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPassiveAirstrike::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	
	Player->GamePlayEvent.AddDynamic(this, &UPassiveAirstrike::AirStrikeAbilityAdd);
	Player->GamePlayEvent.AddDynamic(this, &UPassiveAirstrike::AirStrikeAbilityRemove);
	

	// ...
	
}


// Called every frame
void UPassiveAirstrike::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Character = GetOwner();
	//check if character is flying. then get the damage factor and bump it up by the damage multiplier
	// if (Character && Character->)
	// {
	// 	if (Chara)
	// }
}

void UPassiveAirstrike::AirStrikeAbilityAdd(const FGameplayEventData& GamePlayEventData)
{
	if (GamePlayEventData.EventTag != FGameplayTag::RequestGameplayTag(FName("Event.Player.Jumped"))) return;
	
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(GamePlayEventData.Actor))
	{
		if (!Player->GetMovementComponent()->IsFalling())
		{
			if (UPlayerStats* Stats = Player->GetComponentByClass<UPlayerStats>())
			{
				Stats->UpdateMaxStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Damage")), DamageMultiplier, EEditType::Multiply);
			
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Increasing Damage"));
				}
			}
		}
	}
}

void UPassiveAirstrike::AirStrikeAbilityRemove(const FGameplayEventData& GameplayEventData)
{
	if (GameplayEventData.EventTag != FGameplayTag::RequestGameplayTag(FName("Event.Player.Landed"))) return;
	
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(GameplayEventData.Actor))
	{
		if (UPlayerStats* Stats = Player->GetComponentByClass<UPlayerStats>())
		{
			Stats->UpdateMaxStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Damage")), DamageMultiplier, EEditType::Divide);
			
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Decreasing Damage"));
			}
		}
	}
}
