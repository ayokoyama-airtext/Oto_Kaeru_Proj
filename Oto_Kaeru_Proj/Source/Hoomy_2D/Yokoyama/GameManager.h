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
#define BLOCK_Y_COORD		60.0f	//	Y座標は奥行のパラメータ
#define BLOCK_MOVE_TIME		0.25f


UENUM()
enum class EBlockType : uint8
{
	EEmpty,
	EWall,
	EWater,
	EStart,
	EGoal,
	EMax,
};


USTRUCT()
struct FBlockInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	int	col;

	UPROPERTY(VisibleAnywhere)
	int row;
};



//-------------------------------------------------------------
// Name: AGameManager
// Desc: ゲーム管理クラス
//-------------------------------------------------------------
UCLASS()
class HOOMY_2D_API AGameManager : public AActor
{
	GENERATED_BODY()
	
private:	
	// Sets default values for this actor's properties
	AGameManager(const FObjectInitializer& ObjectInitializer);
	// Singleton
	static AGameManager*	instance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* インスタンスを取得 */
	UFUNCTION(BlueprintCallable)
	static AGameManager* GetInstance();

	/* ステージの状態を取得 */
	UFUNCTION(BlueprintCallable)
	int GetStageStatus(int col, int row) const;

	/* ステージの状態を設定 */
	UFUNCTION(BlueprintCallable)
	void SetStageStatus(int col, int row, EBlockType bt);

	/* ブロックが移動中かどうかを設定 */
	UFUNCTION(BlueprintCallable)
	void SetBlockMoving(bool bBlockMoving) { m_bBlockMoving = bBlockMoving; }

	///////////////////
	// 入力で呼ぶ関数
	///////////////////
	void LeftClickEvent();

protected:
	/* クリアしてるかを確認 */
	void CheckClear();

	/* ブロックを一つ一つ見ていく */
	bool CheckBlock(int x, int y, int* map, bool bFirstCheck);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStringTable	*m_pStringTable;		//	ストリングテーブルへの参照
	UPROPERTY(VisibleAnywhere)
	FText		m_ftStageText;			//	ストリングテーブルから読み込んだデータを保持(デバッグ用に表示)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	INT			m_iCol;					//	ステージの列数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	INT			m_iRow;					//	ステージの行数
	UPROPERTY(VisibleAnywhere)
	TArray<int>	m_StageArray;			//	ステージのデータ


	UPROPERTY(VisibleAnywhere)
	INT			m_iGoalNum;				//	ゴールの数
	UPROPERTY(VisibleAnywhere)
	INT			m_iClearedGoalNum;		//	条件を満たしたゴールの数
	UPROPERTY(VisibleAnywhere)
	FBlockInfo	m_StartBlock;			//	スタートブロック
	UPROPERTY(VisibleAnywhere)
	TArray<FBlockInfo>	m_GoalBlockArray;	//	ゴールブロック


	UPROPERTY(VisibleAnywhere)
	bool		m_bClearStage;			//	クリアしたかどうか


	UPROPERTY(EditAnywhere)
	TSubclassOf<class APaperSpriteActor>	m_BGBPRef;			//	背景アセットへの参照
	UPROPERTY(VisibleAnywhere)
	TArray<TSubclassOf<class ASuperBlock>>	m_BlocksRefArray;	//	ブロックアセットへの参照
	UPROPERTY(VisibleAnywhere)
	class ACameraActor*		m_pCamera;	//	ワールドのカメラ

	UPROPERTY(VisibleAnywhere)
	bool	m_bBlockMoving;		//	移動中のブロックがあるかどうか
};
