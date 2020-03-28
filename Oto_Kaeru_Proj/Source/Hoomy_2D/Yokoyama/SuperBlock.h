/**
* @file		SuperBlock.h
* @brief	ブロックのスーパークラス
* @author	Akihiro Yokoyama
*/
#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "GameManager.h"
#include "SuperBlock.generated.h"

//-------------------------------------------------------------
// Macros
//-------------------------------------------------------------
USTRUCT()
struct FBlockMoveInfo
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
		bool bMoveVertical;

	UPROPERTY(VisibleAnywhere)
		bool bMoveHorizontal;

	UPROPERTY(VisibleAnywhere)
		int	iMoveDirX;

	UPROPERTY(VisibleAnywhere)
		int iMoveDirY;

public:
		void Init(int characode) {
		switch (characode)
		{
		case L'u':
			bMoveVertical = true;		iMoveDirY = -1;
			bMoveHorizontal = false;	iMoveDirX = 0;
			break;
		case L'd':
			bMoveVertical = true;		iMoveDirY = 1;
			bMoveHorizontal = false;	iMoveDirX = 0;
			break;
		case L'r':
			bMoveVertical = false;		iMoveDirY = 0;
			bMoveHorizontal = true;		iMoveDirX = 1;
			break;
		case L'l':
			bMoveVertical = false;		iMoveDirY = 0;
			bMoveHorizontal = true;		iMoveDirX = -1;
			break;
		default:
			break;
		}
	}
		bool IsMovingVertical() const { return bMoveVertical; }
		bool IsMovingHorizontal() const { return bMoveHorizontal; }
		int GetMoveDirX() const { return iMoveDirX; }
		int GetMoveDirY() const { return iMoveDirY; }
		void ReverseMoveDir() {
		iMoveDirX = (bMoveHorizontal) ? iMoveDirX * -1 : 0;
		iMoveDirY = (bMoveVertical) ? iMoveDirY * -1 : 0;
	}
};




//-------------------------------------------------------------
// Name: ASuperBlock
// Desc: 
//-------------------------------------------------------------
UCLASS()
class HOOMY_2D_API ASuperBlock : public APaperSpriteActor
{
	GENERATED_BODY()
	
public:
	ASuperBlock(const FObjectInitializer& ObjectInitializer);
	virtual ~ASuperBlock();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	//
	//	共通メソッド
	//

	/* 座標を設定 */
	virtual void SetPosition(int x, int y) { m_iX = x; m_iY = y; }
	/* 親を設定 */
	virtual void SetParent(class AGameManager* pParent) { m_pParent = pParent; }
	/* 移動情報を設定 */
	virtual void SetMoveInfo(int charaCode) { if (m_bMovable) m_MoveInfo.Init(charaCode); }
	/* クリックされた時に呼ぶ関数 */
	virtual void Clicked(float mouseX, float mouseZ);
	/* 移動可能かどうか変更 */
	virtual void SetMovePossibility(bool bMove) { m_bMovable = bMove; }

	//
	//	カーソルオーバーイベントに登録
	//
	UFUNCTION(BlueprintCallable)
	virtual void BeginCursorOver(UPrimitiveComponent* TouchedComponent);
	UFUNCTION(BlueprintCallable)
	virtual void EndCursorOver(UPrimitiveComponent* TouchedComponent);

protected:
	/* 点が矩形内にあるか判定する */
	bool CheckPointInRect(float left, float bottom, float width, float height, float pointX, float pointY);

protected:
	UPROPERTY(VisibleAnywhere)
	int		m_iX;				//	X座標(ステージ配列上の座標)
	UPROPERTY(VisibleAnywhere)
	int		m_iY;				//	Y座標(ステージ配列上の座標)

	UPROPERTY(VisibleAnywhere)
	FBlockMoveInfo	m_MoveInfo;


	UPROPERTY(VisibleAnywhere)
	int		m_iMoveDirX;		//	X移動方向(ステージ配列上の座標)
	UPROPERTY(VisibleAnywhere)
	int		m_iMoveDirY;		//	Y移動方向(ステージ配列上の座標)
	UPROPERTY(VisibleAnywhere)
	int		m_iDestX;			//	移動先目的X座標(ステージ配列上の座標)
	UPROPERTY(VisibleAnywhere)
	int		m_iDestY;			//	移動先目的Y座標(ステージ配列上の座標)
	UPROPERTY(VisibleAnywhere)
	float	m_fDestWorldX;		//	移動先目的X座標(ワールド上の座標)
	UPROPERTY(VisibleAnywhere)
	float	m_fDestWorldZ;		//	移動先目的Z座標(ワールド上の座標)
	UPROPERTY(VisibleAnywhere)
	float	m_fStartWorldX;		//	移動開始X座標(ワールド上の座標)
	UPROPERTY(VisibleAnywhere)
	float	m_fStartWorldZ;		//	移動開始Z座標(ワールド上の座標)
	UPROPERTY(VisibleAnywhere)
	bool	m_bMoving;			//	移動中かどうか
	UPROPERTY(VisibleAnywhere)
	float	m_fTimer;			//	移動用タイマー
	UPROPERTY(VisibleAnywhere)
	float	m_fMoveTime;		//	移動にかける時間

	UPROPERTY(VisibleAnywhere)
	class AGameManager*		m_pParent;	//	同じワールド内のGameManagerへの参照

	//	継承先で初期化設定
	UPROPERTY(VisibleAnywhere)
	bool	m_bMovable;			//	移動可能ブロックかどうか
	UPROPERTY(VisibleAnywhere)
	EBlockType	m_eBlockType;	//	このブロックのタイプ

	//	矢印のアクタ
	UPROPERTY(VisibleAnywhere)
	APaperSpriteActor*		m_pChildArrowActor;
};
