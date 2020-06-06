// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEffectManager.h"
#include "Engine/GameEngine.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleSystemComponent.h"
//-------------------------------------------------------------
AMyEffectManager* AMyEffectManager::instance = nullptr;

//=============================================================
// Sets default values
AMyEffectManager::AMyEffectManager(const FObjectInitializer& ObjectInitializer)
	: AActor(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
}

//=============================================================
// Called when the game starts or when spawned
void AMyEffectManager::BeginPlay()
{
	Super::BeginPlay();

	if (instance == nullptr)
	{
		instance = this;
	}

	int _num = (int)EParticleID::EMax;

	for (int i = 0; i < _num; ++i)
	{
		const TCHAR* chars = *m_pParticleReferances[i];
		UParticleSystem* ps = LoadObject<UParticleSystem>(nullptr, chars);
		if (ps != nullptr)
		{
			m_pParticleSystems[i] = ps;
		}
	}
}

//=============================================================
void AMyEffectManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	instance = nullptr;
}

//=============================================================
// Called every frame
void AMyEffectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//=============================================================
AMyEffectManager* AMyEffectManager::GetInstance()
{
	if (instance == nullptr)
	{
		if (GEngine->GameViewport != nullptr)
		{
			UWorld* world = GEngine->GameViewport->GetWorld();
			TSubclassOf<AMyEffectManager> findclass = AMyEffectManager::StaticClass();
			TArray<AActor*> actors;

			UGameplayStatics::GetAllActorsOfClass(world, findclass, actors);
			if (actors.Num())
			{
				instance = Cast<AMyEffectManager>(actors[0]);
			}
		}
	}

	return instance;
}

//=============================================================
/**
*@brief		ParticleSystem���擾����
*@note		�A�N�^�� ParticleSystemComponent �ɃZ�b�g���Ďg�p
*/
UParticleSystem* AMyEffectManager::GetParticleSystem(EParticleID id)
{
	if (instance == nullptr)
	{
		GetInstance();
		if (instance == nullptr)
		{
			return nullptr;
		}
	}

	int _index = (int)id;
	return instance->m_pParticleSystems[_index];
}

//=============================================================
/**
*@brief		�p�[�e�B�N��(�����V���b�g)��P�ƂŐ���
*@note		�p�[�e�B�N���͍Đ����I���Ύ������ł���
*/
void AMyEffectManager::SpawnOneShotParticleEmitter(EParticleID id, FVector location, FRotator rotation, float scale)
{
	if (instance == nullptr)
	{
		GetInstance();
		if (instance == nullptr)
		{
			return;
		}
	}


	int _index = (int)id;
	UWorld* _world = instance->GetWorld();
	UParticleSystem* _ps = instance->m_pParticleSystems[_index];
	FVector _vecScale = FVector(scale, scale, scale);
	UGameplayStatics::SpawnEmitterAtLocation(_world, _ps, location, rotation, _vecScale);
}

//=============================================================
/**
*@brief		�p�[�e�B�N��(���[�v)��P�ƂŐ���
*@note		DestroyLoopParticleEmitters() ���Ă΂Ȃ��Ə����Ȃ�
*/
void AMyEffectManager::SpawnLoopParticleEmitter(EParticleID id, FVector location, FRotator rotation, float scale)
{
	if (instance == nullptr)
	{
		GetInstance();
		if (instance == nullptr)
		{
			return;
		}
	}


	int _index = (int)id;
	UWorld* _world = instance->GetWorld();
	UParticleSystem* _ps = instance->m_pParticleSystems[_index];
	FVector _vecScale = FVector(scale, scale, scale);
	UParticleSystemComponent* psc = UGameplayStatics::SpawnEmitterAtLocation(_world, _ps, location, rotation, _vecScale, false);

	if (psc != nullptr)
	{
		instance->m_pLoopParticleComponents.Add(psc);
	}
}

void AMyEffectManager::DestroyLoopParticleEmitters()
{
	if (instance == nullptr)
	{
		GetInstance();
		if (instance == nullptr)
		{
			return;
		}
	}

	int _num = instance->m_pLoopParticleComponents.Num();
	for (int i = 0; i < _num; ++i)
	{
		instance->m_pLoopParticleComponents[i]->DestroyComponent();
	}
	instance->m_pLoopParticleComponents.Empty();
}