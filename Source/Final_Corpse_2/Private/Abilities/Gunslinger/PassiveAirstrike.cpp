// Fill out your copyright notice in the Description page of Project Settings.


#include "Final_Corpse_2/Public/Abilities/Gunslinger/PassiveAirstrike.h"


// Sets default values for this component's properties
UPassiveAirstrike::UPassiveAirstrike()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPassiveAirstrike::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPassiveAirstrike::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor* Character = GetOwner();
	//check if character is flying. then get the damage factor and bump it up by the damage multiplier
	// if (Character && Character->)
	// {
	// 	if (Chara)
	// }
}

