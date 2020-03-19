// Fill out your copyright notice in the Description page of Project Settings.


#include "SuperBlock.h"
#include "PaperSprite.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"


//-------------------------------------------------------------
// Name: ASuperBlock()
// Desc: Ctor
//-------------------------------------------------------------
ASuperBlock::ASuperBlock(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer), m_iX(0), m_iY(0), m_pParent(nullptr), m_fDestWorldX(0), m_fDestWorldZ(0), m_bMoving(false), m_fTimer(0), m_iMoveDirX(0), m_iMoveDirY(0), m_fMoveTime(BLOCK_MOVE_TIME), m_fStartWorldX(0), m_fStartWorldZ(0)
{
	UPaperSpriteComponent* pRenderComp = GetRenderComponent();
	if (pRenderComp != NULL)
	{
		RootComponent = pRenderComp;
	}
	else
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	}

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}



//-------------------------------------------------------------
// Name: ~ASuperBlock()
// Desc: Dtor
//-------------------------------------------------------------
ASuperBlock::~ASuperBlock()
{
	
}



//-------------------------------------------------------------
// Name: BeginPlay()
// Desc: ゲーム開始直前に呼ばれる初期化処理
//-------------------------------------------------------------
void ASuperBlock::BeginPlay()
{
	Super::BeginPlay();

}



//-------------------------------------------------------------
// Name: Tick()
// Desc: 毎フレーム呼ばれる関数
// Parm: DeltaTime / 前フレームからの経過時間
//-------------------------------------------------------------
void ASuperBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//	ブロック移動処理
	if (m_bMoving)
	{
		m_fTimer += DeltaTime;
		float rate = 0;

		if (m_fTimer >= m_fMoveTime)
		{
			int destPosStatus = m_pParent->GetStageStatus(m_iDestX + m_iMoveDirX, m_iDestY + m_iMoveDirY);
			if (destPosStatus != -1 && destPosStatus == (int)EBlockType::EEmpty)
			{
				m_iDestX += m_iMoveDirX;
				m_iDestY += m_iMoveDirY;
				m_fStartWorldX = m_fDestWorldX;
				m_fStartWorldZ = m_fDestWorldZ;
				m_fDestWorldX += BLOCK_SIZE * m_iMoveDirX;
				m_fDestWorldZ += BLOCK_SIZE * (-m_iMoveDirY);	//	ワールド座標と配列上の座標の縦軸は反転している
				m_fTimer -= BLOCK_MOVE_TIME;
			}
			else
			{
				m_fTimer = m_fMoveTime;
				m_bMoving = false;
			}
		}

		
		rate = m_fTimer * (1.0f / m_fMoveTime);
		

		SetActorLocation(FMath::Lerp(FVector(m_fStartWorldX, BLOCK_Y_COORD, m_fStartWorldZ), FVector(m_fDestWorldX, BLOCK_Y_COORD, m_fDestWorldZ), rate));

		if (!m_bMoving)
		{
			m_fTimer = 0;
			m_pParent->SetStageStatus(m_iX, m_iY, EBlockType::EEmpty);	//	前の位置を空にする
			m_iX = m_iDestX;
			m_iY = m_iDestY;
			m_pParent->SetStageStatus(m_iX, m_iY, m_eBlockType);		//	新しいポジションに値を設定
			m_pParent->SetBlockMoving(false);
		}
	}
}



//-------------------------------------------------------------
// Name: Clicked()
// Desc: クリックされた時の処理
//-------------------------------------------------------------
void ASuperBlock::Clicked(float mouseX, float mouseZ)
{
	if (!m_bMovable)
		return;

	FVector ActorPos = GetActorLocation();
	float centerX = ActorPos.X, centerZ = ActorPos.Z;	//	ワールド上の座標
	float BSizeDiv2 = BLOCK_SIZE * 0.5f, BSizeDiv4 = BLOCK_SIZE * 0.25f;

	m_iMoveDirX = m_iMoveDirY = 0;	//	ステージ配列上の座標

	if (CheckPointInRect(centerX - BSizeDiv4, centerZ + BSizeDiv4, BSizeDiv2, BSizeDiv4, mouseX, mouseZ))		//	上移動要求
	{
		m_iMoveDirY = -1;
	}
	else if (CheckPointInRect(centerX - BSizeDiv4, centerZ - BSizeDiv2, BSizeDiv2, BSizeDiv4, mouseX, mouseZ))	//	下移動要求
	{
		m_iMoveDirY = 1;
	}
	else if (CheckPointInRect(centerX + BSizeDiv4, centerZ - BSizeDiv4, BSizeDiv4, BSizeDiv2, mouseX, mouseZ))	//	右移動要求
	{
		m_iMoveDirX = 1;
	}
	else if (CheckPointInRect(centerX - BSizeDiv2, centerZ - BSizeDiv4, BSizeDiv4, BSizeDiv2, mouseX, mouseZ))	//	左移動要求
	{
		m_iMoveDirX = -1;
	}

	int destPosStatus = m_pParent->GetStageStatus(m_iX + m_iMoveDirX, m_iY + m_iMoveDirY);

	if (destPosStatus != -1 && destPosStatus == (int)EBlockType::EEmpty)
	{
		m_iDestX = m_iX + m_iMoveDirX;
		m_iDestY = m_iY + m_iMoveDirY;
		m_fDestWorldX = centerX + BLOCK_SIZE * m_iMoveDirX;
		m_fDestWorldZ = centerZ + BLOCK_SIZE * (-m_iMoveDirY);	//	ワールド座標と配列上の座標の縦軸は反転している
		m_fStartWorldX = centerX;
		m_fStartWorldZ = centerZ;
		m_fMoveTime = BLOCK_MOVE_TIME;
		m_bMoving = true;
		m_fTimer = 0;
		m_pParent->SetBlockMoving(true);
	}
}



//-------------------------------------------------------------
// Name: CheckPointInRect()
// Desc: 点が四角形の内側にあるかどうかを判定
//-------------------------------------------------------------
bool ASuperBlock::CheckPointInRect(float left, float bottom, float width, float height, float pointX, float pointY)
{
	if (pointX < left)
		return false;
	if (pointX > left + width)
		return false;
	if (pointY < bottom)
		return false;
	if (pointY > bottom + height)
		return false;

	return true;
}