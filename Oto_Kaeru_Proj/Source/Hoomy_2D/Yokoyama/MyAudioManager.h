// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyAudioManager.generated.h"
//-------------------------------------------------------------
#define SE_MAX_NUM	3
#define BGM_MAX_NUM	1

UENUM(BlueprintType)
enum class ESEID : uint8
{
	EClearSE,
	EGameOverSE,
	EClickSE,
	ESE04,
	ESE05,
	ESE06,
	ESE07,
	ESE08,
	ESE09,
	ESE10,
	EMax,
};

UENUM(BlueprintType)
enum class EBGMID : uint8
{
	EBGM01,
	EBGM02,
	EMax,
};

//=============================================================

UCLASS()
class HOOMY_2D_API AMyAudioManager : public AActor
{
	GENERATED_BODY()
	
private:
	AMyAudioManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	static AMyAudioManager* instance;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		static AMyAudioManager* GetInstance();

	//
	//	�T�E���h�Đ�
	//
	UFUNCTION(BlueprintCallable)
		static void PlaySE(ESEID id, float volume = 1.0f, int ACNumber = 0);
	UFUNCTION(BlueprintCallable)
		static void PlayBGM(EBGMID id, float volume = 1.0f);

	/**
	*@brief	BGM���t�F�[�h�A�E�g������
	*@note	�t�F�[�h�A�E�g��ɍĐ�����BGM���w��\
	*/
	UFUNCTION(BlueprintCallable)
		static void FadeOutBGM(float fadeTime, bool bPlayOtherBGMAfterFading = false, EBGMID nextPlayBGMID = EBGMID::EBGM01, float volume = 1.f);

private:
	void PlaySE_inst(ESEID id, float volume, int ACNumber);
	void PlayBGM_inst(EBGMID id, float volume);
	void FadeOutBGM_inst(float fadeTime, bool bPlayOtherBGMAfterFading, EBGMID nextPlayBGMID, float volume);
	//	���\�[�X�擾

protected:
	//
	//	Audio Components(�I�[�f�B�I�t�@�C���̑����S��)
	//
	UPROPERTY(EditAnywhere)
		class UAudioComponent*	m_pSEAudioComponents[SE_MAX_NUM] = { nullptr };
	UPROPERTY(EditAnywhere)
		class UAudioComponent*	m_pBGMAudioComponents[BGM_MAX_NUM] = { nullptr };

	//
	//	Sound Waves(�����f�[�^)
	//
	UPROPERTY(VisibleAnywhere)
		class USoundBase*		m_pSESoundBases[(UINT)ESEID::EMax] = { nullptr };
	UPROPERTY(EditAnywhere)
		FString					m_pSEReferences[(UINT)ESEID::EMax] = { "" };

	UPROPERTY(VisibleAnywhere)
		class USoundBase*		m_pBGMSoundBases[(UINT)EBGMID::EMax] = { nullptr };
	UPROPERTY(EditAnywhere)
		FString					m_pBGMReferences[(UINT)EBGMID::EMax] = { "" };

	//
	//	Fade BGM
	//

	//	Fade�����ǂ���
	bool	m_bFading = false;
	//	Fade��ʂ�BGM���Đ����邩�ǂ���
	bool	m_bPlayBGMAfterFading = false;
	//	Fade��ɍĐ�����BGM��id
	EBGMID	m_eBGMIDAfterFading = EBGMID::EBGM01;
	//	Fade��ɍĐ�����BGM�̃{�����[��
	float	m_fBGMVolumeAfterFading = 0.f;
};
