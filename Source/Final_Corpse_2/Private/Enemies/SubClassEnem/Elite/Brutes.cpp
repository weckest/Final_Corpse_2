// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/SubClassEnem/Elite/Brutes.h"

#include "DrawDebugHelpers.h"
#include "Character/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Enemies/EnemyStatsComponent.h"


// Sets default values
ABrutes::ABrutes()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
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

void ABrutes::ApplyAttackHit()
{
	if (isDead() || !EnemyStats) return;
	
	const float HalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	const FVector Center = GetActorLocation() 
			+ GetActorForwardVector().GetSafeNormal2D() * AoeForwardOffset
			- FVector(0.f, 0.f, HalfHeight);
	
	if (bDrawDebugAoe)
	{
		DrawDebugCircle(GetWorld(), Center + FVector(0,0,5), AoeRadius, 48, FColor::Red,
			false, 1.0f, 0, 3.0f, FVector(1,0,0),FVector(0,1,0), false);
	}
	
	APlayerCharacter* Player = GetTargetPlayer();
	if (!Player) return;
	
	const FVector PlayerFeet = Player->GetActorLocation() - FVector(0.f,0.f,Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	
	if (FVector::DistSquared2D(Center,PlayerFeet) > AoeRadius * AoeRadius) return;
	if (PlayerFeet.Z - Center.Z > AoeMaxHeight) return;
	
	SendPlayerDamage(EnemyStats->BaseAttack);
}
