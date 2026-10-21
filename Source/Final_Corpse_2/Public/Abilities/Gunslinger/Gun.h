// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Final_Corpse_2/Public/Abilities/Ability.h"
#include "Gun.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FINAL_CORPSE_2_API UGun : public UActorComponent, public IAbility
{
private:
	virtual void Ability_Implementation(AActor* AbilityUser) override;
	
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGun();
	void Shoot(AActor* AbilityUser);
	void Reload();

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	int MaxAmmo = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	int Ammo = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	float CooldownTime = 1.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	float CurrentCooldownTime = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	float AbilityRange = 1000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	int Bullets = 2;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (ClampMax = "360.0"))
	float Spread = 20.0f;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
