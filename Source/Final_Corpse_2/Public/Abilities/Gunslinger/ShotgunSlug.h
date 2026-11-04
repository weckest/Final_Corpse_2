// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShotgunSlug.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FINAL_CORPSE_2_API UShotgunSlug : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UShotgunSlug();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	void Shoot(AActor* AbilityUser);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	float Damage = 25.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	float CooldownTime = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	float CurrentCooldownTime = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	float AbilityRange = 1000.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities")
	int Bullets = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (ClampMax = "360.0"))
	float Spread = 10.0f;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	
	
};
