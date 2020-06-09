// Fill out your copyright notice in the Description page of Project Settings.


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


AOtosama::AOtosama() : nOtsamaPattern(1), nCount(0)
{

	// もし移動させるときヨーが移動しないように
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// コリジョンのサイズを設定
	GetCapsuleComponent()->SetCapsuleHalfHeight(120.0f);
	GetCapsuleComponent()->SetCapsuleRadius(120.0f);

	// キャラクタの自動回転を防止
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// 設定
	GetCharacterMovement()->GravityScale = 3.0f;

	// キャラの動きをXZにロック
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Capsuleのローテーションを防止
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

	// Spriteコンポーネントでレプリケーションを有効にして、ネットワーク化されたときにアニメーションが表示させる
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;

	GetSprite()->SetFlipbook(NotSwimmingAnimation);
}

void AOtosama::UpdateAnimation()
{
	//const FVector PlayerVelocity = GetVelocity();
	//const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// 状態をカエル
	UPaperFlipbook* DesiredAnimation = (nOtsamaPattern > 0) ? NotSwimmingAnimation : SwimmingAnimation;
	if (GetSprite()->GetFlipbook() != DesiredAnimation)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}

	if ((nCount % 100 == 0) && (nOtsamaPattern == 1)) {
		AMyAudioManager::PlaySE(ESEID::EClickSE);
	}
}

/*
* @brief 毎フレーム処理
*/
void AOtosama::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//UpdateCharacter();
}

void AOtosama::InWater()
{
	GetSprite()->SetFlipbook(SwimmingAnimation);
}

void AOtosama::OutWater()
{
	GetSprite()->SetFlipbook(NotSwimmingAnimation);
}

void AOtosama::SetTonoPos(FVector location)
{
	Otolocation = location;
	AMyEffectManager::SpawnLoopParticleEmitter(EParticleID::EOnp, Otolocation, FRotator::ZeroRotator);
}

void AOtosama::UpdateCharacter()
{
	//! アニメーション切替
	//UpdateAnimation();

	// ほかの処理を書くなら
}