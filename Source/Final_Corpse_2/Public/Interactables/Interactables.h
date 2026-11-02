// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactables.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UInteractables : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FINAL_CORPSE_2_API IInteractables
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void Interact(AActor* Interactor, UPrimitiveComponent* HitComponent) = 0;
};
