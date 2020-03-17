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
// Desc: �Q�[���J�n���O�ɌĂ΂�鏉��������
//-------------------------------------------------------------
void AWallBlock::BeginPlay()
{
	Super::BeginPlay();

}



//-------------------------------------------------------------
// Name: Tick()
// Desc: ���t���[���Ă΂��֐�
// Parm: DeltaTime / �O�t���[������̌o�ߎ���
//-------------------------------------------------------------
void AWallBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}