// Fill out your copyright notice in the Description page of Project Settings.

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
	void SetTonoPos(FVector location);

	// 水ブロックに入った時
	void InWater();

	// 水ブロックから出たとき
	void OutWater();
};
