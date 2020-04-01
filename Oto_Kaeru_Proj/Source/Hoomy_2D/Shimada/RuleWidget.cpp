// Fill out your copyright notice in the Description page of Project Settings.


#include "RuleWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UObject/ConstructorHelpers.h"


// �R���X�g���N�^
URuleWidget::URuleWidget(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer), bClickFlag(false), ButtonNum(0), bEndFlag(false), PanelPosX(0), PanelPosY(0)
{
	/*
	// �e�N�X�`���f�[�^�̎擾
	static ConstructorHelpers::FObjectFinder<UTexture2D> _DataFileFTex(TEXT("/Game/MainFolder/BG/OtoTitleBG")),
		_DataFileSTex(TEXT("/Game/MainFolder/BG/OtoTitleBG")),
		_DataFileEFbTex(TEXT("/Game/MainFolder/BG/OtoTitleBG")),
		_DataFileNbTex(TEXT("/Game/MainFolder/BG/OtoTitleBG")),
		_DataFilePbTex(TEXT("/Game/MainFolder/BG/OtoTitleBG")),
		_DataFileESbTex(TEXT("/Game/MainFolder/BG/OtoTitleBG")),
		_DataFileOuterTex(TEXT("/Game/MainFolder/BG/Has")),
		_DataFileRogoTex(TEXT("/Game/Working/Shimada/UI/RuleRogo"));

	// Image�i�����j
	if (_DataFileFTex.Object)
		FirstTex = _DataFileFTex.Object;
	if (_DataFileSTex.Object)
		SecondTex = _DataFileSTex.Object;

	// Button
	if (_DataFileEFbTex.Object)
		LEndButtonTex = _DataFileEFbTex.Object;
	if (_DataFileNbTex.Object)
		NextButtonTex = _DataFileNbTex.Object;
	if (_DataFilePbTex.Object)
		PrevButtonTex = _DataFilePbTex.Object;
	if (_DataFileESbTex.Object)
		REndButtonTex = _DataFileESbTex.Object;

	// Image�i�O�g�j
	if (_DataFileOuterTex.Object)
		OuterTex = _DataFileOuterTex.Object;

	// Image�i���S�j
	if (_DataFileRogoTex.Object)
		RogoTex = _DataFileRogoTex.Object;
	*/
}

//*********************************************************
// ����������

// ������
void URuleWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	/*
	// �ewidget�p��
	// �e�N�X�`���\��t��
	// �{�^���ȊOBP�œ\��t���܂������ɂ��������̂�
	UImage*  _pImageFTex = Cast<UImage>(GetWidgetFromName("FirstImage"));
	UButton* _pButtonEFTex = Cast<UButton>(GetWidgetFromName("End01Button"));
	UButton* _pButtonNTex = Cast<UButton>(GetWidgetFromName("NextButton"));
	UImage*  _pImageSTex = Cast<UImage>(GetWidgetFromName("SecondImage"));
	UButton* _pButtonPTex = Cast<UButton>(GetWidgetFromName("PrevButton"));
	UButton* _pButtonESTex = Cast<UButton>(GetWidgetFromName("End02Button"));
	UImage*  _pImageOuterTex = Cast<UImage>(GetWidgetFromName("OuterImage"));
	UImage*  _pImageRogoTex = Cast<UImage>(GetWidgetFromName("RogoImage"));

	if (_pImageFTex)
		_pImageFTex->SetBrushFromTexture(FirstTex);
	if (_pImageSTex)
		_pImageSTex->SetBrushFromTexture(SecondTex);

	if (_pButtonEFTex)
		_pButtonEFTex->SetStyle()

	if (_pImageOuterTex)
		_pImageOuterTex->SetBrushFromTexture(OuterTex);
	if (_pImageRogoTex)
		_pImageRogoTex->SetBrushFromTexture(RogoTex);
		*/
}

// �J�n�O
void URuleWidget::NativePreConstruct()
{
	Super::NativePreConstruct();


}

// �o����
void URuleWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

// �C��
void URuleWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void URuleWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (!bClickFlag)
		ButtonNum = CheckClick();

	if (ButtonNum == 1)
		bEndFlag = true;
	else if (ButtonNum == 2)
		MoveAnim(ButtonNum);
	else if (ButtonNum == 3)
		MoveAnim(ButtonNum);

}


// �{�^���̃`�F�b�N�m�F
int32 URuleWidget::CheckClick()
{
	UButton* _pButtonEFTex = Cast<UButton>(GetWidgetFromName("LeftEndButton"));
	UButton* _pButtonNTex = Cast<UButton>(GetWidgetFromName("NextButton"));
	UButton* _pButtonPTex = Cast<UButton>(GetWidgetFromName("PrevButton"));
	UButton* _pButtonESTex = Cast<UButton>(GetWidgetFromName("RightEndButton"));

	// �N���b�N
	if (!_pButtonEFTex->OnClicked.IsBound() || !_pButtonESTex->OnClicked.IsBound()) {
		return 1;
	}
	if (!_pButtonNTex->OnClicked.IsBound()) {
		return 2;
	}
	if (!_pButtonPTex->OnClicked.IsBound()) {
		return 3;
	}

	return 0;
}

void URuleWidget::MoveAnim(int32 dir)
{
	UCanvasPanel* PanelWidget = Cast<UCanvasPanel>(GetWidgetFromName("SlidPanel"));
	UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(PanelWidget->Slot);

	switch (dir)
	{
	case 2:
		if (PanelPosX > -MaxX)
			PanelPosX -= SlidScale;
		else
			bClickFlag = !bClickFlag;

	case 3:
		if (PanelPosX < MinX)
			PanelPosX += SlidScale;
		else
			bClickFlag = !bClickFlag;

	}

	CanvasPanelSlot->SetPosition(FVector2D(PanelPosX, PanelPosY));
}