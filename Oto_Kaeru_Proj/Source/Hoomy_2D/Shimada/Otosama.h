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
	// �����ɂ��Ȃ��Ƃ��̃A�j���[�V����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* NotSwimmingAnimation;

	// �����ɂ���Ƃ��̃A�j���[�V����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* SwimmingAnimation;

	// �A�j���[�V�����ؑ֗p
	void UpdateAnimation();

	void UpdateCharacter();

public:
	AOtosama();

	UPROPERTY(VisibleAnywhere)
		int nOtsamaPattern;

	// �p�[�e�B�N��
	void SetTonoPos(FVector location);

	// ���u���b�N�ɓ�������
	void InWater();

	// ���u���b�N����o���Ƃ�
	void OutWater();
};
