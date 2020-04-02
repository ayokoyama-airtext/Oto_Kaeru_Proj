// Fill out your copyright notice in the Description page of Project Settings.


#include "GameUserWidget.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "GameManager.h"


//-------------------------------------------------------------
// Name: UGameUserWidget()
// Desc: Ctor
//-------------------------------------------------------------
UGameUserWidget::UGameUserWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), m_fImageTimer(0), m_ePhase(Normal), m_iClearFlag(0)
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

	m_pBlackImage = Cast<UImage>(GetWidgetFromName("BlackImage"));
	m_pBlackImage->SetRenderOpacity(0);
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


	switch (m_ePhase)
	{
	case Normal:
		break;
	case Clear:
		if (ShowImage(m_pClearImage, TIME_TO_SHOW_IMAGE, InDeltaTime))
		{
			m_fImageTimer = 0;
			m_ePhase = BlackOut;
		}
		break;
	case GameOver:
		if (ShowImage(m_pGameOverImage, TIME_TO_SHOW_IMAGE, InDeltaTime))
		{
			m_fImageTimer = 0;
			m_ePhase = BlackOut;
		}
		break;
	case BlackOut:
		if (ShowImage(m_pBlackImage, TIME_TO_BLACK_OUT, InDeltaTime))
		{
			m_fImageTimer = 0;
			m_ePhase = EndScene;
		}
		break;
	case EndScene:
		if (m_iClearFlag)
		{
			UGameplayStatics::OpenLevel(world, TEXT("/Game/Working/Sugita/Map/SugiMap"));
		}
		else
		{
			UGameplayStatics::OpenLevel(world, TEXT("/Game/Working/Miyamoto/Map/MiyaMap"));
		}
		break;
	default:
		break;
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
	m_ePhase = Clear;
	m_iClearFlag = 1;
}



//-------------------------------------------------------------
// Name: ShowGameOverImage()
// Desc: 
//-------------------------------------------------------------
void UGameUserWidget::ShowGameOverImage()
{
	m_ePhase = GameOver;
}



//-------------------------------------------------------------
// Name: ShowImage()
// Desc: 一定時間をかけてイメージを表示する
// Out : 未完了 / false, 完了 / true
//-------------------------------------------------------------
bool UGameUserWidget::ShowImage(UImage* pImage, float MaxTime, float DeltaTime)
{
	bool retVal = true;

	if (m_fImageTimer <= MaxTime)
	{
		float rate_ = 0;

		if ((m_fImageTimer += DeltaTime) < MaxTime)
		{
			rate_ = m_fImageTimer * (1.0f / MaxTime);
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