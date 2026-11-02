// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SubClassEnem/Elite/Burrowers.h"


// Sets default values
ABurrowers::ABurrowers()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABurrowers::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABurrowers::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABurrowers::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
