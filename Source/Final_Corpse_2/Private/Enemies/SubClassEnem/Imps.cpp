// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SubClassEnem/Mid/Imps.h"


// Sets default values
AImps::AImps()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AImps::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AImps::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AImps::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
