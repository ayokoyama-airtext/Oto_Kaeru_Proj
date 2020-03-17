/**
* @file		SuperBlock.h
* @brief	�u���b�N�̃X�[�p�[�N���X
* @author	Akihiro Yokoyama
*/
#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "SuperBlock.generated.h"



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
	//
	//	���ʃ��\�b�h
	//

	/* ���W��ݒ� */
	virtual void SetPosition(int x, int y) { m_iX = x; m_iY = y; }
	/* �e��ݒ� */
	virtual void SetParent(class AGameManager* pParent) { m_pParent = pParent; }


protected:
	UPROPERTY(VisibleAnywhere)
	int		m_iX;	//	X���W
	UPROPERTY(VisibleAnywhere)
	int		m_iY;	//	Y���W
	UPROPERTY(VisibleAnywhere)
	bool	m_bMovable;		//	�ړ��\�u���b�N���ǂ���

	UPROPERTY(VisibleAnywhere)
	class AGameManager*		m_pParent;	//	�������[���h����GameManager�ւ̎Q��
};
