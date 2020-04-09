/**
* @file		GameUserWidget.h
* @brief	ゲームUIクラス
* @author	Akihiro Yokoyama
*/
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameUserWidget.generated.h"

//-------------------------------------------------------------
// Macros
//-------------------------------------------------------------
#define TIME_TO_SHOW_IMAGE	1.5f
#define TIME_TO_BLACK_OUT	0.5f


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
	void SetMaxClickNum(int num);
	UFUNCTION()
	void UpdateClickNumText(int currentClickNum);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateClickNumBP(int currentClickNum);
	UFUNCTION()
	void ShowClearImage();
	UFUNCTION()
	void ShowGameOverImage();

private:
	//
	//	Enum
	//
	enum Phase
	{
		Opening,
		OpeningWait,
		OpeningEnd,
		Normal,
		Clear,
		GameOver,
		EndingWait,
		BlackOut,
		EndScene
	};

	//
	//	各Widget
	//
	UPROPERTY()
	class UTextBlock*	m_pClickNumText;

	UPROPERTY()
	class UImage*		m_pClearImage;

	UPROPERTY()
	class UImage*		m_pGameOverImage;

	UPROPERTY()
	class UImage*		m_pStageNameImage;

	UPROPERTY()
	class UImage*		m_pBlackImage;

	//
	//	Variants
	//
	int		m_iMaxClickNum;
	float	m_fImageTimer;
	Phase	m_ePhase;
	int		m_iClearFlag;

	//
	//	Functions
	//
	bool ChangeImageVisibility(class UImage* pImage, float MaxTime, float DeltaTime, bool bShowImage);

};
