// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class HOOMY_2D_API UGameUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UGameUserWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION()
	void SetMaxClickNum(int num) { m_iMaxClickNum = num; }
	UFUNCTION()
	void UpdateClickNumText(int currentClickNum);

private:
	//
	//	ŠeWidget
	//
	UPROPERTY()
	class UTextBlock*	m_pClickNumText;

	UPROPERTY()
	class UImage*		m_pClearImage;

	UPROPERTY()
	class UImage*		m_pGameOverImage;

	//
	//	Variants
	//
	int		m_iMaxClickNum;
	
};
