// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterBlock.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "PaperSprite.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"


//-------------------------------------------------------------
// Name: AWaterBlock()
// Desc: Ctor
//-------------------------------------------------------------
AWaterBlock::AWaterBlock(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	m_bMovable = true;
	m_eBlockType = EBlockType::EWater;
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

	//m_pMaterial_Instance->SetScalarParameterValue("Amount", 0);
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