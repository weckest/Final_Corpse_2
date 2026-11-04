// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractablesSpawner.generated.h"

UCLASS()
class FINAL_CORPSE_2_API AInteractablesSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractablesSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
