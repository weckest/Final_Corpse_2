// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Gunslinger/PassiveBackBlast.h"

#include "Abilities/PlayerStats.h"
#include "Character/PlayerCharacter.h"
#include "Engine/Engine.h"
#include "GameFramework/PawnMovementComponent.h"


// Sets default values for this component's properties
UPassiveBackBlast::UPassiveBackBlast()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	
}


// Called when the game starts
void UPassiveBackBlast::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	
	Player->GamePlayEvent.AddDynamic(this, &UPassiveBackBlast::BackBlast);
	
}


// Called every frame
void UPassiveBackBlast::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPassiveBackBlast::BackBlast(const FGameplayEventData& EventData)
{
	if (EventData.EventTag != FGameplayTag::RequestGameplayTag(FName("Event.Player.Ability.Primary")) &&
		EventData.EventTag != FGameplayTag::RequestGameplayTag(FName("Event.Player.Ability.Secondary"))) return;
	
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(EventData.Actor))
	{
		if (Player->GetMovementComponent()->IsFalling())
		{
			FVector Forward = Player->GetActorForwardVector();
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Blasting Bros Back Before: %f"), Forward.Dot(Player->GetActorRightVector())));
			}
			Forward *= -Force * Player->GetComponentByClass<UPlayerStats>()->GetMaxStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Damage")));
			
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, FString::Printf(TEXT("Blasting Bros Back: %f"), Forward.Length()));
			}
			
			Player->LaunchCharacter(Forward, true, true);
		}
	}
}
