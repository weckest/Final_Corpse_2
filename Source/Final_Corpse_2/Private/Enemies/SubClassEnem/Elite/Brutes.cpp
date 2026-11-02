// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SubClassEnem/Elite/Brutes.h"


// Sets default values
ABrutes::ABrutes()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABrutes::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABrutes::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABrutes::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
