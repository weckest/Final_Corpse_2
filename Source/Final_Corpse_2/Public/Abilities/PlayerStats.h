// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerStats.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FINAL_CORPSE_2_API UPlayerStats : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerStats();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Health")
	float Health = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Health")
	float BaseHealth = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Health")
	float MaxHealth = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Health")
	float BaseMaxHealth = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Health")
	float HealthRegen = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Health")
	float BaseHealthRegen = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Health")
	float HealthRegenRate = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Health")
	float BaseHealthRegenRate = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Damage")
	float Damage = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Damage")
	float BaseDamage = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Damage")
	float DamageMultiplier = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Damage")
	float BaseDamageMultiplier = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Cooldown")
	float Cooldown = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Cooldown")
	float BaseCooldown = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Speed")
	float MaxSpeed = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Speed")
	float BaseMaxSpeed = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Speed")
	float SpeedAcceleration = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Speed")
	float BaseSpeedAcceleration = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Jump")
	int DoubleJumps = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Jump")
	int BaseDoubleJumps = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Jump")
	int MaxDoubleJumps = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stats|Jump")
	int BaseMaxDoubleJumps = 1;
	
};
