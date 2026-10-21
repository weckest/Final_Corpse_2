// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/EliteEnem.h"


// Sets default values
AEliteEnem::AEliteEnem()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEliteEnem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEliteEnem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEliteEnem::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

