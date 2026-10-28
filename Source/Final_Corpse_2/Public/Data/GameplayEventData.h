// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/ObjectMacros.h"
#include "GameplayEventData.generated.h"

class AActor;

USTRUCT(BlueprintType)
struct FINAL_CORPSE_2_API FGameplayEventData
{
	GENERATED_BODY()	
	
	UPROPERTY(BlueprintReadWrite)
	FGameplayTag EventTag;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> Actor;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<AActor> Target;
	
	UPROPERTY(BlueprintReadWrite)
	float Magnitude;
};
