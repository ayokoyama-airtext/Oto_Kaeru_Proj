// Fill out your copyright notice in the Description page of Project Settings.


#include "RuleWidget.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"


// �R���X�g���N�^
URuleWidget::URuleWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer), bClickFlag(false), ButtonNum(0), bEndFlag(false), PanelPosX(0), PanelPosY(0), bFadeFlag(true), bInFade(true), fAlpha(1.0)
{
	m_pCButton = Prev;

	// �R���X�g���N�^
	//// �T�E���h�E�F�[�u�I�u�W�F�N�g��T��
	//static ConstructorHelpers::FObjectFinder< USoundBase > find_sound(TEXT("SoundWave'/Content/Working/Soundroom/�N���b�N'"));
	//if (find_sound.Succeeded()) {
	//	Sound_Obj = find_sound.Object;
	//}

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

	m_pFadeImage = Cast<UImage>(GetWidgetFromName("FadeImage"));
	m_pFadeImage->SetRenderOpacity(fAlpha);

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

	if (bInFade)
		bFadeFlag = ChangeAlpha(m_pFadeImage, bFadeFlag);

	if (bClickFlag)
		MoveAnim();

	// �|�W�V�����ύX
	UCanvasPanel* PanelWidget = Cast<UCanvasPanel>(GetWidgetFromName("SlidPanel"));
	UCanvasPanelSlot* CanvasPanelSlot = Cast<UCanvasPanelSlot>(PanelWidget->Slot);

	CanvasPanelSlot->SetPosition(FVector2D(PanelPosX, PanelPosY));

	UButton* _pButtonNTex = Cast<UButton>(GetWidgetFromName("NextButton"));
	UButton* _pButtonPTex = Cast<UButton>(GetWidgetFromName("PrevButton"));
	UButton* _pButtonEndTex = Cast<UButton>(GetWidgetFromName("EndButton"));

	// �N���b�N
	if (!bClickFlag && !bInFade) {
		// Next
		if (_pButtonNTex) {
			if (!_pButtonNTex->OnClicked.IsBound()) {
				// �f���Q�[�g�ǉ�
				_pButtonNTex->OnClicked.AddDynamic(this, &URuleWidget::ChangeClick);
			}
		}
		// Prev
		if (_pButtonPTex) {
			if (!_pButtonPTex->OnClicked.IsBound()) {
				// �f���Q�[�g�ǉ�
				_pButtonPTex->OnClicked.AddDynamic(this, &URuleWidget::ChangeClick);
			}
		}
		// End
		if (_pButtonEndTex) {
			if (!_pButtonEndTex->OnClicked.IsBound()) {
				_pButtonEndTex->OnClicked.AddDynamic(this, &URuleWidget::TrueFlag);
			}
		}
	}

	if (bEndFlag) {
		EndLevel();
	}

}

void URuleWidget::ChangeClick()
{
	bClickFlag = true;

	//UGameplayStatics::SpawnSound2D(GetWorld(), Sound_Obj, 1.0f, 1.0f, 0.0f, nullptr, false, false);

	if (m_pCButton == Next)
		m_pCButton = Prev;
	else
		m_pCButton = Next;
}

void URuleWidget::MoveAnim()
{
	UE_LOG(LogTemp, Log, TEXT("MoveAnim"));

	switch (m_pCButton)
	{
	case Next:
		if (PanelPosX > MaxX) {
			PanelPosX -= SlidScale;
			UE_LOG(LogTemp, Log, TEXT("Ok"));
		}
		else {
			//PanelPosX = 1.0f;
			bClickFlag = false;
			UE_LOG(LogTemp, Log, TEXT("No"));
		}

		UE_LOG(LogTemp, Log, TEXT("In"));

		break;

	case Prev:
		if (PanelPosX < MinX)
			PanelPosX += SlidScale;
		else
			bClickFlag = false;

		break;

	}

}

void URuleWidget::EndLevel()
{
	UGameplayStatics::OpenLevel(this, TEXT("/Game/Working/Shimada/Map/Title"));
}

void URuleWidget::TrueFlag()
{
	UE_LOG(LogTemp, Log, TEXT("������"));

	// �|�W�V�����ύX
	UCanvasPanel* FadePanelWidget = Cast<UCanvasPanel>(GetWidgetFromName("FadePanel"));
	UCanvasPanelSlot* FadeCanvasPanelSlot = Cast<UCanvasPanelSlot>(FadePanelWidget->Slot);
	FadeCanvasPanelSlot->SetPosition(FVector2D(0.0, 0.0));

	bInFade = true;
}

bool URuleWidget::ChangeAlpha(UImage* Image, bool FadeFlag)
{
	if (FadeFlag) {
		if (fAlpha > 0.0)
			fAlpha -= 0.01;
		else {
			UE_LOG(LogTemp, Log, TEXT("toumei"));
			// �|�W�V�����ύX
			UCanvasPanel* FadePanelWidget = Cast<UCanvasPanel>(GetWidgetFromName("FadePanel"));
			UCanvasPanelSlot* FadeCanvasPanelSlot = Cast<UCanvasPanelSlot>(FadePanelWidget->Slot);
			FadeCanvasPanelSlot->SetPosition(FVector2D(-1920.0, 0.0));

			bInFade = false;
			return false;
		}
	}
	else {
		if (fAlpha < 1.0)
			fAlpha += 0.01;
		else {
			UE_LOG(LogTemp, Log, TEXT("kuro"));
			bInFade = false;
			bEndFlag = true;
			return true;
		}
	}

	m_pFadeImage->SetRenderOpacity(fAlpha);

	return FadeFlag;
}