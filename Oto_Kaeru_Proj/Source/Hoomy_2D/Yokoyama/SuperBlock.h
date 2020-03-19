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
	/* クリックされた時に呼ぶ関数 */
	virtual void Clicked(float mouseX, float mouseZ);

protected:
	/* 点が矩形内にあるか判定する */
	bool CheckPointInRect(float left, float bottom, float width, float height, float pointX, float pointY);

protected:
	UPROPERTY(VisibleAnywhere)
	int		m_iX;				//	X座標(ステージ配列上の座標)
	UPROPERTY(VisibleAnywhere)
	int		m_iY;				//	Y座標(ステージ配列上の座標)


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
};
