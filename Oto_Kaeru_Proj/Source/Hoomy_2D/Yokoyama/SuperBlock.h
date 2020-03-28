/**
* @file		SuperBlock.h
* @brief	�u���b�N�̃X�[�p�[�N���X
* @author	Akihiro Yokoyama
*/
#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "GameManager.h"
#include "SuperBlock.generated.h"

//-------------------------------------------------------------
// Macros
//-------------------------------------------------------------
USTRUCT()
struct FBlockMoveInfo
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
		bool bMoveVertical;

	UPROPERTY(VisibleAnywhere)
		bool bMoveHorizontal;

	UPROPERTY(VisibleAnywhere)
		int	iMoveDirX;

	UPROPERTY(VisibleAnywhere)
		int iMoveDirY;

public:
		void Init(int characode) {
		switch (characode)
		{
		case L'u':
			bMoveVertical = true;		iMoveDirY = -1;
			bMoveHorizontal = false;	iMoveDirX = 0;
			break;
		case L'd':
			bMoveVertical = true;		iMoveDirY = 1;
			bMoveHorizontal = false;	iMoveDirX = 0;
			break;
		case L'r':
			bMoveVertical = false;		iMoveDirY = 0;
			bMoveHorizontal = true;		iMoveDirX = 1;
			break;
		case L'l':
			bMoveVertical = false;		iMoveDirY = 0;
			bMoveHorizontal = true;		iMoveDirX = -1;
			break;
		default:
			break;
		}
	}
		bool IsMovingVertical() const { return bMoveVertical; }
		bool IsMovingHorizontal() const { return bMoveHorizontal; }
		int GetMoveDirX() const { return iMoveDirX; }
		int GetMoveDirY() const { return iMoveDirY; }
		void ReverseMoveDir() {
		iMoveDirX = (bMoveHorizontal) ? iMoveDirX * -1 : 0;
		iMoveDirY = (bMoveVertical) ? iMoveDirY * -1 : 0;
	}
};




//-------------------------------------------------------------
// Name: ASuperBlock
// Desc: 
//-------------------------------------------------------------
UCLASS()
class HOOMY_2D_API ASuperBlock : public APaperSpriteActor
{
	GENERATED_BODY()
	
public:
	ASuperBlock(const FObjectInitializer& ObjectInitializer);
	virtual ~ASuperBlock();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	//
	//	���ʃ��\�b�h
	//

	/* ���W��ݒ� */
	virtual void SetPosition(int x, int y) { m_iX = x; m_iY = y; }
	/* �e��ݒ� */
	virtual void SetParent(class AGameManager* pParent) { m_pParent = pParent; }
	/* �ړ�����ݒ� */
	virtual void SetMoveInfo(int charaCode) { if (m_bMovable) m_MoveInfo.Init(charaCode); }
	/* �N���b�N���ꂽ���ɌĂԊ֐� */
	virtual void Clicked(float mouseX, float mouseZ);
	/* �ړ��\���ǂ����ύX */
	virtual void SetMovePossibility(bool bMove) { m_bMovable = bMove; }

	//
	//	�J�[�\���I�[�o�[�C�x���g�ɓo�^
	//
	UFUNCTION(BlueprintCallable)
	virtual void BeginCursorOver(UPrimitiveComponent* TouchedComponent);
	UFUNCTION(BlueprintCallable)
	virtual void EndCursorOver(UPrimitiveComponent* TouchedComponent);

protected:
	/* �_����`���ɂ��邩���肷�� */
	bool CheckPointInRect(float left, float bottom, float width, float height, float pointX, float pointY);

protected:
	UPROPERTY(VisibleAnywhere)
	int		m_iX;				//	X���W(�X�e�[�W�z���̍��W)
	UPROPERTY(VisibleAnywhere)
	int		m_iY;				//	Y���W(�X�e�[�W�z���̍��W)

	UPROPERTY(VisibleAnywhere)
	FBlockMoveInfo	m_MoveInfo;


	UPROPERTY(VisibleAnywhere)
	int		m_iMoveDirX;		//	X�ړ�����(�X�e�[�W�z���̍��W)
	UPROPERTY(VisibleAnywhere)
	int		m_iMoveDirY;		//	Y�ړ�����(�X�e�[�W�z���̍��W)
	UPROPERTY(VisibleAnywhere)
	int		m_iDestX;			//	�ړ���ړIX���W(�X�e�[�W�z���̍��W)
	UPROPERTY(VisibleAnywhere)
	int		m_iDestY;			//	�ړ���ړIY���W(�X�e�[�W�z���̍��W)
	UPROPERTY(VisibleAnywhere)
	float	m_fDestWorldX;		//	�ړ���ړIX���W(���[���h��̍��W)
	UPROPERTY(VisibleAnywhere)
	float	m_fDestWorldZ;		//	�ړ���ړIZ���W(���[���h��̍��W)
	UPROPERTY(VisibleAnywhere)
	float	m_fStartWorldX;		//	�ړ��J�nX���W(���[���h��̍��W)
	UPROPERTY(VisibleAnywhere)
	float	m_fStartWorldZ;		//	�ړ��J�nZ���W(���[���h��̍��W)
	UPROPERTY(VisibleAnywhere)
	bool	m_bMoving;			//	�ړ������ǂ���
	UPROPERTY(VisibleAnywhere)
	float	m_fTimer;			//	�ړ��p�^�C�}�[
	UPROPERTY(VisibleAnywhere)
	float	m_fMoveTime;		//	�ړ��ɂ����鎞��

	UPROPERTY(VisibleAnywhere)
	class AGameManager*		m_pParent;	//	�������[���h����GameManager�ւ̎Q��

	//	�p����ŏ������ݒ�
	UPROPERTY(VisibleAnywhere)
	bool	m_bMovable;			//	�ړ��\�u���b�N���ǂ���
	UPROPERTY(VisibleAnywhere)
	EBlockType	m_eBlockType;	//	���̃u���b�N�̃^�C�v

	//	���̃A�N�^
	UPROPERTY(VisibleAnywhere)
	APaperSpriteActor*		m_pChildArrowActor;
};
