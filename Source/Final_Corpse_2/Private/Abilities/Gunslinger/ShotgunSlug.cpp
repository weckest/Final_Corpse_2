// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/Gunslinger/ShotgunSlug.h"

#include "DrawDebugHelpers.h"
#include "Abilities/PlayerStats.h"
#include "Character/PlayerCharacter.h"
#include "Enemies/BaseEnemy.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"


// Sets default values for this component's properties
UShotgunSlug::UShotgunSlug()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UShotgunSlug::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UShotgunSlug::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CurrentCooldownTime = FMath::Clamp(CurrentCooldownTime -= DeltaTime, 0.0f, CooldownTime);
	// ...
}

void UShotgunSlug::Shoot(AActor* AbilityUser)
{
	
	if (CurrentCooldownTime > 0.0f)
	{
		return;
	} else if (UPlayerStats* PlayerStats = AbilityUser->FindComponentByClass<UPlayerStats>())
	{
		CurrentCooldownTime = CooldownTime;
		
		if (APlayerCharacter* Player = Cast<APlayerCharacter>(AbilityUser))
		{
			Player->GamePlayEvent.Broadcast({FGameplayTag::RequestGameplayTag(FName("Event.Player.Ability.Primary")), Player, nullptr, 0.0f});
		}
		
		FVector StartLocation;
		FRotator ViewRotation;

		AbilityUser->GetActorEyesViewPoint(StartLocation, ViewRotation);

		FVector ForwardVector = ViewRotation.Vector() + FVector{0.0f, 0.0f, 0.25f};
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

			DrawDebugLine(
				AbilityUser->GetWorld(),
				StartLocation,
				BulletEnd,
				LineColor,
				false,
				5.0f,
				0,
				1.5f
			);
			
			if (!bHit) continue;
			
			AActor* HitActor = HitResult.GetActor();
			
			if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(HitActor))
			{
				float DealtDamage = Damage * AbilityUser->GetComponentByClass<UPlayerStats>()->GetMaxStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Damage")));
				
				APlayerCharacter* Player = Cast<APlayerCharacter>(AbilityUser);
				Player->GamePlayEvent.Broadcast({FGameplayTag::RequestGameplayTag(FName("Event.Enemy.DamageRequest")), Player, HitActor, DealtDamage});
			}
		}
		
		
	}
}
