// Fill out your copyright notice in the Description page of Project Settings.


#include "Otosama.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

AOtosama::AOtosama() : nOtsamaPattern(0)
{
	// �����ړ�������Ƃ����[���ړ����Ȃ��悤��
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// �R���W�����̃T�C�Y��ݒ�
	GetCapsuleComponent()->SetCapsuleHalfHeight(120.0f);
	GetCapsuleComponent()->SetCapsuleRadius(120.0f);

	// �L�����N�^�̎�����]��h�~
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// �ݒ�
	GetCharacterMovement()->GravityScale = 3.0f;

	// �L�����̓�����XZ�Ƀ��b�N
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Capsule�̃��[�e�[�V������h�~
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

	// Sprite�R���|�[�l���g�Ń��v���P�[�V������L���ɂ��āA�l�b�g���[�N�����ꂽ�Ƃ��ɃA�j���[�V�������\��������
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;
}

void AOtosama::UpdateAnimation()
{
	//const FVector PlayerVelocity = GetVelocity();
	//const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// ��Ԃ�NotSwim�ɂ���
	UPaperFlipbook* DesiredAnimation = (nOtsamaPattern > 0) ? NotSwimmingAnimation : SwimmingAnimation;
	if (GetSprite()->GetFlipbook() != DesiredAnimation)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

/*
* @brief ���t���[������
*/
void AOtosama::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateCharacter();
}

void AOtosama::InWater()
{
	GetSprite()->SetFlipbook(SwimmingAnimation);

}

void AOtosama::UpdateCharacter()
{
	//! �A�j���[�V�����ؑ�
	UpdateAnimation();

	// �ق��̏����������Ȃ�
}