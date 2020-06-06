// Fill out your copyright notice in the Description page of Project Settings.

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

	// 水ブロックに入った時
	void Otama();

	void UpdateCharacter();

public:
	AOtamago();

	UPROPERTY(VisibleAnywhere)
	int nOtamaPattern;
};
