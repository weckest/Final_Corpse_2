// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/EliteEnem.h"
#include "Brutes.generated.h"

UCLASS()
class FINAL_CORPSE_2_API ABrutes : public AEliteEnem
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABrutes();
	
	virtual void ApplyAttackHit() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category="Enemy|Attack|AoE" , meta = (ClampMin = "0"))
	float AoeRadius = 350.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Enemy|Attack|AoE")
	float AoeForwardOffset = 100.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Enemy|Attack|AoE" , meta = (ClampMin = "0"))
	float AoeMaxHeight = 120.0f;
	
	UPROPERTY(EditDefaultsOnly, Category="Enemy|Attack|AoE")
	bool bDrawDebugAoe = true;
	
	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
