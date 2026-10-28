// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ItemEffect.generated.h"

class AActor;
/**
 * 
 */
UCLASS(Abstract, Blueprintable, EditInlineNew)
class FINAL_CORPSE_2_API UItemEffect : public UObject
{
	GENERATED_BODY()
	
public:
	virtual void OnAdded(AActor* Owner, int32 Count) {}
	
	virtual void OnCountChanged(int32 NewCount) {}
	
	virtual void OnRemoved(AActor* Owner) {}
	
};
