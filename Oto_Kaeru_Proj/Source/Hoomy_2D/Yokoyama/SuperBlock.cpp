// Fill out your copyright notice in the Description page of Project Settings.


#include "SuperBlock.h"
#include "PaperSprite.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"


//-------------------------------------------------------------
// Name: ASuperBlock()
// Desc: Ctor
//-------------------------------------------------------------
ASuperBlock::ASuperBlock(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer), m_iX(0), m_iY(0), m_pParent(nullptr)
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
// Desc: �Q�[���J�n���O�ɌĂ΂�鏉��������
//-------------------------------------------------------------
void ASuperBlock::BeginPlay()
{
	Super::BeginPlay();

}