// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/FinalCorpse2GameMode.h"
#include "Character/BaseCharacter.h"

AFinalCorpse2GameMode::AFinalCorpse2GameMode()
{
	DefaultPawnClass = ABaseCharacter::StaticClass();
}
