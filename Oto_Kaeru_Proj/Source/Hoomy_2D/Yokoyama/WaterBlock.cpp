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
// Desc: �Q�[���J�n���O�ɌĂ΂�鏉��������
//-------------------------------------------------------------
void AWaterBlock::BeginPlay()
{
	Super::BeginPlay();

	//m_pMaterial_Instance->SetScalarParameterValue("Amount", 0);
}



//-------------------------------------------------------------
// Name: Tick()
// Desc: ���t���[���Ă΂��֐�
// Parm: DeltaTime / �O�t���[������̌o�ߎ���
//-------------------------------------------------------------
void AWaterBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}