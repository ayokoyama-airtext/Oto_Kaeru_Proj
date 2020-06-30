/**
* @file		Otosama.cpp
* @brief	音のさま.cpp
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
* @brief コンストラクタ
*********************/
AOtosama::AOtosama() : nOtsamaPattern(1), nCount(-1), bWaterFlag(false)
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

/*
* @brief アニメーションの変更
*/
void AOtosama::UpdateAnimation()
{
	//const FVector PlayerVelocity = GetVelocity();
	//const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	//// 状態をカエル
	//UPaperFlipbook* DesiredAnimation = (nOtsamaPattern > 0) ? NotSwimmingAnimation : SwimmingAnimation;
	//if (GetSprite()->GetFlipbook() != DesiredAnimation)
	//{
	//	GetSprite()->SetFlipbook(DesiredAnimation);
	//}


}

/*********************
* @brief 毎フレーム処理
*********************/
void AOtosama::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// 最初のティックでのみ呼ばれる
	if (!bWaterFlag && nCount == -1)
		AMyEffectManager::SpawnLoopParticleEmitter(EParticleID::EOnp, Otolocation, FRotator::ZeroRotator);

	if ((nCount % 100 == 0) && (nOtsamaPattern == 1)) {
		AMyAudioManager::PlaySE(ESEID::EClickSE);
	}

	nCount++;

	//UpdateCharacter();
}

/**
* @brief 水に入った時の処理
*/
void AOtosama::InWater()
{
	AMyEffectManager::DestroyLoopParticleEmitters();
	GetSprite()->SetFlipbook(SwimmingAnimation);
	UE_LOG(LogTemp, Warning, TEXT("Inwater"));
}

/**
* @brief 水から出たときの時の処理
*/
void AOtosama::OutWater()
{
	AMyEffectManager::SpawnLoopParticleEmitter(EParticleID::EOnp, Otolocation, FRotator::ZeroRotator);
	GetSprite()->SetFlipbook(NotSwimmingAnimation);
}

/**
* @brief　エフェクト用ポジションセット
*/
void AOtosama::SetTonoPos(FVector location, bool bWater)
{
	Otolocation = location;
	bWaterFlag = bWater;
	UE_LOG(LogTemp, Warning, TEXT("SetPos"));
}

/**
* @brief アニメーション切替時呼び出し関数
*/
void AOtosama::UpdateCharacter()
{
	//! アニメーション切替
	//UpdateAnimation();

	// ほかの処理を書くなら
}