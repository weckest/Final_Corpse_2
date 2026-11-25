// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameOverWidget.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (GO_Button)
	{
		GO_Button->OnClicked.AddDynamic(this, &UGameOverWidget::OnButtonClicked);
	}
	
	//GetScore, GetHighScore, SaveHighScoreIfHighScore
	
	
	
}

void UGameOverWidget::OnButtonClicked()
{
	UGameplayStatics::OpenLevel(this, MainMenuLevelName);
}
