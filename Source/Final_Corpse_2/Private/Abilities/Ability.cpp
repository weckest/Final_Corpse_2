// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Ability.h"

#include "Engine/Engine.h"


void UAbility::Ability(AActor* AbilityUser)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("HOW DID YOU GET HERE"));
	}
}
