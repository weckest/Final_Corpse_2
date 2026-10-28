// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Gunslinger/Gun.h"

#include "DrawDebugHelpers.h"
#include "Abilities/PlayerStats.h"
#include "Engine/Engine.h"


void UGun::Ability_Implementation(AActor* AbilityUser)
{
	if (!AbilityUser) return;
	
	Shoot(AbilityUser);
}

// Sets default values
UGun::UGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGun::Shoot(AActor* AbilityUser)
{
	
	if (CurrentCooldownTime > 0.0f)
	{
		return;
	} else if (Ammo <= 0)
	{
		Reload();
	} else if (UPlayerStats* PlayerStats = AbilityUser->FindComponentByClass<UPlayerStats>())
	{
		this->Ammo--;
		CurrentCooldownTime = CooldownTime;
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, TEXT("Shooting"));
		}
		
		FVector StartLocation;
		FRotator ViewRotation;
	
		AbilityUser->GetActorEyesViewPoint(StartLocation, ViewRotation);
	
		FVector ForwardVector = ViewRotation.Vector();
		FVector EndLocation = StartLocation + (ForwardVector * AbilityRange);
		
		float BulletSpreadStep = Spread / Bullets;
		
		EndLocation = EndLocation.RotateAngleAxis(-(Spread / 2) + (BulletSpreadStep / 2), {0,0,1});
			
		FHitResult HitResult;
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(AbilityUser);
		
		for (int i = 0; i < Bullets; i++)
		{
			bool bHit = AbilityUser->GetWorld()->LineTraceSingleByChannel(
				HitResult,
				StartLocation,
				EndLocation.RotateAngleAxis(BulletSpreadStep * i, {0,0,1}),
				ECC_Visibility,
				TraceParams
			);
	
			FColor LineColor = bHit ? FColor::Green : FColor::Red;
	
			DrawDebugLine(
				AbilityUser->GetWorld(),
				StartLocation,
				EndLocation.RotateAngleAxis(BulletSpreadStep * i, {0,0,1}), 
				LineColor,
				false,
				5.0f,
				0,
				1.5f
			);
		}
		
		
	}
}

void UGun::Reload()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, TEXT("Reloading"));
	}
	Ammo = MaxAmmo;
}

// Called every frame
void UGun::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CurrentCooldownTime = FMath::Clamp(CurrentCooldownTime -= DeltaTime, 0.0f, CooldownTime);
	
	if (Ammo <= 0)
	{
		Reload();
	}
}
