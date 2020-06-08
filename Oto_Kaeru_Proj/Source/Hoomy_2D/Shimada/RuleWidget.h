// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RuleWidget.generated.h"

#define SlidScale 20
#define MinX 0
#define MaxX -1920

UENUM()
enum ClickButton {
	Next = 0,
	Prev = 1
};


/**
 * @brief RuleWidget.h
 */
UCLASS()
class HOOMY_2D_API URuleWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	URuleWidget(const FObjectInitializer& ObjectInitializer);

protected:
	UFUNCTION(BlueprintCallable, Category = "Test")
		void TestClicked() { UE_LOG(LogTemp, Log, TEXT("Clicked")); }

	UFUNCTION(BlueprintCallable, Category = "Change")
		void ChangeClick();

	UFUNCTION(BlueprintCallable, Category = "True")
		void TrueFlag();

protected:
	// ������
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	// Tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//	�I������
	virtual void NativeDestruct() override;

	void MoveAnim();
	void EndLevel();

	/*
	// �e�N�X�`��
	UTexture2D* FirstTex;
	UTexture2D* LEndButtonTex;
	UTexture2D* NextButtonTex;
	UTexture2D* SecondTex;
	UTexture2D* PrevButtonTex;
	UTexture2D* REndButtonTex;
	UTexture2D* OuterTex;
	UTexture2D* RogoTex;
	*/

	// �{�^���N���b�N�p�t���O
	bool bClickFlag;

	// �����ꂽ�{�^��
	int32 ButtonNum;

	// �X���C�h�Ŏg��
	float PanelPosX;
	float PanelPosY;

	ClickButton m_pCButton;

private:
	UPROPERTY()
		USoundBase* Sound_Obj;

	UPROPERTY()
		class UImage* m_pFadeImage;
	// Exit�t���O
	bool bEndFlag;
	bool bFadeFlag;
	bool bInFade;
	float fAlpha;

	bool ChangeAlpha(class UImage* Image, bool FadeFlag);

};