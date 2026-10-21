// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/MidTierEnem.h"


// Sets default values
AMidTierEnem::AMidTierEnem()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMidTierEnem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMidTierEnem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMidTierEnem::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

