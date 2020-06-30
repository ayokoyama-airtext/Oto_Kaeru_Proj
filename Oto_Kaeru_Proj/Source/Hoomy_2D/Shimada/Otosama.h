/**
* @file		Otosama.h
* @brief	音のさま.h
* @author	yshimada
* @data		20200420
*/
#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Otosama.generated.h"

class UTextRenderComponent;

/**
 * @brief MainChara(Otonosama)
 */
UCLASS(Config = Game)
class HOOMY_2D_API AOtosama : public APaperCharacter
{
	GENERATED_BODY()

		UTextRenderComponent* TextComponent;
	virtual void Tick(float DeltaSeconds) override;

private:
	FVector Otolocation;
	int nCount;
	bool bWaterFlag;

protected:
	// 水中にいないときのアニメーション
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* NotSwimmingAnimation;

	// 水中にいるときのアニメーション
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* SwimmingAnimation;

	// アニメーション切替用
	void UpdateAnimation();

	void UpdateCharacter();

public:
	AOtosama();

	UPROPERTY(VisibleAnywhere)
		int nOtsamaPattern;

	// パーティクル
	void SetTonoPos(FVector location, bool bWatar);

	// 水ブロックに入った時
	void InWater();

	// 水ブロックから出たとき
	void OutWater();
};
