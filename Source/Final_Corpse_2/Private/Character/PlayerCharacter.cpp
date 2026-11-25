// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "TimerManager.h"
#include "Abilities/PlayerStats.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/StaminaComponent.h"
#include "Engine/Engine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Math/UnitConversion.h"
#include "UI/GameOverWidget.h"


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

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	bIsFastFalling = false;
	GamePlayEvent.Broadcast({FGameplayTag::RequestGameplayTag(FName("Event.Player.Landed")), this, nullptr, 0.0f});
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
		EnhancedInputComponent->BindAction(GroundDashAction, ETriggerEvent::Started, this, &APlayerCharacter::GroundDash);
		EnhancedInputComponent->BindAction(AirDashAction, ETriggerEvent::Started, this, &APlayerCharacter::AirDash);
		
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::OnCrouchPressed);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Canceled, this, &APlayerCharacter::OnCrouchReleased);
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
	
	if (GetCharacterMovement()->IsFalling())
	{
		
		//flag here
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("Not Enough Stamina: %s"), TryWallJump() ? TEXT("true") : TEXT("false")));
		}
		
		if (!TryWallJump())
		{
			if (JumpCurrentCount < JumpMaxCount)
			{
				if (UPlayerStats* PlayerStats = this->GetComponentByClass<UPlayerStats>())
				{
					float& Stamina = PlayerStats->GetCurrentStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Stamina")));
				
					if (Stamina < 100.0f)
					{
						//flag here
						if (GEngine)
						{
							GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("Not Enough Stamina: %.2f"), Stamina));
						}
					} else
					{
						GamePlayEvent.Broadcast({FGameplayTag::RequestGameplayTag(FName("Event.Stamina.MajorStaminaUsed")), this, nullptr, 0.0f});
					
						Jump();
					}
				}
			}
			
			
			
		}
	} else
	{
		GamePlayEvent.Broadcast({FGameplayTag::RequestGameplayTag(FName("Event.Player.Jumped")), this, nullptr, 0.0f});
		Jump();
	}
	
	//if (!CanJump()) return;
	
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

void APlayerCharacter::OnCrouchPressed()
{
	
	UMovementComponent* MovementComp = GetMovementComponent();
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	
	if (MovementComp && GetMovementComponent()->IsFalling())
	{
		if (UPlayerStats* PlayerStats = this->GetComponentByClass<UPlayerStats>())
		{
			float& Stamina = PlayerStats->GetCurrentStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Stamina")));
				
			if (Stamina < 50.0f)
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("Not Enough Stamina: %.2f"), Stamina));
				}
				return;
			}
		}
			
		if (!bIsFastFalling && (CurrentTime - LastCrouchPressTime) <= DoubleTapWindow)
		{
			TriggerFastFall();
			LastCrouchPressTime = -1.0f;
			return;
		}
	} else
	{
		Crouch();
	}
	
	LastCrouchPressTime = CurrentTime;
}

void APlayerCharacter::OnCrouchReleased()
{
	UnCrouch();
}

void APlayerCharacter::TriggerFastFall()
{
	bIsFastFalling = true;
	GamePlayEvent.Broadcast({FGameplayTag::RequestGameplayTag(FName("Event.Stamina.MinorStaminaUsed")), this, nullptr, 0.0f});
		
	LaunchCharacter(FVector(0.0f, 0.0f, -FastFallSpeed), false, true);
}

void APlayerCharacter::PrimaryAbility(const FInputActionValue& Value)
{	
	if (ActivePrimaryAbility)
	{
		ActivePrimaryAbility->Ability(this);
	} else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("NO GUN")));
		}
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
	
	//stamina check
	if (UPlayerStats* PlayerStats = this->GetComponentByClass<UPlayerStats>())
	{
		float& Stamina = PlayerStats->GetCurrentStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Stamina")));
		
		
		
		if (Stamina < 100.0f)
		{
			//flag here
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("Not Enough Stamina: %.2f"), Stamina));
			}
			return;
		}
		
		//subtract stamina
		GamePlayEvent.Broadcast({FGameplayTag::RequestGameplayTag(FName("Event.Stamina.MajorStaminaUsed")), this, nullptr, 0.0f});
		
		
		
		FVector InputDirection = GetLastMovementInputVector();
		if (InputDirection.IsNearlyZero())
		{
			InputDirection = GetActorForwardVector();
		}
		
		FVector DashVelocity = InputDirection.GetSafeNormal() * GroundDashVelocity;
		
		LaunchCharacter(DashVelocity, true, false);
		
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
	
	//stamina check
	if (UPlayerStats* PlayerStats = this->GetComponentByClass<UPlayerStats>())
	{
		float& Stamina = PlayerStats->GetCurrentStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Stamina")));

		if (Stamina < 100.0f)
		{
			if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("Not Enough Stamina: %.2f"), Stamina));
		}
			return;
		}
		
		//subtract stamina
		GamePlayEvent.Broadcast({FGameplayTag::RequestGameplayTag(FName("Event.Stamina.MajorStaminaUsed")), this, nullptr, 0.0f});
		
		FVector InputDirection = GetLastMovementInputVector();
		if (InputDirection.IsNearlyZero())
		{
			InputDirection = GetActorForwardVector();
		}
		
		FVector DashVelocity = InputDirection.GetSafeNormal() * AirDashVelocity;
		
		LaunchCharacter(DashVelocity, false, false);
		
		bIsDashing = true;

		GetWorldTimerManager().SetTimer(DashLockoutTimerHandle, [this]()
		{
			bIsDashing = false;
		}, DashLockoutTime, false);
	}
}

bool APlayerCharacter::TryWallJump()
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (!MovementComp || !MovementComp->IsFalling()) return false;
	
	FHitResult Hit;
	if (!FindWall(Hit)) return false;
	
	//stamina check
	if (UPlayerStats* PlayerStats = this->GetComponentByClass<UPlayerStats>())
	{
		float& Stamina = PlayerStats->GetCurrentStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Stamina")));
		
		if (Stamina < 50.0f)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("WJ Not Enough Stamina: %.2f"), Stamina));
			}
			return false;
		}
	}
	
	//subtract stamina
	GamePlayEvent.Broadcast({FGameplayTag::RequestGameplayTag(FName("Event.Stamina.MinorStaminaUsed")), this, nullptr, 0.0f});
	
	FVector CurrentVelocity = MovementComp->Velocity;
	CurrentVelocity.Z = 0.0f;
	
	FVector ParallelVel = FVector::VectorPlaneProject(CurrentVelocity, Hit.ImpactNormal);
	
	FVector HorizontalVel = ParallelVel + (Hit.ImpactNormal * WallJumpHorizontalForce);

	FVector FinalLaunchVelocity = HorizontalVel;
	FinalLaunchVelocity.Z = WallJumpVerticalForce;
	
	LaunchCharacter(FinalLaunchVelocity, true, true);
	return true;
}

bool APlayerCharacter::FindWall(FHitResult& OutHit) const
{
	const FVector Start = GetActorLocation();
	const FVector Forward = GetActorForwardVector();
	const FVector Right = GetActorRightVector();
	
	const TArray<FVector> Directions = {Forward, Right, -Right};
	
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	for (const FVector& Direction : Directions)
	{
		const FVector End = Start + (Direction * WallCheckDistance);
		if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
		{
			
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("WJ Hit Det: %.2f"), FMath::Abs(OutHit.ImpactNormal.Z)));
			}
			
			if (FMath::Abs(OutHit.ImpactNormal.Z) < 0.3f)
			{
				return true;
			}
		}
	}
	
	return false;
	
}

void APlayerCharacter::Hit(const FGameplayEventData& EventData)
{
	if (EventData.EventTag != FGameplayTag::RequestGameplayTag(FName("Event.Player.DamageRequest"))) return;
	
	if (UPlayerStats* PlayerStats = GetComponentByClass<UPlayerStats>())
	{
		float& Health = PlayerStats->GetCurrentStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Health")));
		const float& MaxHealth = PlayerStats->GetMaxStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Health")));
		Health = FMath::Clamp(Health - EventData.Magnitude, 0.0f, MaxHealth);
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("%.2f"), Health));
		}
		
		GamePlayEvent.Broadcast({FGameplayTag::RequestGameplayTag(FName("Event.Player.Hit")), EventData.Actor, this, EventData.Magnitude});
		
		if (!bIsDead && Health <= 0.0f)
		{
			if (APlayerController* PC = Cast<APlayerController>(GetController()))
			{
				bIsDead = true;
				DisableInput(PC);
				GetCharacterMovement()->StopMovementImmediately();
				
				GetWorldTimerManager().SetTimer(
						DeathTimerHandle,
						this,
						&APlayerCharacter::ShowGameOverUI,
						2.0f,
						false
					);
			}
		}
	}
}

void APlayerCharacter::ShowGameOverUI()
{
	
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, FString::Printf(TEXT("Inside GOUI")));
	}
	
	if (!bIsDead)
	{
		return;
	}
	
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UGameOverWidget* GameOverWidget = CreateWidget<UGameOverWidget>(PC, GameOverWidgetClass))
		{
			GameOverWidget->AddToViewport();
		
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(GameOverWidget->TakeWidget());
			PC->SetInputMode(InputMode);
			PC->bShowMouseCursor = true;
		}
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
	
	if (UPlayerStats* PlayerStats = GetComponentByClass<UPlayerStats>())
	{
		PlayerStats->GetCurrentStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Health"))) = PlayerStats->GetMaxStatValue(FGameplayTag::RequestGameplayTag(FName("Stats.Health")));
	}
	
	GamePlayEvent.AddDynamic(this, &APlayerCharacter::Hit);
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
