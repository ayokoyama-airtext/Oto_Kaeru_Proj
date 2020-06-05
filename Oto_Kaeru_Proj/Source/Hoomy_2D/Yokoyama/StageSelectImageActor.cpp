// Fill out your copyright notice in the Description page of Project Settings.


#include "StageSelectImageActor.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "PaperSprite.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"
#include "StageSelectManager.h"


//-------------------------------------------------------------
// Name: AStageSelectImageActor()
// Desc: Ctor
//-------------------------------------------------------------
AStageSelectImageActor::AStageSelectImageActor(const FObjectInitializer& ObjectInitializer)
	:APaperSpriteActor(ObjectInitializer), m_LoadMapPath(""), m_bCursorOver(false), m_bCursorEnd(false), m_fTimer(0),m_fExpansionRate(1.0f), m_fMatParamAmount(0.06f),m_pMaterial_Instance(nullptr), m_fDefaultYCoord(360.f), m_fExpansionYCoord(400.f), m_pStageNameActor(nullptr), m_pManager(nullptr)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}



//-------------------------------------------------------------
// Name: BeginPlay()
// Desc: �Q�[���J�n���O�ɌĂ΂�鏉��������
//-------------------------------------------------------------
void AStageSelectImageActor::BeginPlay()
{
	Super::BeginPlay();

	UPaperSpriteComponent* pRenderComp = GetRenderComponent();
	pRenderComp->OnBeginCursorOver.AddDynamic(this, &AStageSelectImageActor::BeginCursorOver);
	pRenderComp->OnEndCursorOver.AddDynamic(this, &AStageSelectImageActor::EndCursorOver);
	pRenderComp->OnClicked.AddDynamic(this, &AStageSelectImageActor::Clicked);

	//	�ݒ肳��Ă���}�e���A������}�e���A���C���X�^���X���쐬���ăZ�b�g
	m_pMaterial_Instance = pRenderComp->CreateAndSetMaterialInstanceDynamic(0);
	m_pMaterial_Instance->SetScalarParameterValue("Amount", 0);

	//	�����A�N�^���B��
	if (m_pStageNameActor)
	{
		m_pStageNameActor->GetRenderComponent()->SetSpriteColor(FLinearColor(1, 1, 1, 0));
	}
}



//-------------------------------------------------------------
// Name: Tick()
// Desc: ���t���[���Ă΂��֐�
// Parm: DeltaTime / �O�t���[������̌o�ߎ���
//-------------------------------------------------------------
void AStageSelectImageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_bCursorOver)
	{
		float rate_ = 0;

		if ((m_fTimer += DeltaTime) < ANIMATION_TIME)
		{
			rate_ = m_fTimer * (1.0f / ANIMATION_TIME);
		}
		else
		{
			rate_ = 1.0f;
			m_fTimer = 0;
			m_bCursorOver = false;
		}

		//	�X�P�[���A�b�v
		float current_ = GetActorScale().X;
		float new_ = FMath::Lerp(current_, m_fExpansionRate, rate_);
		SetActorScale3D(FVector(new_, 1.0f, new_));

		//	��炬�p�����[�^
		current_ = m_pMaterial_Instance->K2_GetScalarParameterValue("Amount");
		new_ = FMath::Lerp(current_, m_fMatParamAmount, rate_);
		m_pMaterial_Instance->SetScalarParameterValue("Amount", new_);

		//	Y���W
		FVector currentLoc_ = GetActorLocation();
		new_ = FMath::Lerp(currentLoc_.Y, m_fExpansionYCoord, rate_);
		SetActorLocation(FVector(currentLoc_.X, new_, currentLoc_.Z));

		//	�����A�N�^�\��
		if (m_pStageNameActor)
		{
			m_pStageNameActor->GetRenderComponent()->SetSpriteColor(FLinearColor(1, 1, 1, rate_));
		}
	}
	else if (m_bCursorEnd)
	{
		float rate_ = 0;

		if ((m_fTimer += DeltaTime) < ANIMATION_TIME)
		{
			rate_ = m_fTimer * (1.0f / ANIMATION_TIME);
		}
		else
		{
			rate_ = 1.0f;
			m_fTimer = 0;
			m_bCursorEnd = false;
		}

		//	�X�P�[���_�E��
		float current_ = GetActorScale().X;
		float new_ = FMath::Lerp(current_, 1.0f, rate_);
		SetActorScale3D(FVector(new_, 1.0f, new_));

		//	��炬�p�����[�^
		current_ = m_pMaterial_Instance->K2_GetScalarParameterValue("Amount");
		new_ = FMath::Lerp(current_, 0.f, rate_);
		m_pMaterial_Instance->SetScalarParameterValue("Amount", new_);

		//	Y���W
		FVector currentLoc_ = GetActorLocation();
		new_ = FMath::Lerp(currentLoc_.Y, m_fDefaultYCoord, rate_);
		SetActorLocation(FVector(currentLoc_.X, new_, currentLoc_.Z));

		//	�����A�N�^��\��
		if (m_pStageNameActor)
		{
			m_pStageNameActor->GetRenderComponent()->SetSpriteColor(FLinearColor(1, 1, 1, 1.0f - rate_));
		}
	}
}



//-------------------------------------------------------------
// Name: BeginCursorOver()
// Desc: OnBeginCursorOver�f���Q�[�g�ɓo�^
//-------------------------------------------------------------
void AStageSelectImageActor::BeginCursorOver(UPrimitiveComponent* TouchedComponent)
{
	m_bCursorOver = true;
	m_bCursorEnd = false;
	m_fTimer = 0;
	GetRenderComponent()->SetTranslucentSortPriority(1);
}



//-------------------------------------------------------------
// Name: EndCursorOver()
// Desc: OnEndCursorOver�f���Q�[�g�ɓo�^
//-------------------------------------------------------------
void AStageSelectImageActor::EndCursorOver(UPrimitiveComponent* TouchedComponent)
{
	m_bCursorOver = false;
	m_bCursorEnd = true;
	m_fTimer = 0;
	GetRenderComponent()->SetTranslucentSortPriority(0);
}



//-------------------------------------------------------------
// Name: Clicked()
// Desc: OnClicked�f���Q�[�g�ɓo�^
//-------------------------------------------------------------
void AStageSelectImageActor::Clicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	UE_LOG(LogTemp, Warning, TEXT("Clicked Event!"));
	if (!m_LoadMapPath.IsEmpty())
	{
		//UGameplayStatics::OpenLevel(this, FName(*m_LoadMapPath));
		if (m_pManager)
		{
			m_pManager->StartLoadMap(m_LoadMapPath);
		}
	}
}
