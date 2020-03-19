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

protected:
	// �����ɂ��Ȃ��Ƃ��̃A�j���[�V����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* NotSwimmingAnimation;

	// �����ɂ���Ƃ��̃A�j���[�V����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* SwimmingAnimation;

	// �A�j���[�V�����ؑ֗p
	void UpdateAnimation();

	// ���u���b�N�ɓ�������
	void InWater();

	void UpdateCharacter();

public:
	AOtosama();

};
