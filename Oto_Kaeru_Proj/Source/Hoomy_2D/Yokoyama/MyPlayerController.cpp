// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "GameManager.h"


//-------------------------------------------------------------
// Name: BeginPlay()
// Desc: ゲーム開始直前に呼ばれる初期化処理
//-------------------------------------------------------------
void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI());
}



//-------------------------------------------------------------
// Name: SetInputComponent()
// Desc: 入力イベントと関数をバインドする
//-------------------------------------------------------------
void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AMyPlayerController::HandleClickEvent);
}



//-------------------------------------------------------------
// Name: HandleClickEvent()
// Desc: 左クリックとバインドする関数
//-------------------------------------------------------------
void AMyPlayerController::HandleClickEvent()
{
	AGameManager* GM = AGameManager::GetInstance();
	if (GM)
	{
		GM->LeftClickEvent();
	}
}