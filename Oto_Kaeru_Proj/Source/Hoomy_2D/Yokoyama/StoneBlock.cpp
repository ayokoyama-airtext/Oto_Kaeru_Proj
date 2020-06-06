// Fill out your copyright notice in the Description page of Project Settings.


#include "StoneBlock.h"


AStoneBlock::AStoneBlock(const FObjectInitializer& ObjInit)
	:Super(ObjInit)
{
	m_bMovable = true;
	m_eBlockType = EBlockType::EStone;
}

AStoneBlock::~AStoneBlock()
{
}

void AStoneBlock::BeginPlay()
{
	Super::BeginPlay();
}

void AStoneBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
