// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Hoomy_2DGameMode.generated.h"

/**
 * The GameMode defines the game being played. It governs the game rules, scoring, what actors
 * are allowed to exist in this game type, and who may enter the game.
 *
 * This game mode just sets the default pawn to be the MyCharacter asset, which is a subclass of Hoomy_2DCharacter
 */
UCLASS(minimalapi)
class AHoomy_2DGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AHoomy_2DGameMode();

	/** Remove the current menu widget and create a new one from the specified class, if provided. (���݂̃��j���[�E�B�W�F�b�g����菜���A�w�肳�ꂽ�N���X������΂�������V�������̂��쐬���܂�) */
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString currentStage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString nextStage;

	UFUNCTION(BlueprintCallable)
		FString GetCurrentStage() { return currentStage; }

	UFUNCTION(BlueprintCallable)
		FString GetNextStage() { return nextStage; }

protected:

	virtual void BeginPlay() override;

	/** The widget class we will use as our menu when the game starts. (�Q�[���J�n���Ƀ��j���[�Ƃ��Ďg�p����E�B�W�F�b�g �N���X�ł�) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
	TSubclassOf<UUserWidget> StartingWidgetClass;

	/** The widget instance that we are using as our menu.  (���j���[�Ƃ��Ďg�p����E�B�W�F�b�g �C���X�^���X�ł�) */
	UPROPERTY()
	UUserWidget*	m_pCurrentWidget;
};
