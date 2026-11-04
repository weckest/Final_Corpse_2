// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Data/ItemDataAsset.h"
#include "GameFramework/Actor.h"
#include "Interactables/Interactables.h"
#include "UI/Widgets/ShopWidget.h"
#include "Shop.generated.h"

UCLASS()
class FINAL_CORPSE_2_API AShop : public AActor, public IInteractables
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	
	UPROPERTY(EditDefaultsOnly, Category="Shop")
	TSubclassOf<UShopWidget> ShopWidgetClass;
	
	UPROPERTY()
	TObjectPtr<UShopWidget> ShopWidget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shop")
	TArray<TObjectPtr<UItemDataAsset>> RolledItems;
	
	// method to randomize 3 items from the item pool and display them in the shop
	// prices for each item should also be displayed
	UFUNCTION(BlueprintCallable, Category = "Shop")
	TArray<UItemDataAsset*> RandomizeItems();
	
	// method to handle the purchase of an item by the player
	UFUNCTION(BlueprintCallable, Category = "Shop")
	bool PurchaseItem(int32 ItemIndex);
	
	//method to handle displaying the shop UI and allowing the player to select an item to purchase
	UFUNCTION(BlueprintCallable, Category = "UI")
	void OpenShopUI(AActor* Interactor);
	
	// method to handle closing the shop UI and returning control to the player
	UFUNCTION(BlueprintCallable, Category = "UI")
	void CloseShopUI();
	
	// Implement the IInteractables interface
	virtual void Interact(AActor* Interactor, UPrimitiveComponent* HitComponent) override;
};
