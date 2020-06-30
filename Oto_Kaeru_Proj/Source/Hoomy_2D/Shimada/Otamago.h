/**
* @file		Otamago.h
* @brief	�I�^�}�W���N�V.h
* @author	yshimada
* @data		20200420
*/
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

	void UpdateCharacter();

public:
	AOtamago();

	UPROPERTY(VisibleAnywhere)
		int nOtamaPattern;

	// ���u���b�N�ɓ�������
	void Otama();

};
