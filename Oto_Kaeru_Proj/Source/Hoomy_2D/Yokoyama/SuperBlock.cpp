// Fill out your copyright notice in the Description page of Project Settings.


#include "SuperBlock.h"
#include "PaperSprite.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"


//-------------------------------------------------------------
// Name: ASuperBlock()
// Desc: Ctor
//-------------------------------------------------------------
ASuperBlock::ASuperBlock(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer), m_iX(0), m_iY(0), m_pParent(nullptr), m_fDestWorldX(0), m_fDestWorldZ(0), m_bMoving(false), m_fTimer(0), m_iMoveDirX(0), m_iMoveDirY(0), m_fMoveTime(BLOCK_MOVE_TIME), m_fStartWorldX(0), m_fStartWorldZ(0), m_pChildArrowActor(nullptr)
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

	UPaperSpriteComponent* pRenderComp = GetRenderComponent();
	if (m_bMovable)
	{
		pRenderComp->OnBeginCursorOver.AddDynamic(this, &ASuperBlock::BeginCursorOver);
		pRenderComp->OnEndCursorOver.AddDynamic(this, &ASuperBlock::EndCursorOver);

		FString path_ = "/Game/Working/Yokoyama/BP/Arrow_BP.Arrow_BP_C";
		TSubclassOf<class APaperSpriteActor> sc = TSoftClassPtr<APaperSpriteActor>(FSoftObjectPath(*path_)).LoadSynchronous();	//	�N���X�̎擾
		if (sc != nullptr)
		{
			APaperSpriteActor* child_ = GetWorld()->SpawnActor<APaperSpriteActor>(sc);
			if (child_)
			{
				child_->SetActorRotation(FRotator(0, 0, 0));
				child_->AttachToActor(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
				m_pChildArrowActor = child_;
				m_pChildArrowActor->SetActorRelativeLocation(FVector(0, +2.5f, 0));
				m_pChildArrowActor->SetActorHiddenInGame(false);
			}
		}
	}

	//	�ݒ肳��Ă���}�e���A������}�e���A���C���X�^���X���쐬���ăZ�b�g
	m_pMaterial_Instance = pRenderComp->CreateAndSetMaterialInstanceDynamic(0);
}



//-------------------------------------------------------------
// Name: EndPlay()
// Desc: �Q�[���I�����O�ɌĂ΂�鏈��
//-------------------------------------------------------------
void ASuperBlock::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	m_pParent = nullptr;
}



//-------------------------------------------------------------
// Name: Tick()
// Desc: ���t���[���Ă΂��֐�
// Parm: DeltaTime / �O�t���[������̌o�ߎ���
//-------------------------------------------------------------
void ASuperBlock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//	�u���b�N�ړ�����
	if (m_bMoving)
	{
		m_fTimer += DeltaTime;
		float rate = 0;

		if (m_fTimer >= m_fMoveTime)
		{
			int destPosStatus = m_pParent->GetStageStatus(m_iDestX + m_iMoveDirX, m_iDestY + m_iMoveDirY);
			if (destPosStatus != -1 && (destPosStatus == (int)EBlockType::EEmpty || destPosStatus == (int)EBlockType::EStart))
			{
				//	�������I�g�m�T�}�̏�ɂ���Ȃ�A���̈ړ��悪Empty�ł���~����
				if (m_pParent->GetStageStatus(m_iDestX, m_iDestY) == (int)EBlockType::EStart)
				{
					m_fTimer = m_fMoveTime;
					m_bMoving = false;
				}
				//	�ړ����s
				else 
				{
					m_iDestX += m_iMoveDirX;
					m_iDestY += m_iMoveDirY;
					m_fStartWorldX = m_fDestWorldX;
					m_fStartWorldZ = m_fDestWorldZ;
					m_fDestWorldX += BLOCK_SIZE * m_iMoveDirX;
					m_fDestWorldZ += BLOCK_SIZE * (-m_iMoveDirY);	//	���[���h���W�Ɣz���̍��W�̏c���͔��]���Ă���
					m_fTimer -= BLOCK_MOVE_TIME;
				}
			}
			else
			{
				m_fTimer = m_fMoveTime;
				m_bMoving = false;
			}
		}

		
		rate = m_fTimer * (1.0f / m_fMoveTime);
		

		SetActorLocation(FMath::Lerp(FVector(m_fStartWorldX, BLOCK_Y_COORD, m_fStartWorldZ), FVector(m_fDestWorldX, BLOCK_Y_COORD, m_fDestWorldZ), rate));

		//	�ړ��������̏���
		if (!m_bMoving)
		{
			m_fTimer = 0;
			m_pParent->SetStageStatus(m_iX, m_iY, EBlockType::EEmpty);	//	�O�̈ʒu����ɂ���
			m_pParent->SetBlockStatus(m_iX, m_iY, nullptr);

			m_iX = m_iDestX;
			m_iY = m_iDestY;

			m_pParent->SetStageStatus(m_iX, m_iY, m_eBlockType);		//	�V�����|�W�V�����ɒl��ݒ�
			m_pParent->SetBlockStatus(m_iX, m_iY, this);

			m_MoveInfo.ReverseMoveDir();	//	����̈ړ������𔽓]������

			RotateChildArrow();

			m_pParent->IncreaseClickCount();
			m_pParent->SetBlockMoving(false);
		}
	}
}



//-------------------------------------------------------------
// Name: Clicked()
// Desc: �N���b�N���ꂽ���̏���
//-------------------------------------------------------------
void ASuperBlock::Clicked(float mouseX, float mouseZ)
{
	if (!m_bMovable)
		return;

	FVector ActorPos = GetActorLocation();
	float centerX = ActorPos.X, centerZ = ActorPos.Z;	//	���[���h��̍��W
	float BSizeDiv2 = BLOCK_SIZE * 0.5f, BSizeDiv4 = BLOCK_SIZE * 0.25f;

	m_iMoveDirX = m_iMoveDirY = 0;	//	�X�e�[�W�z���̍��W

	m_iMoveDirX = m_MoveInfo.GetMoveDirX();
	m_iMoveDirY = m_MoveInfo.GetMoveDirY();

	//	�N���b�N�������W�ɉ����Ĉړ����������߂�ꍇ
	//if (CheckPointInRect(centerX - BSizeDiv4, centerZ + BSizeDiv4, BSizeDiv2, BSizeDiv4, mouseX, mouseZ))		//	��ړ��v��
	//{
	//	m_iMoveDirY = -1;
	//}
	//else if (CheckPointInRect(centerX - BSizeDiv4, centerZ - BSizeDiv2, BSizeDiv2, BSizeDiv4, mouseX, mouseZ))	//	���ړ��v��
	//{
	//	m_iMoveDirY = 1;
	//}
	//else if (CheckPointInRect(centerX + BSizeDiv4, centerZ - BSizeDiv4, BSizeDiv4, BSizeDiv2, mouseX, mouseZ))	//	�E�ړ��v��
	//{
	//	m_iMoveDirX = 1;
	//}
	//else if (CheckPointInRect(centerX - BSizeDiv2, centerZ - BSizeDiv4, BSizeDiv4, BSizeDiv2, mouseX, mouseZ))	//	���ړ��v��
	//{
	//	m_iMoveDirX = -1;
	//}

	int destPosStatus = m_pParent->GetStageStatus(m_iX + m_iMoveDirX, m_iY + m_iMoveDirY);

	if (destPosStatus != -1 && (destPosStatus == (int)EBlockType::EEmpty || destPosStatus == (int)EBlockType::EStart))
	{
		//	���̈ړ���(�z����W)
		m_iDestX = m_iX + m_iMoveDirX;
		m_iDestY = m_iY + m_iMoveDirY;
		//	���̈ړ���(���[���h���W)
		m_fDestWorldX = centerX + BLOCK_SIZE * m_iMoveDirX;
		m_fDestWorldZ = centerZ + BLOCK_SIZE * (-m_iMoveDirY);	//	���[���h���W�Ɣz���̍��W�̏c���͔��]���Ă���
		//	���ݒn(���[���h���W)
		m_fStartWorldX = centerX;
		m_fStartWorldZ = centerZ;

		m_fMoveTime = BLOCK_MOVE_TIME;
		m_bMoving = true;
		m_fTimer = 0;

		//	���I�g�m�T�}�Ɣ���Ă���Ȃ�񐅒���Ԃɖ߂��Ă���
		if (m_pParent->GetStageStatus(m_iX, m_iY) == (int)EBlockType::EStartWithWater)
		{
			m_pParent->ChangeOtonosamaState(false);
		}

		m_pParent->SetBlockMoving(true);
	}
}



//-------------------------------------------------------------
// Name: BeginCursorOver()
// Desc: 
//-------------------------------------------------------------
void ASuperBlock::BeginCursorOver(UPrimitiveComponent* TouchedComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("BeginCursorOver!."));
	if (m_bMovable)
	{
		//m_pChildArrowActor->SetActorHiddenInGame(false);
		
		RotateChildArrow();

		//m_pChildArrowActor->SetActorRotation(FRotator(0, yaw_, 0));
	}
}



//-------------------------------------------------------------
// Name: EndCursorOver()
// Desc: 
//-------------------------------------------------------------
void ASuperBlock::EndCursorOver(UPrimitiveComponent* TouchedComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("EndCursorOver!."));
	if (m_bMovable)
	{
		//m_pChildArrowActor->SetActorHiddenInGame(true);
	}
}



//-------------------------------------------------------------
// Name: CheckPointInRect()
// Desc: �_���l�p�`�̓����ɂ��邩�ǂ����𔻒�
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



//-------------------------------------------------------------
// Name: RotateChildArrow()
// Desc: ������]�����Č��݂̐i�s�����ɍ��킹��
//-------------------------------------------------------------
void ASuperBlock::RotateChildArrow()
{
	float rot_ = 0;
	if (m_MoveInfo.IsMovingHorizontal())
	{
		rot_ = (m_MoveInfo.GetMoveDirX() == 1) ? 270.f : 90.f;
	}
	else
	{
		rot_ = (m_MoveInfo.GetMoveDirY() == 1) ? 180.f : 0;
	}
	m_pChildArrowActor->SetActorRelativeRotation(FRotator(rot_, 0, 0));
}



void ASuperBlock::SetMoveInfo(int charaCode)
{
	if (m_bMovable)
	{
		if (charaCode == L' ')
		{
			SetMovePossibility(false);
			m_pChildArrowActor->Destroy();
			m_pChildArrowActor = nullptr;
		}
		else
		{
			m_MoveInfo.Init(charaCode);
			RotateChildArrow();
		}
	}
}



void ASuperBlock::SetMovePossibility(bool bMove) 
{ 
	m_bMovable = bMove;
	if (m_pChildArrowActor != nullptr)
	{
		m_pChildArrowActor->SetActorHiddenInGame(!bMove);
	}
}