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
	*@brief		ParticleSystemを取得する
	*@note		アクタの ParticleSystemComponent にセットして使用
	*/
	UFUNCTION(BlueprintCallable)
		static class UParticleSystem* GetParticleSystem(EParticleID id);

	/**
	*@brief		パーティクル(ワンショット)を単独で生成
	*@note		パーティクルは再生が終われば自動消滅する
	*/
	UFUNCTION(BlueprintCallable)
		static void SpawnOneShotParticleEmitter(EParticleID id, FVector location, FRotator rotation, float scale = 1.0f);

	/**
	*@brief		パーティクル(ループ)を単独で生成
	*@note		DestroyLoopParticleEmitters() を呼ばないと消えない
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
	//	LoopするParticleSystemを管理する。削除時以外は参照しないこと
	UPROPERTY()
		TArray<class UParticleSystemComponent*>	m_pLoopParticleComponents;

};
