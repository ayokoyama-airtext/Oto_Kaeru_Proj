/**
* @file		GameManager.h
* @brief	�Q�[��(�X�e�[�W)�Ǘ��N���X
* @author	Akihiro Yokoyama
*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameManager.generated.h"

//-------------------------------------------------------------
// Macros
//-------------------------------------------------------------
#define	BLOCK_SIZE			128.0f
#define BLOCK_Y_COORD		60.0f
#define BLOCK_MOVE_TIME		1.0f


UENUM()
enum class EBlockType : uint8
{
	EEmpty,
	EWall,
	EWater,
	EMax,
};



//-------------------------------------------------------------
// Name: AGameManager
// Desc: �Q�[���Ǘ��N���X
//-------------------------------------------------------------
UCLASS()
class HOOMY_2D_API AGameManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameManager(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStringTable	*m_pStringTable;		//	�X�g�����O�e�[�u���ւ̎Q��
	UPROPERTY(VisibleAnywhere)
	FText		m_ftStageText;			//	�X�g�����O�e�[�u������ǂݍ��񂾃f�[�^��ێ�(�f�o�b�O�p�ɕ\��)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	INT			m_iCol;					//	�X�e�[�W�̍s��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	INT			m_iRow;					//	�X�e�[�W�̗�
	UPROPERTY(VisibleAnywhere)
	TArray<int>	m_StageArray;			//	�X�e�[�W�̃f�[�^
	UPROPERTY(EditAnywhere)
	TSubclassOf<class APaperSpriteActor>	m_BGBPRef;			//	�w�i�A�Z�b�g�ւ̎Q��

	UPROPERTY(VisibleAnywhere)
	TArray<TSubclassOf<class ASuperBlock>>	m_BlocksRefArray;	//	�u���b�N�A�Z�b�g�ւ̎Q��
	UPROPERTY(VisibleAnywhere)
	class ACameraActor*		m_pCamera;	//	���[���h�̃J����
};
