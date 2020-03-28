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
	: Super(ObjectInitializer)
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