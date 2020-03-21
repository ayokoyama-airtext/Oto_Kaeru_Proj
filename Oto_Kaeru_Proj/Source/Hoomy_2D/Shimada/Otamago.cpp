// Fill out your copyright notice in the Description page of Project Settings.


#include "Otamago.h"
#include "PaperFlipbookComponent.h"
#include "Components/TextRenderComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

AOtamago::AOtamago()
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
	GetCharacterMovement()->GravityScale = 0.0f;

	// キャラの動きをXZにロック
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Capsuleのローテーションを防止
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;

	// Spriteコンポーネントでレプリケーションを有効にして、ネットワーク化されたときにアニメーションが表示させる
	GetSprite()->SetIsReplicated(true);
	bReplicates = true;
}

void AOtamago::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// 状態をNotSwimにする
	UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? OtamagoAnimation : OtamaAnimation;
	if (GetSprite()->GetFlipbook() != DesiredAnimation)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

/*
* @brief 毎フレーム処理
*/
void AOtamago::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateCharacter();
}

void AOtamago::Otama()
{
	GetSprite()->SetFlipbook(OtamaAnimation);

}

void AOtamago::UpdateCharacter()
{
	//! アニメーション切替
	UpdateAnimation();

	// ほかの処理を書くなら
}