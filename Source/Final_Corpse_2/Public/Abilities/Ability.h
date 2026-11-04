// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UObject/Interface.h"
#include "Ability.generated.h"

// This class does not need to be modified.
UCLASS()
class UAbility : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Abilities")
	virtual void Ability(AActor* AbilityUser);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	bool bIsReady = true;
};
