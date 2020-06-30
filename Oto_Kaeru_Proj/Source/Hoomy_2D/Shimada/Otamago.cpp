/**
* @file		Otamago.cpp
* @brief	�I�^�}�W���N�V.cpp
* @author	yshimada
* @data		20200420
*/
#include "Otamago.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

/*********************
* @brief �R���X�g���N�^
*********************/
AOtamago::AOtamago() : nOtamaPattern(1)
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
	GetCharacterMovement()->GravityScale = 0.0f;

	// �L�����̓�����XZ�Ƀ��b�N
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Capsule�̃��[�e�[�V������h�~
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

	// Sprite�R���|�[�l���g�Ń��v���P�[�V������L���ɂ��āA�l�b�g���[�N�����ꂽ�Ƃ��ɃA�j���[�V�������\��������
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;

	GetSprite()->SetFlipbook(OtamagoAnimation);
}

/*********************
* @brief �A�b�v�f�[�g
*********************/
void AOtamago::UpdateAnimation()
{
	//const FVector PlayerVelocity = GetVelocity();
	//PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// ���݂̂���ׂ��X�v���C�g��ǂݎ���r���ăZ�b�g
	UPaperFlipbook* DesiredAnimation = (nOtamaPattern > 0) ? OtamagoAnimation : OtamaAnimation;
	if (GetSprite()->GetFlipbook() != DesiredAnimation)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}

}

/*********************
* @brief ���t���[������
*********************/
void AOtamago::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//UpdateCharacter();
}

/*
* @brief �I�^�}�W���N�V�ɂ���
*/
void AOtamago::Otama()
{
	GetSprite()->SetFlipbook(OtamaAnimation);

}

/**
* @brief �L�����ύX�Ăяo��
*/
void AOtamago::UpdateCharacter()
{
	//! �A�j���[�V�����ؑ�
	UpdateAnimation();

	// �ق��̏����������Ȃ�
}