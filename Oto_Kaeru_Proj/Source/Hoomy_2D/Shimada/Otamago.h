/**
* @file		Otamago.h
* @brief	オタマジャクシ.h
* @author	yshimada
* @data		20200420
*/
#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Otamago.generated.h"

class UTextRenderComponent;

/**
 * @broef 卵カエル
 */
UCLASS(Config = Game)
class HOOMY_2D_API AOtamago : public APaperCharacter
{
	GENERATED_BODY()

		UTextRenderComponent* TextComponent;
	virtual void Tick(float DeltaSeconds) override;

protected:
	// 卵の時のアニメーション
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* OtamagoAnimation;

	// オタマジャクシ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* OtamaAnimation;

	// アニメーション切替用
	void UpdateAnimation();

	void UpdateCharacter();

public:
	AOtamago();

	UPROPERTY(VisibleAnywhere)
		int nOtamaPattern;

	// 水ブロックに入った時
	void Otama();

};
