// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectStageWidget.h"
#include "Components/Image.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"
#include "StageSelectManager.h"


//-------------------------------------------------------------
// Name: UGameUserWidget()
// Desc: Ctor
//-------------------------------------------------------------
USelectStageWidget::USelectStageWidget(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer), m_pBlackImage(nullptr), m_fImageTimer(0), m_iPhase(0), m_pManager(nullptr)
{
}



void USelectStageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	m_pBlackImage = Cast<UImage>(GetWidgetFromName("BlackImage"));
	m_pBlackImage->SetRenderOpacity(1);
	m_pBlackImage->SetVisibility(ESlateVisibility::Visible);
}

void USelectStageWidget::NativeTick(const FGeometry & MyGeo, float InDeltaTime)
{
	UWorld *world = GEngine->GameViewport->GetWorld();

	if (world == nullptr)
		return;

	Super::NativeTick(MyGeo, InDeltaTime);

	switch (m_iPhase)
	{
	case 0:
		if (ChangeImageVisibility(m_pBlackImage, 1.0f, InDeltaTime, false))
		{
			m_pBlackImage->SetVisibility(ESlateVisibility::HitTestInvisible);
			m_fImageTimer = 0;
			m_iPhase = 1;
		}
		break;

	case 1:
		break;

	case 2:
		if (ChangeImageVisibility(m_pBlackImage, 2.0f, InDeltaTime, true))
		{
			m_fImageTimer = 0;
			m_iPhase = 3;
		}
		break;

	case 3:
		if (m_pManager)
		{
			m_pManager->LoadNextMap();
			m_iPhase = 4;
		}
		break;
	case 4:
		break;
	}
}



//-------------------------------------------------------------
// Name: ShowImage()
// Desc: 一定時間をかけてイメージを表示する
// In  : [bShowImage] true/表示, false/非表示
// Out : false/未完了, true/完了
//-------------------------------------------------------------
bool USelectStageWidget::ChangeImageVisibility(UImage* pImage, float MaxTime, float DeltaTime, bool bShowImage)
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


void USelectStageWidget::StartLoadMap()
{
	m_iPhase = 2;
	m_pBlackImage->SetVisibility(ESlateVisibility::Visible);
}