// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SubClassEnem/Low/Rats.h"


// Sets default values
ARats::ARats()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ARats::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARats::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ARats::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
