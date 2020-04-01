// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RuleWidget.generated.h"

/**
 * @brief RuleWidget.h
 */
UCLASS()
class HOOMY_2D_API URuleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	URuleWidget(const FObjectInitializer& ObjectInitializer);

	// Exitフラグ
	UPROPERTY()
	bool bEndFlag;

protected:
	// 初期化
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	// Tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//	終了処理
	virtual void NativeDestruct() override;

	int32 CheckClick();

	void MoveAnim(int32 dir);

	/*
	// テクスチャ
	UTexture2D* FirstTex;
	UTexture2D* LEndButtonTex;
	UTexture2D* NextButtonTex;
	UTexture2D* SecondTex;
	UTexture2D* PrevButtonTex;
	UTexture2D* REndButtonTex;
	UTexture2D* OuterTex;
	UTexture2D* RogoTex;
	*/

	// ボタンクリック用フラグ
	bool bClickFlag;

	// 押されたボタン
	int32 ButtonNum;

	// スライドで使う
	float PanelPosX;
	float PanelPosY;
};

#define SlidScale 20
#define MinX 0
#define MaxX 1920