// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SubClassEnem/Mid/Wretches.h"


// Sets default values
AWretches::AWretches()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWretches::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWretches::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AWretches::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
