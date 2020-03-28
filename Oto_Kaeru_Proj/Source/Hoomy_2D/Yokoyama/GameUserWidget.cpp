// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUserWidget.h"
#include "Engine/GameEngine.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "GameManager.h"


//-------------------------------------------------------------
// Name: UGameUserWidget()
// Desc: Ctor
//-------------------------------------------------------------
UGameUserWidget::UGameUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), m_bShowClearImage(false), m_bShowGameOverImage(false), m_fImageTimer(0)
{
}



//-------------------------------------------------------------
// Name: NativeConstruct()
// Desc: 開始処理(出現時)
//-------------------------------------------------------------
void UGameUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pClickNumText = Cast<UTextBlock>(GetWidgetFromName("ClickNumText"));

	m_pClearImage = Cast<UImage>(GetWidgetFromName("ClearImage"));
	m_pClearImage->SetRenderOpacity(0);

	m_pGameOverImage = Cast<UImage>(GetWidgetFromName("GameOverImage"));
	m_pGameOverImage->SetRenderOpacity(0);
}



//-------------------------------------------------------------
// Name: NativeTick()
// Desc: アニメーション
//-------------------------------------------------------------
void UGameUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	UWorld* world = GEngine->GameViewport->GetWorld();

	if (world == nullptr)	return;

	if (world->IsPaused())	return;

	Super::NativeTick(MyGeometry, InDeltaTime);


	//	Clear Image
	if (m_bShowClearImage)
	{
		if (ShowImage(m_pClearImage, InDeltaTime))
		{
			m_bShowClearImage = false;
			m_fImageTimer = 0;
		}
	}

	//	GameOver Image
	if (m_bShowGameOverImage)
	{
		if (ShowImage(m_pGameOverImage, InDeltaTime))
		{
			m_bShowGameOverImage = false;
			m_fImageTimer = 0;
		}
	}
}



//-------------------------------------------------------------
// Name: UpdateClickNumText()
// Desc: アニメーション
//-------------------------------------------------------------
void UGameUserWidget::UpdateClickNumText(int current)
{
	if (!m_pClickNumText)
		return;

	FText text_ = FText::FromString(FString::Printf(TEXT("%d / %d"), current, m_iMaxClickNum));
	m_pClickNumText->SetText(text_);
}



//-------------------------------------------------------------
// Name: ShowClearImage()
// Desc: 
//-------------------------------------------------------------
void UGameUserWidget::ShowClearImage()
{
	m_bShowClearImage = true;
}



//-------------------------------------------------------------
// Name: ShowGameOverImage()
// Desc: 
//-------------------------------------------------------------
void UGameUserWidget::ShowGameOverImage()
{
	m_bShowGameOverImage = true;
}



//-------------------------------------------------------------
// Name: ShowImage()
// Desc: 一定時間をかけてイメージを表示する
// Out : 未完了 / false, 完了 / true
//-------------------------------------------------------------
bool UGameUserWidget::ShowImage(UImage* pImage, float DeltaTime)
{
	bool retVal = true;

	if (m_fImageTimer <= TIME_TO_SHOW_IMAGE)
	{
		float rate_ = 0;

		if ((m_fImageTimer += DeltaTime) < TIME_TO_SHOW_IMAGE)
		{
			rate_ = m_fImageTimer * (1.0f / TIME_TO_SHOW_IMAGE);
			retVal = false;
		}
		else
		{
			rate_ = 1.0f;
		}

		pImage->SetRenderOpacity(rate_);
	}

	return retVal;
}