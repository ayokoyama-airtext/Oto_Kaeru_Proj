// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyEffectManager.generated.h"
//-------------------------------------------------------------
UENUM(BlueprintType)
enum class EParticleID : uint8
{
	EWaterSprush,
	EWaterTrail,
	EWaterDust,
	EOnp,
	EMax,
};


//=============================================================
UCLASS()
class HOOMY_2D_API AMyEffectManager : public AActor
{
	GENERATED_BODY()
	
private:
	// Sets default values for this actor's properties
	AMyEffectManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	static AMyEffectManager* instance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		static AMyEffectManager* GetInstance();

	/**
	*@brief		ParticleSystem���擾����
	*@note		�A�N�^�� ParticleSystemComponent �ɃZ�b�g���Ďg�p
	*/
	UFUNCTION(BlueprintCallable)
		static class UParticleSystem* GetParticleSystem(EParticleID id);

	/**
	*@brief		�p�[�e�B�N��(�����V���b�g)��P�ƂŐ���
	*@note		�p�[�e�B�N���͍Đ����I���Ύ������ł���
	*/
	UFUNCTION(BlueprintCallable)
		static void SpawnOneShotParticleEmitter(EParticleID id, FVector location, FRotator rotation, float scale = 1.0f);

	/**
	*@brief		�p�[�e�B�N��(���[�v)��P�ƂŐ���
	*@note		DestroyLoopParticleEmitters() ���Ă΂Ȃ��Ə����Ȃ�
	*/
	UFUNCTION(BlueprintCallable)
		static void SpawnLoopParticleEmitter(EParticleID id, FVector location, FRotator rotation, float scale = 1.0f);
	UFUNCTION(BlueprintCallable)
		static void DestroyLoopParticleEmitters();

protected:
	UPROPERTY(VisibleAnywhere)
		class UParticleSystem*	m_pParticleSystems[(UINT)EParticleID::EMax] = { nullptr };
	UPROPERTY(EditAnywhere)
		FString	m_pParticleReferances[(UINT)EParticleID::EMax];

private:
	//	Loop����ParticleSystem���Ǘ�����B�폜���ȊO�͎Q�Ƃ��Ȃ�����
	UPROPERTY()
		TArray<class UParticleSystemComponent*>	m_pLoopParticleComponents;

};
