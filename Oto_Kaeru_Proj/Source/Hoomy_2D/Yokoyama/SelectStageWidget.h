// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectStageWidget.generated.h"

/**
 * 
 */
UCLASS()
class HOOMY_2D_API USelectStageWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	USelectStageWidget(const FObjectInitializer& ObjInit);

	void SetParent(class AStageSelectManager* manager) { m_pManager = manager; }
	void StartLoadMap();

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeo, float InDeltaTime) override;
	
	UPROPERTY()
		class UImage*	m_pBlackImage;

	UPROPERTY()
		class AStageSelectManager*	m_pManager;
	
	float	m_fImageTimer;
	int		m_iPhase;
	bool ChangeImageVisibility(class UImage* pImage, float MaxTime, float DeltaTime, bool bShowImage);
};
