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
	: Super(ObjectInitializer), m_fImageTimer(0), m_ePhase(Opening), m_iClearFlag(0)
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

	m_pStageNameImage = Cast<UImage>(GetWidgetFromName("StageNameImage"));
	m_pStageNameImage->SetRenderOpacity(0);

	m_pBlackImage = Cast<UImage>(GetWidgetFromName("BlackImage"));
	m_pBlackImage->SetRenderOpacity(0);
	m_pBlackImage->SetVisibility(ESlateVisibility::Visible);
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
	case Opening:
		if (ChangeImageVisibility(m_pStageNameImage, TIME_TO_SHOW_IMAGE, InDeltaTime, true))
		{
			m_fImageTimer = 0;
			m_ePhase = OpeningWait;
		}
		break;
	case OpeningWait:
		if ((m_fImageTimer += InDeltaTime) >= TIME_TO_SHOW_IMAGE)
		{
			m_fImageTimer = 0;
			m_ePhase = OpeningEnd;
		}
		break;
	case OpeningEnd:
		if (ChangeImageVisibility(m_pStageNameImage, TIME_TO_SHOW_IMAGE, InDeltaTime, false))
		{
			m_fImageTimer = 0;
			AGameManager::NotifyOpeningEnd();
			m_pBlackImage->SetVisibility(ESlateVisibility::HitTestInvisible);
			m_ePhase = Normal;
		}
		break;
	case Normal:
		break;
	case Clear:
		if (ChangeImageVisibility(m_pClearImage, TIME_TO_SHOW_IMAGE, InDeltaTime, true))
		{
			m_fImageTimer = 0;
			m_ePhase = EndingWait;
		}
		break;
	case GameOver:
		if (ChangeImageVisibility(m_pGameOverImage, TIME_TO_SHOW_IMAGE, InDeltaTime, true))
		{
			m_fImageTimer = 0;
			m_ePhase = EndingWait;
		}
		break;
	case EndingWait:
		if ((m_fImageTimer += InDeltaTime) >= TIME_TO_SHOW_IMAGE)
		{
			m_fImageTimer = 0;
			m_ePhase = BlackOut;
		}
		break;
	case BlackOut:
		if (ChangeImageVisibility(m_pBlackImage, TIME_TO_BLACK_OUT, InDeltaTime, true))
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
// In  : [bShowImage] true/表示, false/非表示
// Out : false/未完了, true/完了
//-------------------------------------------------------------
bool UGameUserWidget::ChangeImageVisibility(UImage* pImage, float MaxTime, float DeltaTime, bool bShowImage)
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

		if (bShowImage)
		{
			pImage->SetRenderOpacity(rate_);
		}
		else
		{
			pImage->SetRenderOpacity(1.0f - rate_);
		}
	}

	return retVal;
}
