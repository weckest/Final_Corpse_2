// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "TimerManager.h"
#include "Abilities/PlayerStats.h"
#include "Camera/CameraComponent.h"
#include "Components/StaminaComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/UnitConversion.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
		
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::DoJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Canceled, this, &APlayerCharacter::StopJump);
		
		//Abilities
		EnhancedInputComponent->BindAction(PrimaryAbilityAction, bIsPrimaryAbilityHoldable ? ETriggerEvent::Triggered : ETriggerEvent::Started, this, &APlayerCharacter::PrimaryAbility);
		EnhancedInputComponent->BindAction(SecondaryAbilityAction, bIsSecondaryAbilityHoldable ? ETriggerEvent::Triggered : ETriggerEvent::Started, this, &APlayerCharacter::SecondaryAbility);
	
		//Movement
		EnhancedInputComponent->BindAction(GroundDashAction, ETriggerEvent::Triggered, this, &APlayerCharacter::GroundDash);
		EnhancedInputComponent->BindAction(AirDashAction, ETriggerEvent::Triggered, this, &APlayerCharacter::AirDash);
	}
	
	
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	if (!GetController()) return;
	
	FVector2D LookAxisVector = Value.Get<FVector2d>();
	
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void APlayerCharacter::DoJump()
{
	if (!CanJump()) return;
	GamePlayEvent.Broadcast({FGameplayTag::RequestGameplayTag(FName("Event.Player.Jumped")), this, nullptr, 0.0f});
	Jump();
}

void APlayerCharacter::StopJump()
{
	StopJumping();
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (!GetController()) return;
	UPlayerStats* PlayerStats = this->GetComponentByClass<UPlayerStats>();
	
	if (!PlayerStats) return;
	
	FGameplayTag Tag = FGameplayTag::RequestGameplayTag(FName("Stats.Speed"), false);
	
	if (!Tag.IsValid()) return;
	
	float* SpeedModifier = PlayerStats->MaxStats.Find(Tag);
	
	if (!SpeedModifier) return;
	
	FVector2D MovementVector =  Value.Get<FVector2d>() * *SpeedModifier * 600.0f;
	
	AddMovementInput(GetActorForwardVector(), MovementVector.X);
	AddMovementInput(GetActorRightVector(), MovementVector.Y);
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	GamePlayEvent.Broadcast({FGameplayTag::RequestGameplayTag(FName("Event.Player.Landed")), this, nullptr, 0.0f});
}

void APlayerCharacter::PrimaryAbility(const FInputActionValue& Value)
{	
	if (ActivePrimaryAbility)
	{
		ActivePrimaryAbility->Ability(this);
	}
}

void APlayerCharacter::SecondaryAbility(const FInputActionValue& Value)
{
	if (ActiveSecondaryAbility)
	{
		ActiveSecondaryAbility->Ability(this);
	}
}

void APlayerCharacter::GroundDash(const FInputActionValue& Value)
{
	
	
	
	//on ground and is not dashing
	if (!GetCharacterMovement()->IsMovingOnGround() || bIsDashing) return;
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, TEXT("Past First Check"));
	}
	
	//stamina check
	if (UPlayerStats* PlayerStats = this->GetComponentByClass<UPlayerStats>())
	{
		// float& Stamina = PlayerStats->GetCurrentStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Stamina")));
		//
		// if (GEngine)
		// {
		// 	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::SanitizeFloat(Stamina));
		// }
		//
		// if (Stamina < 100.0f)
		// {
		// 	//flag here
		// 	return;
		// }
		//
		// //subtract stamina
		// GamePlayEvent.Broadcast({FGameplayTag::RequestGameplayTag(FName("Event.Stamina.MajorStaminaUsed")), this, nullptr, 0.0f});
		//
		
		FVector Launch = GetVelocity(); //CHANGE THESE HARDCODES
		Launch.X = Launch.X * 100.0f;
		Launch.Y = Launch.Y * 100.0f;
		
		LaunchCharacter(Launch, false, false);
		
		bIsDashing = true;

		GetWorldTimerManager().SetTimer(DashLockoutTimerHandle, [this]()
		{
			bIsDashing = false;
		}, DashLockoutTime, false);
	}
	
}

void APlayerCharacter::AirDash(const FInputActionValue& Value)
{
	//in air and is not dashing
	if (GetCharacterMovement()->IsMovingOnGround() || bIsDashing) return;
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, TEXT("Past First Check"));
	}
	
	//stamina check
	if (UPlayerStats* PlayerStats = this->GetComponentByClass<UPlayerStats>())
	{
		// float& Stamina = PlayerStats->GetCurrentStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Stamina")));
		//
		// if (GEngine)
		// {
		// 	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::SanitizeFloat(Stamina));
		// }
		//
		// if (Stamina < 100.0f)
		// {
		// 	//flag here
		// 	return;
		// }
		//
		// //subtract stamina
		// GamePlayEvent.Broadcast({FGameplayTag::RequestGameplayTag(FName("Event.Stamina.MajorStaminaUsed")), this, nullptr, 0.0f});
		//
		
		FVector Launch = GetVelocity(); //CHANGE THESE HARDCODES
		Launch.X = Launch.X * AirDashVelocity;
		Launch.Y = Launch.Y * AirDashVelocity;
		Launch.Z = 0.0f;
		
		LaunchCharacter(Launch, false, false);
		
		bIsDashing = true;

		GetWorldTimerManager().SetTimer(DashLockoutTimerHandle, [this]()
		{
			bIsDashing = false;
		}, DashLockoutTime, false);
	}
}


// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UActorComponent* ResolvedPrimaryComp = PrimaryAbilityReference.GetComponent(this);
	
	if (UAbility* Ability = Cast<UAbility>(ResolvedPrimaryComp))
	{
		ActivePrimaryAbility = Ability;
	}
	
	UActorComponent* ResolvedSecondaryComp = SecondaryAbilityReference.GetComponent(this);
	
	if (UAbility* Ability = Cast<UAbility>(ResolvedSecondaryComp))
	{
		
		ActiveSecondaryAbility = Ability;
	}
	
	for (auto PassiveAbility : PassiveAbilityReference)
	{
		UActorComponent* PassiveComponent = PassiveAbility.GetComponent(this);
		
		if (PassiveComponent)
		{
			ActivePassiveAbility.Add(PassiveComponent);
		}
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

