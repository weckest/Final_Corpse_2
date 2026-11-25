// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/ItemSlotWidget.h"

#include "Algo/Count.h"
#include "Data/ItemDataAsset.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UItemSlotWidget::SetItem(UItemDataAsset* Item, int32 count)
{
	if (!Item)
	{
		return;
	}
	
	if (Item->Icon)
	{
		Icon->SetBrushFromTexture(Item->Icon);
	}
	
	Count->SetText(count > 1 ? FText::AsNumber(count): FText::GetEmpty());
}
