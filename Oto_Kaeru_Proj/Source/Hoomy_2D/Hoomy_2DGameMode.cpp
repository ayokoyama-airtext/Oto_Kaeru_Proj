// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "Hoomy_2DGameMode.h"
#include "Hoomy_2DCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

AHoomy_2DGameMode::AHoomy_2DGameMode()
	:m_pCurrentWidget(nullptr)
{
	// Set default pawn class to our character
	DefaultPawnClass = AHoomy_2DCharacter::StaticClass();	
}



void AHoomy_2DGameMode::BeginPlay()
{
	Super::BeginPlay();
	ChangeMenuWidget(StartingWidgetClass);

	currentStage = UGameplayStatics::ParseOption(OptionsString, TEXT("Current"));
	nextStage = UGameplayStatics::ParseOption(OptionsString, TEXT("Next"));
}



void AHoomy_2DGameMode::ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass)
{
	if (m_pCurrentWidget != nullptr)
	{
		m_pCurrentWidget->RemoveFromParent();
		m_pCurrentWidget = nullptr;
	}

	if (NewWidgetClass != nullptr)
	{
		m_pCurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
		if (m_pCurrentWidget != nullptr)
		{
			m_pCurrentWidget->AddToViewport();
		}
	}
}