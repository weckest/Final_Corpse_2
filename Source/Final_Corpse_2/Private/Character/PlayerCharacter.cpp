// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


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
	
	Jump();
}

void APlayerCharacter::StopJump()
{
	StopJumping();
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (!GetController()) return;
	
	FVector2D MovementVector = Value.Get<FVector2d>();
	
	AddMovementInput(GetActorForwardVector(), MovementVector.X);
	AddMovementInput(GetActorRightVector(), MovementVector.Y);
}

void APlayerCharacter::PrimaryAbility(const FInputActionValue& Value)
{	
	if (ActivePrimaryAbility)
	{
		
		IAbility::Execute_Ability(ActivePrimaryAbility.GetObject(), this);
	}
}

void APlayerCharacter::SecondaryAbility(const FInputActionValue& Value)
{
	if (ActiveSecondaryAbility)
	{
		IAbility::Execute_Ability(ActiveSecondaryAbility.GetObject(), this);
	}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	UActorComponent* ResolvedComp = PrimaryAbilityReference.GetComponent(this);
	
	if (ResolvedComp && ResolvedComp->Implements<UAbility>())
	{
		ActivePrimaryAbility = ResolvedComp;
	}
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

