// Fill out your copyright notice in the Description page of Project Settings.


#include "SuperBlock.h"
#include "PaperSprite.h"
#include "Paper2D/Classes/PaperSpriteComponent.h"


//-------------------------------------------------------------
// Name: ASuperBlock()
// Desc: Ctor
//-------------------------------------------------------------
ASuperBlock::ASuperBlock(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer), m_iX(0), m_iY(0), m_pParent(nullptr), m_fDestWorldX(0), m_fDestWorldZ(0), m_bMoving(false), m_fTimer(0)
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
		if (m_fTimer >= BLOCK_MOVE_TIME)
		{
			rate = 1.0f;
			m_bMoving = false;
		}
		else
		{
			rate = m_fTimer * (1.0f / BLOCK_MOVE_TIME);
		}

		SetActorLocation(FMath::Lerp(GetActorLocation(), FVector(m_fDestWorldX, BLOCK_Y_COORD, m_fDestWorldZ), rate));

		if (!m_bMoving)
		{
			m_fTimer = 0;
			m_pParent->SetStageStatus(m_iX, m_iY, EBlockType::EEmpty);	//	�O�̈ʒu����ɂ���
			m_iX = m_iDestX;
			m_iY = m_iDestY;
			m_pParent->SetStageStatus(m_iX, m_iY, m_eBlockType);		//	�V�����|�W�V�����ɒl��ݒ�
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

	int destDirX = 0, destDirY = 0;	//	�X�e�[�W�z���̍��W

	if (CheckPointInRect(centerX - BSizeDiv4, centerZ + BSizeDiv4, BSizeDiv2, BSizeDiv4, mouseX, mouseZ))		//	��ړ��v��
	{
		destDirY = -1;
	}
	else if (CheckPointInRect(centerX - BSizeDiv4, centerZ - BSizeDiv2, BSizeDiv2, BSizeDiv4, mouseX, mouseZ))	//	���ړ��v��
	{
		destDirY = 1;
	}
	else if (CheckPointInRect(centerX + BSizeDiv4, centerZ - BSizeDiv4, BSizeDiv4, BSizeDiv2, mouseX, mouseZ))	//	�E�ړ��v��
	{
		destDirX = 1;
	}
	else if (CheckPointInRect(centerX - BSizeDiv2, centerZ - BSizeDiv4, BSizeDiv4, BSizeDiv2, mouseX, mouseZ))	//	���ړ��v��
	{
		destDirX = -1;
	}

	int destPosStatus = m_pParent->GetStageStatus(m_iX + destDirX, m_iY + destDirY);

	if (destPosStatus != -1 && destPosStatus == (int)EBlockType::EEmpty)
	{
		m_iDestX = m_iX + destDirX;
		m_iDestY = m_iY + destDirY;
		m_fDestWorldX = centerX + BLOCK_SIZE * destDirX;
		m_fDestWorldZ = centerZ + BLOCK_SIZE * (-destDirY);	//	���[���h���W�Ɣz���̍��W�̏c���͔��]���Ă���
		m_bMoving = true;
		m_fTimer = 0;
		m_pParent->SetBlockMoving(true);
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