// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Gunslinger/Gun.h"

#include "DrawDebugHelpers.h"
#include "Abilities/PlayerStats.h"
#include "Character/PlayerCharacter.h"
#include "Enemies/BaseEnemy.h"
#include "Enemies/EnemyStatsComponent.h"
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
		
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(AbilityUser))
		{
			Player->GamePlayEvent.Broadcast({FGameplayTag::RequestGameplayTag(FName("Event.Player.Ability.Primary")), Player, nullptr, 0.0f});
		}
		
		FVector StartLocation;
		FRotator ViewRotation;

		AbilityUser->GetActorEyesViewPoint(StartLocation, ViewRotation);

		FVector ForwardVector = ViewRotation.Vector();
		FVector RightVector = ViewRotation.RotateVector(FVector::RightVector);

		FVector EndLocation = StartLocation + (ForwardVector * AbilityRange);

		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(AbilityUser);

		// Calculate the angular distance between each pellet.
		float BulletSpreadStep = Bullets > 1 ? Spread / (Bullets - 1) : 0.0f;

		for (int i = 0; i < Bullets; i++)
		{
			// Center the spread around the player's forward direction.
			float SpreadAngle = -(Spread / 2.0f) + (BulletSpreadStep * i);

			// Rotate the forward direction around the camera's up axis.
			FVector BulletDirection = ForwardVector.RotateAngleAxis(
				SpreadAngle,
				FVector::UpVector
			);

			FVector BulletEnd = StartLocation + (BulletDirection * AbilityRange);

			FHitResult HitResult;

			bool bHit = AbilityUser->GetWorld()->LineTraceSingleByChannel(
				HitResult,
				StartLocation,
				BulletEnd,
				ECC_Visibility,
				TraceParams
			);

			FColor LineColor = bHit ? FColor::Green : FColor::Red;

			// DrawDebugLine(
			// 	AbilityUser->GetWorld(),
			// 	StartLocation,
			// 	BulletEnd,
			// 	LineColor,
			// 	false,
			// 	5.0f,
			// 	0,
			// 	1.5f
			// );
			
			if (!bHit) return;
			
			AActor* HitActor = HitResult.GetActor();
			if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(HitActor))
			{
				UEnemyStatsComponent* EnemyStats = Enemy->GetComponentByClass<UEnemyStatsComponent>();
			}
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
