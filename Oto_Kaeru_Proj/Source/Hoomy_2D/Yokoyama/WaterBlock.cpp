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
// Desc: �Q�[���J�n���O�ɌĂ΂�鏉��������
//-------------------------------------------------------------
void AWaterBlock::BeginPlay()
{
	Super::BeginPlay();

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