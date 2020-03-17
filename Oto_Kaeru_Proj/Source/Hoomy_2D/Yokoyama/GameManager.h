/**
* @file		GameManager.h
* @brief	ゲーム(ステージ)管理クラス
* @author	Akihiro Yokoyama
*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

//-------------------------------------------------------------
// Macros
//-------------------------------------------------------------
#define	BLOCK_SIZE			128.0f
#define BLOCK_Y_COORD		60.0f
#define BLOCK_MOVE_TIME		1.0f


UENUM()
enum class EBlockType : uint8
{
	EEmpty,
	EWall,
	EWater,
	EMax,
};



//-------------------------------------------------------------
// Name: AGameManager
// Desc: ゲーム管理クラス
//-------------------------------------------------------------
UCLASS()
class HOOMY_2D_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStringTable	*m_pStringTable;		//	ストリングテーブルへの参照
	UPROPERTY(VisibleAnywhere)
	FText		m_ftStageText;			//	ストリングテーブルから読み込んだデータを保持(デバッグ用に表示)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	INT			m_iCol;					//	ステージの行数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	INT			m_iRow;					//	ステージの列数
	UPROPERTY(VisibleAnywhere)
	TArray<int>	m_StageArray;			//	ステージのデータ
	UPROPERTY(EditAnywhere)
	TSubclassOf<class APaperSpriteActor>	m_BGBPRef;			//	背景アセットへの参照

	UPROPERTY(VisibleAnywhere)
	TArray<TSubclassOf<class ASuperBlock>>	m_BlocksRefArray;	//	ブロックアセットへの参照
	UPROPERTY(VisibleAnywhere)
	class ACameraActor*		m_pCamera;	//	ワールドのカメラ
};
