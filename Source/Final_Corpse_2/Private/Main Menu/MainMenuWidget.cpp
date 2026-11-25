// Fill out your copyright notice in the Description page of Project Settings.


#include "Main Menu/MainMenuWidget.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (PlayButton)
	{
		PlayButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnPlayClicked);
	}
	
	if (SettingsButton)
	{
		SettingsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnSettingsClicked);
	}
	if (CreditsButton)
	{
		CreditsButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnCreditsClicked);
	}

	if (SettingsBackButton)
	{
		SettingsBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnBackClicked);
	}
	if (CreditsBackButton)
	{
		CreditsBackButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnBackClicked);
	}
		
	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnQuitClicked);
	}
		
	if (MenuSwitcher && MainMenuPanel)
	{
		MenuSwitcher->SetActiveWidget(MainMenuPanel);
	}
	
}

void UMainMenuWidget::OnPlayClicked()
{
	if (APlayerController* PC = GetOwningPlayer())
	{
		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
		PC->bShowMouseCursor = false;
	}
	
	UGameplayStatics::OpenLevel(this, GameplayLevelName);
}

void UMainMenuWidget::OnSettingsClicked()
{
	if (MenuSwitcher && SettingsPanel)
	{
		MenuSwitcher->SetActiveWidget(SettingsPanel);
	}
}

void UMainMenuWidget::OnCreditsClicked()
{
	if (MenuSwitcher && CreditsPanel)
	{
		MenuSwitcher->SetActiveWidget(CreditsPanel);
	}
}

void UMainMenuWidget::OnBackClicked()
{
	if (MenuSwitcher && MainMenuPanel)
	{
		MenuSwitcher->SetActiveWidget(MainMenuPanel);
	}
}

void UMainMenuWidget::OnQuitClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(),EQuitPreference::Quit, false);
}
