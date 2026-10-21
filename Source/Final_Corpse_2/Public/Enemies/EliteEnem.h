// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/BaseEnemy.h"
#include "EliteEnem.generated.h"

UCLASS()
class FINAL_CORPSE_2_API AEliteEnem : public ABaseEnemy
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEliteEnem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
