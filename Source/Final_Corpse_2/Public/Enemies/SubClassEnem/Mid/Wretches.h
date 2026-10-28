// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/MidTierEnem.h"
#include "Wretches.generated.h"

UCLASS()
class FINAL_CORPSE_2_API AWretches : public AMidTierEnem
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWretches();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
