// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Shop/Shop.h"

#include "Character/PlayerCharacter.h"


class APlayerCharacter;
// Sets default values
AShop::AShop()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AShop::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShop::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TArray<UItemDataAsset*> AShop::RandomizeItems()
{
	return TArray<UItemDataAsset*>();
}

bool AShop::PurchaseItem(int32 ItemIndex)
{
	return false;
}

void AShop::OpenShopUI(AActor* Interactor)
{
	if (!Interactor) return;
	
	// Check if the Interactor is a player character
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(Interactor);
	if (!PlayerCharacter) return;
	
	// open the UI for the player character
	if (ShopWidget && ShopWidget->IsInViewport()) return;
	if (!ShopWidgetClass) return;
	
	// grab the controller
	APlayerController* PC = PlayerCharacter->GetController<APlayerController>();
	
	//create widget and put it on screen
	ShopWidget = CreateWidget<UShopWidget>(PC, ShopWidgetClass);
	if (!ShopWidget) return;
	
	if (RolledItems.IsEmpty())
	{
		RolledItems = RandomizeItems();
	}
	
	ShopWidget->OwningShop = this;
	ShopWidget->setupShop(RolledItems);
	ShopWidget->AddToViewport();
	
	// switch controls to UI mode
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(ShopWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PC->SetInputMode(InputMode);
	PC->bShowMouseCursor = true;
	
}

void AShop::CloseShopUI()
{
}

void AShop::Interact(AActor* Interactor, UPrimitiveComponent* HitComponent)
{
	
	OpenShopUI(Interactor);
	
}

