/**
* @file		StageSelectImageActor.h
* @brief	�X�e�[�W�Z���N�g�N���X
* @author	Akihiro Yokoyama
*/
#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "StageSelectImageActor.generated.h"

//-------------------------------------------------------------
// Macros
//-------------------------------------------------------------
#define ANIMATION_TIME	0.5f



//-------------------------------------------------------------
// Name: AStageSelectImageActor
// Desc: �X�e�[�W�Z���N�g�p�̃A�N�^�N���X
//-------------------------------------------------------------
UCLASS()
class HOOMY_2D_API AStageSelectImageActor : public APaperSpriteActor
{
	GENERATED_BODY()

public:
	AStageSelectImageActor(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//
	//	�J�[�\���C�x���g�ɓo�^
	//
	UFUNCTION(BlueprintCallable)
	virtual void BeginCursorOver(UPrimitiveComponent* TouchedComponent);
	UFUNCTION(BlueprintCallable)
	virtual void EndCursorOver(UPrimitiveComponent* TouchedComponent);
	UFUNCTION(BlueprintCallable)
	virtual void Clicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);
	UFUNCTION(BlueprintCallable)
	FString GetLoadMapPath() { return m_LoadMapPath; }

protected:
	UPROPERTY(EditAnywhere)
	FString		m_LoadMapPath;		//	�N���b�N���Ƀ��[�h����}�b�v�̃p�X

	//	�A�j���[�V�����֘A
	bool		m_bCursorOver;
	bool		m_bCursorEnd;
	float		m_fTimer;

	UPROPERTY(EditAnywhere)
	float		m_fExpansionRate;	//	�J�[�\���I�[�o�[���ɂǂꂭ�炢�傫�����邩
	UPROPERTY(EditAnywhere)
	float		m_fMatParamAmount;	//	�}�e���A���̃p�����[�^�ɐݒ肷��l
	float		m_fDefaultYCoord;
	float		m_fExpansionYCoord;

	UPROPERTY()
	class UMaterialInstanceDynamic*	m_pMaterial_Instance;	//	�}�e���A���C���X�^���X�ւ̎Q��

	UPROPERTY(EditAnywhere)
	class APaperSpriteActor*		m_pStageNameActor;		//	�X�e�[�W����SpriteActor�ւ̎Q��

	UPROPERTY(EditAnywhere)
		class AStageSelectManager*	m_pManager;
};
