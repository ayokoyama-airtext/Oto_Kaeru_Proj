/**
* @file		Otosama.cpp
* @brief	���̂���.cpp
* @author	yshimada
* @data		20200420
*/
#include "Otosama.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
//#include "Yokoyama/GameManager.h"
#include "Yokoyama/MyEffectManager.h"
#include "Yokoyama/MyAudioManager.h"

/*********************
* @brief �R���X�g���N�^
*********************/
AOtosama::AOtosama() : nOtsamaPattern(1), nCount(-1), bWaterFlag(false)
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

	GetSprite()->SetFlipbook(NotSwimmingAnimation);
}

/*
* @brief �A�j���[�V�����̕ύX
*/
void AOtosama::UpdateAnimation()
{
	//const FVector PlayerVelocity = GetVelocity();
	//const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	//// ��Ԃ��J�G��
	//UPaperFlipbook* DesiredAnimation = (nOtsamaPattern > 0) ? NotSwimmingAnimation : SwimmingAnimation;
	//if (GetSprite()->GetFlipbook() != DesiredAnimation)
	//{
	//	GetSprite()->SetFlipbook(DesiredAnimation);
	//}


}

/*********************
* @brief ���t���[������
*********************/
void AOtosama::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// �ŏ��̃e�B�b�N�ł̂݌Ă΂��
	if (!bWaterFlag && nCount == -1)
		AMyEffectManager::SpawnLoopParticleEmitter(EParticleID::EOnp, Otolocation, FRotator::ZeroRotator);

	if ((nCount % 100 == 0) && (nOtsamaPattern == 1)) {
		AMyAudioManager::PlaySE(ESEID::EClickSE);
	}

	nCount++;

	//UpdateCharacter();
}

/**
* @brief ���ɓ��������̏���
*/
void AOtosama::InWater()
{
	AMyEffectManager::DestroyLoopParticleEmitters();
	GetSprite()->SetFlipbook(SwimmingAnimation);
	UE_LOG(LogTemp, Warning, TEXT("Inwater"));
}

/**
* @brief ������o���Ƃ��̎��̏���
*/
void AOtosama::OutWater()
{
	AMyEffectManager::SpawnLoopParticleEmitter(EParticleID::EOnp, Otolocation, FRotator::ZeroRotator);
	GetSprite()->SetFlipbook(NotSwimmingAnimation);
}

/**
* @brief�@�G�t�F�N�g�p�|�W�V�����Z�b�g
*/
void AOtosama::SetTonoPos(FVector location, bool bWater)
{
	Otolocation = location;
	bWaterFlag = bWater;
	UE_LOG(LogTemp, Warning, TEXT("SetPos"));
}

/**
* @brief �A�j���[�V�����ؑ֎��Ăяo���֐�
*/
void AOtosama::UpdateCharacter()
{
	//! �A�j���[�V�����ؑ�
	//UpdateAnimation();

	// �ق��̏����������Ȃ�
}