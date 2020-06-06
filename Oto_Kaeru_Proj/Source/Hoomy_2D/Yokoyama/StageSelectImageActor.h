/**
* @file		StageSelectImageActor.h
* @brief	ステージセレクトクラス
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
// Desc: ステージセレクト用のアクタクラス
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
	//	カーソルイベントに登録
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
	FString		m_LoadMapPath;		//	クリック時にロードするマップのパス

	//	アニメーション関連
	bool		m_bCursorOver;
	bool		m_bCursorEnd;
	float		m_fTimer;

	UPROPERTY(EditAnywhere)
	float		m_fExpansionRate;	//	カーソルオーバー時にどれくらい大きくするか
	UPROPERTY(EditAnywhere)
	float		m_fMatParamAmount;	//	マテリアルのパラメータに設定する値
	float		m_fDefaultYCoord;
	float		m_fExpansionYCoord;

	UPROPERTY()
	class UMaterialInstanceDynamic*	m_pMaterial_Instance;	//	マテリアルインスタンスへの参照

	UPROPERTY(EditAnywhere)
	class APaperSpriteActor*		m_pStageNameActor;		//	ステージ名のSpriteActorへの参照

	UPROPERTY(EditAnywhere)
		class AStageSelectManager*	m_pManager;
};
