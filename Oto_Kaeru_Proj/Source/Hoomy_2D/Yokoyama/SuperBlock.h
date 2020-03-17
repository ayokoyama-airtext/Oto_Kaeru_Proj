/**
* @file		SuperBlock.h
* @brief	ブロックのスーパークラス
* @author	Akihiro Yokoyama
*/
#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
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
	//
	//	共通メソッド
	//

	/* 座標を設定 */
	virtual void SetPosition(int x, int y) { m_iX = x; m_iY = y; }
	/* 親を設定 */
	virtual void SetParent(class AGameManager* pParent) { m_pParent = pParent; }


protected:
	UPROPERTY(VisibleAnywhere)
	int		m_iX;	//	X座標
	UPROPERTY(VisibleAnywhere)
	int		m_iY;	//	Y座標
	UPROPERTY(VisibleAnywhere)
	bool	m_bMovable;		//	移動可能ブロックかどうか

	UPROPERTY(VisibleAnywhere)
	class AGameManager*		m_pParent;	//	同じワールド内のGameManagerへの参照
};
