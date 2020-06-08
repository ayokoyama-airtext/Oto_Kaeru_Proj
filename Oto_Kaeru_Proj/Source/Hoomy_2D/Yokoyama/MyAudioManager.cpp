// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAudioManager.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
//-------------------------------------------------------------
AMyAudioManager* AMyAudioManager::instance = nullptr;

//=============================================================
// Sets default values
AMyAudioManager::AMyAudioManager(const FObjectInitializer& ObjectInitializer)
	: AActor(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	//	Create Audio Components
	for (int i = 0; i < SE_MAX_NUM; ++i)
	{
		FString str = FString::Printf(TEXT("SEAudioComponent%d"), i);
		FName name = FName(*str);
		UAudioComponent* ac = CreateDefaultSubobject<UAudioComponent>(name);
		if (ac)
		{
			ac->SetupAttachment(RootComponent);
			ac->SetRelativeLocation(FVector(0, 0, 0));
			ac->bAutoActivate = false;
			m_pSEAudioComponents[i] = ac;
		}
	}
	for (int i = 0; i < BGM_MAX_NUM; ++i)
	{
		FString str = FString::Printf(TEXT("BGMAudioComponent%d"), i);
		FName name = FName(*str);
		UAudioComponent* ac = CreateDefaultSubobject<UAudioComponent>(name);
		if (ac)
		{
			ac->SetupAttachment(RootComponent);
			ac->SetRelativeLocation(FVector(0, 0, 0));
			ac->bAutoActivate = false;
			m_pBGMAudioComponents[i] = ac;
		}
	}
}

//=============================================================
// Called when the game starts or when spawned
void AMyAudioManager::BeginPlay()
{
	Super::BeginPlay();

	if (instance == nullptr)
	{
		instance = this;
	}

	//	Create Sound Bases
	for (int i = 0; i < (int)ESEID::EMax; ++i)
	{
		if (!m_pSEReferences[i].IsEmpty())
		{
			const TCHAR* str = *m_pSEReferences[i];
			USoundBase* sb = LoadObject<USoundBase>(nullptr, str);
			if (sb != nullptr)
			{
				m_pSESoundBases[i] = sb;
			}
		}
	}
	for (int i = 0; i < (int)EBGMID::EMax; ++i)
	{
		if (!m_pBGMReferences[i].IsEmpty())
		{
			const TCHAR* str = *m_pBGMReferences[i];
			USoundBase* sb = LoadObject<USoundBase>(nullptr, str);
			if (sb != nullptr)
			{
				m_pBGMSoundBases[i] = sb;
			}
		}
	}
}

//=============================================================
void AMyAudioManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	instance = nullptr;
}

//=============================================================
// Called every frame
void AMyAudioManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//	フェード処理
	if (m_bFading)
	{
		if (!m_pBGMAudioComponents[0]->IsPlaying())
		{
			m_bFading = false;
			if (m_bPlayBGMAfterFading)
			{
				USoundBase* sb = m_pBGMSoundBases[(int)m_eBGMIDAfterFading];
				if (sb != nullptr)
				{
					UAudioComponent* ac = m_pBGMAudioComponents[0];
					ac->SetSound(sb);
					ac->SetVolumeMultiplier(m_fBGMVolumeAfterFading);
					ac->Play();
				}
				m_bPlayBGMAfterFading = false;
				m_eBGMIDAfterFading = EBGMID::EBGM01;
				m_fBGMVolumeAfterFading = 0.f;
			}
		}
	}
}

//=============================================================
AMyAudioManager* AMyAudioManager::GetInstance()
{
	if (instance == nullptr)
	{
		if (GEngine->GameViewport != nullptr)
		{
			UWorld* world = GEngine->GameViewport->GetWorld();
			TSubclassOf<AMyAudioManager> findclass = AMyAudioManager::StaticClass();
			TArray<AActor*> actors;

			UGameplayStatics::GetAllActorsOfClass(world, findclass, actors);
			if (actors.Num())
			{
				instance = Cast<AMyAudioManager>(actors[0]);
			}
		}
	}

	return instance;
}

//=============================================================
//
//	再生
//

//	SE
void AMyAudioManager::PlaySE(ESEID id, float volume, int ACNumber)
{
	if (instance == nullptr)
	{
		GetInstance();
		if (instance == nullptr)
		{
			return;
		}
	}

	instance->PlaySE_inst(id, volume, ACNumber);
}
void AMyAudioManager::PlaySE_inst(ESEID id, float vol, int ACNumber)
{
	USoundBase* sb = m_pSESoundBases[(int)id];
	if (sb == nullptr)
	{
		return;
	}

	int ACNum = (ACNumber >= SE_MAX_NUM) ? (SE_MAX_NUM - 1) : (ACNumber < 0) ? (0) : ACNumber;

	UAudioComponent* ac = m_pSEAudioComponents[ACNum];
	ac->SetSound(sb);
	ac->SetVolumeMultiplier(vol);
	ac->Play();
}

//	BGM
void AMyAudioManager::PlayBGM(EBGMID id, float vol)
{
	if (instance == nullptr)
	{
		GetInstance();
		if (instance == nullptr)
		{
			return;
		}
	}

	if (instance->m_bFading)
	{
		return;
	}

	instance->PlayBGM_inst(id, vol);
}
void AMyAudioManager::PlayBGM_inst(EBGMID id, float vol)
{
	USoundBase* sb = m_pBGMSoundBases[(int)id];
	if (sb == nullptr)
	{
		return;
	}

	UAudioComponent* ac = m_pBGMAudioComponents[0];
	ac->SetSound(sb);
	ac->SetVolumeMultiplier(vol);
	ac->Play();
}

/**
*@brief	BGMをフェードアウトさせる
*@note	フェードアウト後に再生するBGMを指定可能
*/
void AMyAudioManager::FadeOutBGM(float fadeTime, bool bPlayOtherBGMAfterFading, EBGMID nextPlayBGMID, float volume)
{
	if (instance == nullptr)
	{
		GetInstance();
		if (instance == nullptr)
		{
			return;
		}
	}

	if (!(instance->m_pBGMAudioComponents[0]->IsPlaying()))
	{
		return;
	}

	if (instance->m_bFading)
	{
		return;
	}

	instance->m_bFading = true;
	instance->FadeOutBGM_inst(fadeTime, bPlayOtherBGMAfterFading, nextPlayBGMID, volume);
}
void AMyAudioManager::FadeOutBGM_inst(float fadeTime, bool bPlayOtherBGMAfterFading, EBGMID nextPlayBGMID, float volume)
{
	m_pBGMAudioComponents[0]->FadeOut(fadeTime, 0.f);
	if (bPlayOtherBGMAfterFading)
	{
		m_bPlayBGMAfterFading = true;
		m_eBGMIDAfterFading = nextPlayBGMID;
		m_fBGMVolumeAfterFading = volume;
	}
}