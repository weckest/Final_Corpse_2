// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/StaminaComponent.h"


// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	CurrentStamina = MaxStamina;
	
}


// Called every frame
void UStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	CurrentStamina = FMath::Clamp(CurrentStamina + (StaminaRegenRate * DeltaTime), 0, MaxStamina); 
	//need to adjust stamina regen rate and/or make it accessible to be updated with items

	// ...
}

void UStaminaComponent::HandleUseStamina(float AmountToUse)
{
	if (CurrentStamina < AmountToUse)
	{
		OnStaminaUpdated.Broadcast(FText::FromString(TEXT("Not Enough Stamina")), CurrentStamina);
		return;
	}
	
	CurrentStamina = FMath::Clamp(CurrentStamina - AmountToUse, 0, MaxStamina);
}
