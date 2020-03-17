// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterBlock.h"


//-------------------------------------------------------------
// Name: AWaterBlock()
// Desc: Ctor
//-------------------------------------------------------------
AWaterBlock::AWaterBlock(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_bMovable = true;
}



//-------------------------------------------------------------
// Name: ~AWaterBlock()
// Desc: Dtor
//-------------------------------------------------------------
AWaterBlock::~AWaterBlock()
{

}



//-------------------------------------------------------------
// Name: BeginPlay()
// Desc: ゲーム開始直前に呼ばれる初期化処理
//-------------------------------------------------------------
void AWaterBlock::BeginPlay()
{
	Super::BeginPlay();

}



//-------------------------------------------------------------
// Name: Tick()
// Desc: 毎フレーム呼ばれる関数
// Parm: DeltaTime / 前フレームからの経過時間
//-------------------------------------------------------------
void AWaterBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}