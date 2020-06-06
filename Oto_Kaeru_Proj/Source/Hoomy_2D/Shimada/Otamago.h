// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "Otamago.generated.h"

class UTextRenderComponent;

/**
 * @broef ���J�G��
 */
UCLASS(Config = Game)
class HOOMY_2D_API AOtamago : public APaperCharacter
{
	GENERATED_BODY()
	
		UTextRenderComponent* TextComponent;
	virtual void Tick(float DeltaSeconds) override;

protected:
	// ���̎��̃A�j���[�V����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* OtamagoAnimation;

	// �I�^�}�W���N�V
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animations)
		class UPaperFlipbook* OtamaAnimation;

	// �A�j���[�V�����ؑ֗p
	void UpdateAnimation();

	// ���u���b�N�ɓ�������
	void Otama();

	void UpdateCharacter();

public:
	AOtamago();

	UPROPERTY(VisibleAnywhere)
	int nOtamaPattern;
};
