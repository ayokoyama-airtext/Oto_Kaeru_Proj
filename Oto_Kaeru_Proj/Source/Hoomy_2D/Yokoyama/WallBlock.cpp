// Fill out your copyright notice in the Description page of Project Settings.


#include "WallBlock.h"
#include "GameManager.h"


//-------------------------------------------------------------
// Name: AWallBlock()
// Desc: Ctor
//-------------------------------------------------------------
AWallBlock::AWallBlock(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	m_bMovable = false;
	m_eBlockType = EBlockType::EWall;
}



//-------------------------------------------------------------
// Name: ~AWallBlock()
// Desc: Dtor
//-------------------------------------------------------------
AWallBlock::~AWallBlock()
{

}



//-------------------------------------------------------------
// Name: BeginPlay()
// Desc: ゲーム開始直前に呼ばれる初期化処理
//-------------------------------------------------------------
void AWallBlock::BeginPlay()
{
	Super::BeginPlay();

}



//-------------------------------------------------------------
// Name: Tick()
// Desc: 毎フレーム呼ばれる関数
// Parm: DeltaTime / 前フレームからの経過時間
//-------------------------------------------------------------
void AWallBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}