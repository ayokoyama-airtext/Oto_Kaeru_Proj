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
#define BLOCK_Y_COORD		60.0f	//	Y���W�͉��s�̃p�����[�^
#define BLOCK_MOVE_TIME		0.25f
#define CAMERA_Y_COORD		1100.0f

/* �V�����ǉ�����Ƃ��͍Ō���ɒǉ����邱�� */
UENUM()
enum class EBlockType : uint8
{
	EEmpty,
	EWall,
	EWater,
	EStart,
	EGoal,
	EWaterWall,
	EStartWithWater,
	EStone,
	EMax,
	EWithinSong = 0x40,
};


USTRUCT()
struct FBlockInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	int	col;

	UPROPERTY(VisibleAnywhere)
	int row;

	/*UPROPERTY(VisibleAnywhere)
	class APaperFlipbookActor*	Tonosama;

	UPROPERTY(VisibleAnywhere)
	class APaperFlipbookActor*	TonosamaInWater;

	UPROPERTY(VisibleAnywhere)
	class APaperFlipbookActor*	Tamago;

	UPROPERTY(VisibleAnywhere)
	class APaperFlipbookActor*	Otama;*/

	UPROPERTY(VisibleAnywhere)
	class AOtosama*	TonosamaBP;

	UPROPERTY(VisibleAnywhere)
	class AOtamago * TamagoBP;

	UPROPERTY(VisibleAnywhere)
	class ASuperBlock*			WaterBlock;

	/* Tonosama�p�ϐ� */
	UPROPERTY(VisibleAnywhere)
	bool bInWater;

	/* Tamago�p�ϐ� */
	UPROPERTY(VisibleAnywhere)
	bool bClear;

	FBlockInfo()
		:col(0),row(0),TonosamaBP(nullptr), TamagoBP(nullptr)/*,Tonosama(nullptr),TonosamaInWater(nullptr),Tamago(nullptr),Otama(nullptr)*/,WaterBlock(nullptr), bInWater(false), bClear(false)
	{
	}
};



//-------------------------------------------------------------
// Name: AGameManager
// Desc: �Q�[���Ǘ��N���X
//-------------------------------------------------------------
UCLASS()
class HOOMY_2D_API AGameManager : public AActor
{
	GENERATED_BODY()
	
private:	
	// Sets default values for this actor's properties
	AGameManager(const FObjectInitializer& ObjectInitializer);
	// Singleton
	static AGameManager*	instance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* �C���X�^���X���擾 */
	UFUNCTION(BlueprintCallable)
	static AGameManager* GetInstance();

	/* �I�[�v�j���O���o�̏I����ʒm */
	UFUNCTION(BlueprintCallable)
	static void NotifyOpeningEnd() { instance->m_bOpeningEnd = true; };

	/* �X�e�[�W�̏�Ԃ��擾 */
	UFUNCTION(BlueprintCallable)
	int GetStageStatus(int col, int row) const;

	/* �X�e�[�W�̏�Ԃ�ݒ� */
	UFUNCTION(BlueprintCallable)
	void SetStageStatus(int col, int row, EBlockType bt);

	/* �u���b�N�̎Q�Ƃ�ݒ� */
	UFUNCTION(BlueprintCallable)
	void SetBlockStatus(int col, int row, class ASuperBlock* pBlock);

	/* �u���b�N���ړ������ǂ�����ݒ� */
	UFUNCTION(BlueprintCallable)
	void SetBlockMoving(bool bBlockMoving) { m_bBlockMoving = bBlockMoving; }

	/* �N���b�N�񐔂𑝂₷ */
	UFUNCTION(BlueprintCallable)
	void IncreaseClickCount();

	UFUNCTION()
	int GetMaxClickCount() const { return m_iMaxClickNum; }

	/* �J�G���̏�Ԃ�ς��� */
	UFUNCTION(BlueprintCallable)
	void ChangeOtonosamaState(bool bInWater);

	///////////////////
	// ���͂ŌĂԊ֐�
	///////////////////
	void LeftClickEvent();

protected:
	/* �N���A���Ă邩���m�F(�J�G���������ɂ���Ƃ�) */
	void CheckClear();

	/* �u���b�N�������Ă����B�ċA�֐� */
	bool CheckBlock(int x, int y, int* map, bool bFirstCheck);

	/* �N���A���Ă邩���m�F(�J�G���������ɂ��Ȃ��Ƃ�) */
	void CheckClearWithoutWater();

	/* �u���b�N�������Ă���(�����ɂ��Ȃ��Ƃ��p) */
	bool CheckBlockWithoutWater(int x, int y);

	/* �͈̔͂̐ݒ��ύX(�I�g�m�T�}�̎���3�}�X) */
	void ChangeBlockStateWithinSong(bool bOn);

	/* �g�m�T�}�̎��͂̐��u���b�N��T�� */
	void CheckWaterBlockAroundTonosama();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStringTable	*m_pStringTable;		//	�X�g�����O�e�[�u���ւ̎Q��
	UPROPERTY(VisibleAnywhere)
	FText		m_ftStageText;			//	�X�g�����O�e�[�u������ǂݍ��񂾃f�[�^��ێ�(�f�o�b�O�p�ɕ\��)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	INT			m_iCol;					//	�X�e�[�W�̗�
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	INT			m_iRow;					//	�X�e�[�W�̍s��
	UPROPERTY(VisibleAnywhere)
	float		m_fWidth;				//	�X�e�[�W��
	UPROPERTY(VisibleAnywhere)
	float		m_fHeight;				//	�X�e�[�W����
	UPROPERTY(VisibleAnywhere)
	TArray<int>	m_StageArray;			//	�X�e�[�W�̃f�[�^
	UPROPERTY(VisibleAnywhere)
	TArray<class ASuperBlock*>	m_BlockArray;	//	�X�|�[�������u���b�N�ւ̎Q�Ɣz��


	UPROPERTY(VisibleAnywhere)
	INT			m_iGoalNum;				//	�S�[���̐�
	UPROPERTY(VisibleAnywhere)
	INT			m_iClearedGoalNum;		//	�����𖞂������S�[���̐�
	UPROPERTY(VisibleAnywhere)
	FBlockInfo	m_StartBlock;			//	�X�^�[�g�u���b�N
	UPROPERTY(VisibleAnywhere)
	TArray<FBlockInfo>	m_GoalBlockArray;	//	�S�[���u���b�N


	UPROPERTY(VisibleAnywhere)
	INT			m_iMaxClickNum;			//	���̃X�e�[�W�ŃN���b�N�ł��鐔
	UPROPERTY(VisibleAnywhere)
	INT			m_iClickCount;			//	���݂̃N���b�N��
	UPROPERTY(VisibleAnywhere)
	bool		m_bClearStage;			//	�N���A�������ǂ���
	UPROPERTY(VisibleAnywhere)
	bool		m_bGameOver;			//	�Q�[���I�[�o�[�������ǂ���

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class UGameUserWidget>	m_WidgetBPRef;		//	Widget�A�Z�b�g�ւ̎Q��
	UPROPERTY(VisibleAnywhere)
	class UGameUserWidget*				m_pWidget;			//	Widget�C���X�^���X�̃|�C���^


	UPROPERTY(EditAnywhere)
	TSubclassOf<class APaperSpriteActor>	m_BGBPRef;			//	�w�i�A�Z�b�g�ւ̎Q��
	UPROPERTY(EditAnywhere)
	TSubclassOf<class APaperSpriteActor>	m_BGEdgeRef;
	UPROPERTY(VisibleAnywhere)
	TArray<TSubclassOf<class ASuperBlock>>	m_BlocksRefArray;	//	�u���b�N�A�Z�b�g�ւ̎Q��

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class APaperFlipbookActor>	m_TonosamaRef;	//	�Ƃ̂��܃A�Z�b�g�ւ̎Q��
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class APaperFlipbookActor>	m_TonosamaInWaterRef;	//	�Ƃ̂��܃A�Z�b�g�ւ̎Q��
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class APaperFlipbookActor>	m_TamagoRef;	//	���܂��A�Z�b�g�ւ̎Q��
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class APaperFlipbookActor>	m_OtamaRef;		//	�����܃A�Z�b�g�ւ̎Q��

	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class AOtosama>	m_TonosamaBPRef;
	UPROPERTY(VisibleAnywhere)
	class AOtosama*				m_pTonosama;			//	Otonosama�C���X�^���X�̃|�C���^
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<class AOtamago>		m_TamagoBPRef;
	UPROPERTY(VisibleAnywhere)
	class AOtamago*				m_pTamago;				//	Tamago�C���X�^���X�̃|�C���^

	UPROPERTY(VisibleAnywhere)
	class ACameraActor*		m_pCamera;	//	���[���h�̃J����

	UPROPERTY(VisibleAnywhere)
	bool	m_bBlockMoving;		//	�ړ����̃u���b�N�����邩�ǂ���
	UPROPERTY(VisibleAnywhere)
	bool	m_bOpeningEnd;		//	�I�[�v�j���O���I��������ǂ���

	//	�Q�[���N���A�@or �Q�[���I�[�o�[���x���ɓn�����
	UPROPERTY(EditAnywhere)
		FString currentStage;
	UPROPERTY(EditAnywhere)
		FString nextStage;
};
