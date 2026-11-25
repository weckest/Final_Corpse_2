// Fill out your copyright notice in the Description page of Project Settings.


#include "Main Menu/MainMenuGameMode.h"

#include "Main Menu/MainMenuPlayerController.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	DefaultPawnClass = nullptr;
	
	PlayerControllerClass = AMainMenuPlayerController::StaticClass();
}
