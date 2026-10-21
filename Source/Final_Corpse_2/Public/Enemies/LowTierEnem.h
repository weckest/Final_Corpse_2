// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "LowTierEnem.generated.h"

UCLASS()
class FINAL_CORPSE_2_API ALowTierEnem : public ABaseEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALowTierEnem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
