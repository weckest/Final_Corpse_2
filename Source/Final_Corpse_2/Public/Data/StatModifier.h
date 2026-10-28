// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/ObjectMacros.h"
#include "StatModifier.generated.h"

UENUM(BlueprintType)
enum class EStatModifierOperation : uint8 {
	Add,
	Multiply,
};

USTRUCT(BlueprintType)
struct FStatModifier
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modifier")
	FGameplayTag StatTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modifier")
	EStatModifierOperation EditOperation = EStatModifierOperation::Add;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modifier")
	EStatModifierOperation CalculateOperation = EStatModifierOperation::Add;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Modifier")
	float Magnitude = 0.0f;
};