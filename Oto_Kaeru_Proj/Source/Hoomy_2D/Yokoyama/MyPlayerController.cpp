// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "GameManager.h"


//-------------------------------------------------------------
// Name: BeginPlay()
// Desc: �Q�[���J�n���O�ɌĂ΂�鏉��������
//-------------------------------------------------------------
void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI());
}



//-------------------------------------------------------------
// Name: SetInputComponent()
// Desc: ���̓C�x���g�Ɗ֐����o�C���h����
//-------------------------------------------------------------
void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AMyPlayerController::HandleClickEvent);
}



//-------------------------------------------------------------
// Name: HandleClickEvent()
// Desc: ���N���b�N�ƃo�C���h����֐�
//-------------------------------------------------------------
void AMyPlayerController::HandleClickEvent()
{
	AGameManager* GM = AGameManager::GetInstance();
	if (GM)
	{
		GM->LeftClickEvent();
	}
}