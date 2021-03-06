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
#define CAMERA_Y_COORD		1100.0f

/* 新しく追加するときは最後尾に追加すること */
UENUM()
enum class EBlockType : uint8
{
	EEmpty,
	EWall,
	EWater,
	EStart,
	EGoal,
	EWaterWall,
	EStartWithWater,
	EStone,
	EMax,
	EWithinSong = 0x40,
};


USTRUCT()
struct FBlockInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	int	col;

	UPROPERTY(VisibleAnywhere)
	int row;

	/*UPROPERTY(VisibleAnywhere)
	class APaperFlipbookActor*	Tonosama;

	UPROPERTY(VisibleAnywhere)
	class APaperFlipbookActor*	TonosamaInWater;

	UPROPERTY(VisibleAnywhere)
	class APaperFlipbookActor*	Tamago;

	UPROPERTY(VisibleAnywhere)
	class APaperFlipbookActor*	Otama;*/

	UPROPERTY(VisibleAnywhere)
	class AOtosama*	TonosamaBP;

	UPROPERTY(VisibleAnywhere)
	class AOtamago * TamagoBP;

	UPROPERTY(VisibleAnywhere)
	class ASuperBlock*			WaterBlock;

	/* Tonosama用変数 */
	UPROPERTY(VisibleAnywhere)
	bool bInWater;

	/* Tamago用変数 */
	UPROPERTY(VisibleAnywhere)
	bool bClear;

	FBlockInfo()
		:col(0),row(0),TonosamaBP(nullptr), TamagoBP(nullptr)/*,Tonosama(nullptr),TonosamaInWater(nullptr),Tamago(nullptr),Otama(nullptr)*/,WaterBlock(nullptr), bInWater(false), bClear(false)
	{
	}
};


USTRUCT()
struct FAreaDisplay
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	class APaperSpriteActor*	pAreaSprite;

	int	nLayer;

	FAreaDisplay()
		:pAreaSprite(nullptr), nLayer(-1)
	{
	}
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

	FVector PPos;

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

	/* オープニング演出の終了を通知 */
	UFUNCTION(BlueprintCallable)
	static void NotifyOpeningEnd() { instance->m_bOpeningEnd = true; };

	/* ステージの状態を取得 */
	UFUNCTION(BlueprintCallable)
	int GetStageStatus(int col, int row) const;

	/* ステージの状態を設定 */
	UFUNCTION(BlueprintCallable)
	void SetStageStatus(int col, int row, EBlockType bt);

	/* ブロックの参照を設定 */
	UFUNCTION(BlueprintCallable)
	void SetBlockStatus(int col, int row, class ASuperBlock* pBlock);

	/* ブロックが移動中かどうかを設定 */
	UFUNCTION(BlueprintCallable)
	void SetBlockMoving(bool bBlockMoving) { m_bBlockMoving = bBlockMoving; }

	/* クリック回数を増やす */
	UFUNCTION(BlueprintCallable)
	void IncreaseClickCount();

	UFUNCTION()
	int GetMaxClickCount() const { return m_iMaxClickNum; }

	/* カエルの状態を変える */
	UFUNCTION(BlueprintCallable)
	void ChangeOtonosamaState(bool bInWater);

	///////////////////
	// 入力で呼ぶ関数
	///////////////////
	void LeftClickEvent();

protected:
	/* クリアしてるかを確認(カエルが水中にいるとき) */
	void CheckClear();

	/* ブロックを一つ一つ見ていく。再帰関数 */
	bool CheckBlock(int x, int y, int* map, bool bFirstCheck);

	/* クリアしてるかを確認(カエルが水中にいないとき) */
	void CheckClearWithoutWater();

	/* ブロックを一つ一つ見ていく(水中にいないとき用) */
	bool CheckBlockWithoutWater(int x, int y);

	/* 歌範囲の設定を変更(オトノサマの周囲3マス) */
	void ChangeBlockStateWithinSong(bool bOn);

	/* トノサマの周囲の水ブロックを探索 */
	void CheckWaterBlockAroundTonosama();

	/* 範囲ブロックの位置計算とリスポーンを行う */
	void InitAreaDisplaySprites();

	/* 範囲ブロックの表示を切り替える */
	void SwitchAreaDisplay(bool bOn);

	/* 範囲ブロックを明滅させる */
	void BreathingAreaDisplay(float deltaTime);

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
	float		m_fWidth;				//	ステージ幅
	UPROPERTY(VisibleAnywhere)
	float		m_fHeight;				//	ステージ高さ
	UPROPERTY(VisibleAnywhere)
	TArray<int>	m_StageArray;			//	ステージのデータ
	UPROPERTY(VisibleAnywhere)
	TArray<class ASuperBlock*>	m_BlockArray;	//	スポーンしたブロックへの参照配列

	float	m_fTimer;	//	タイマー

	UPROPERTY(VisibleAnywhere)
	INT			m_iGoalNum;				//	ゴールの数
	UPROPERTY(VisibleAnywhere)
	INT			m_iClearedGoalNum;		//	条件を満たしたゴールの数
	UPROPERTY(VisibleAnywhere)
	FBlockInfo	m_StartBlock;			//	スタートブロック
	UPROPERTY(VisibleAnywhere)
	TArray<FBlockInfo>	m_GoalBlockArray;	//	ゴールブロック


	UPROPERTY(VisibleAnywhere)
	INT			m_iMaxClickNum;			//	このステージでクリックできる数
	UPROPERTY(VisibleAnywhere)
	INT			m_iClickCount;			//	現在のクリック数
	UPROPERTY(VisibleAnywhere)
	bool		m_bClearStage;			//	クリアしたかどうか
	UPROPERTY(VisibleAnywhere)
	bool		m_bGameOver;			//	ゲームオーバーしたかどうか

	int			m_iEndingPhase;			//	クリア・ゲームオーバー処理用

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameUserWidget>	m_WidgetBPRef;		//	Widgetアセットへの参照
	UPROPERTY(VisibleAnywhere)
	class UGameUserWidget*				m_pWidget;			//	Widgetインスタンスのポインタ


	UPROPERTY(EditAnywhere)
	TSubclassOf<class APaperSpriteActor>	m_BGBPRef;			//	背景アセットへの参照
	UPROPERTY(EditAnywhere)
	TSubclassOf<class APaperSpriteActor>	m_BGEdgeRef;		//	枠アセットへの参照
	UPROPERTY(VisibleAnywhere)
	TArray<TSubclassOf<class ASuperBlock>>	m_BlocksRefArray;	//	ブロックアセットへの参照

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class APaperFlipbookActor>	m_TonosamaRef;	//	とのさまアセットへの参照
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class APaperFlipbookActor>	m_TonosamaInWaterRef;	//	とのさまアセットへの参照
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class APaperFlipbookActor>	m_TamagoRef;	//	たまごアセットへの参照
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class APaperFlipbookActor>	m_OtamaRef;		//	おたまアセットへの参照

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class AOtosama>	m_TonosamaBPRef;
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class AOtamago>		m_TamagoBPRef;

	UPROPERTY(VisibleAnywhere)
	class ACameraActor*		m_pCamera;	//	ワールドのカメラ

	UPROPERTY(VisibleAnywhere)
	bool	m_bBlockMoving;		//	移動中のブロックがあるかどうか
	UPROPERTY(VisibleAnywhere)
	bool	m_bOpeningEnd;		//	オープニングが終わったかどうか

	//	ゲームクリア　or ゲームオーバーレベルに渡す情報
	UPROPERTY(EditAnywhere)
		FString currentStage;
	UPROPERTY(EditAnywhere)
		FString nextStage;

	//	範囲表示用
	UPROPERTY(VisibleAnywhere)
		TArray<FAreaDisplay>	m_AreaDisplaySprites;		//	範囲表示用BP
	UPROPERTY(EditAnywhere)
		TSubclassOf<class APaperSpriteActor>	m_AreaSpriteBPRef;			//	範囲表示用BPアセットへの参照
	float	m_fBreathingTimer;		//	範囲表示点滅用タイマー

	bool	m_bFirst;
};
