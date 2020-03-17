// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "Engine/Blueprint.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"
#include "StringTableCore.h"
#include "StringTable.h"
#include "Camera/CameraActor.h"
#include "Paper2D/Classes/PaperSpriteActor.h"
#include "SuperBlock.h"
#include "WallBlock.h"

//-------------------------------------------------------------
// Static Fields Init
//-------------------------------------------------------------
AGameManager* AGameManager::instance = nullptr;



//-------------------------------------------------------------
// Name: AGameManager()
// Desc: Ctor
//-------------------------------------------------------------
AGameManager::AGameManager(const FObjectInitializer& ObjectInitializer)
	:AActor(ObjectInitializer),m_iCol(0),m_iRow(0), m_bBlockMoving(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	//	�X�|�[�������邽�߂Ƀu���b�NBP�̎Q�Ƃ����ɍs��
	m_BlocksRefArray.Init(nullptr, (uint8)EBlockType::EMax);
#if UE_GAME
	//	�����^�C��
	//	�u���b�N
	TArray<FString>	BlockBPPathArray = {"",
										"/Game/Working/Yokoyama/BP/WallBlockBP",
										"/Game/Working/Yokoyama/BP/WaterBlockBP",
										};
	for (int i = 1; i < (uint8)EBlockType::EMax; ++i)
	{
		static ConstructorHelpers::FClassFinder<ASuperBlock> BluePrintFile(*BlockBPPathArray[i]);
		if (BluePrintFile.Class)
		{
			m_BlocksRefArray[i] = (UClass*)BluePrintFile.Class;
		}
	}
#else
	//	�G�f�B�^�[
	//	�u���b�N
	TArray<FString>	BlockBPPathArray = {"",
										"Blueprint'/Game/Working/Yokoyama/BP/WallBlockBP.WallBlockBP'",
										"Blueprint'/Game/Working/Yokoyama/BP/WaterBlockBP.WaterBlockBP'",
										 };
	
	for (int i = 1; i < (uint8)EBlockType::EMax; ++i)
	{
		ConstructorHelpers::FObjectFinder<UBlueprint> BluePrintFile(*BlockBPPathArray[i]);
		if (BluePrintFile.Object)
		{
			m_BlocksRefArray[i] = (UClass*)BluePrintFile.Object->GeneratedClass;
		}
	}
#endif
}



//-------------------------------------------------------------
// Name: BeginPlay()
// Desc: �Q�[���J�n���O�ɌĂ΂�鏉��������
//-------------------------------------------------------------
void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	if (instance == nullptr)
	{
		instance = this;
	}
	
	//	�X�g�����O�e�[�u������X�e�[�W�f�[�^��ǂݍ���
	if (m_pStringTable)
	{
		FName	tableID_ = m_pStringTable->GetStringTableId();
		m_ftStageText = FText::FromStringTable(tableID_, "stage");

		FString str_ = m_ftStageText.ToString();
		const TCHAR*  wchar_ = *str_;
		UE_LOG(LogTemp, Warning, TEXT("%s"), *str_);

		//	�ŏ���2�����͍s���Ɨ�
		m_iCol = *wchar_++ - L'0';
		m_iRow = *wchar_++ - L'0';
		//	�Ō�܂Ő��������Ă���
		while (*wchar_ != L'\0')
		{
			int c_ = *wchar_;
			if (L'0' <= c_ && c_ <= L'9')
			{
				m_StageArray.Emplace(c_ - L'0');
			}
			wchar_++;
		}
	}

	//	�u���b�N���X�|�[��������
	float width, height;
	float x, y, z;

	width = BLOCK_SIZE * m_iRow;
	height = BLOCK_SIZE * m_iCol;
	y = BLOCK_Y_COORD;

	if (m_StageArray.Num())
	{
		for (int i = 0; i < m_StageArray.Num(); ++i)
		{
			int blockID_ = m_StageArray[i];
			if (blockID_ != 0)
			{
				int col = i / m_iCol;
				int row = i % m_iRow;
				x = row * BLOCK_SIZE + BLOCK_SIZE * 0.5f;
				z = height - (col * BLOCK_SIZE + BLOCK_SIZE * 0.5f);
				ASuperBlock* block_ = GetWorld()->SpawnActor<ASuperBlock>(m_BlocksRefArray[blockID_], FVector(x, y, z), FRotator(0, 0, 0));
				if (block_)
				{
					block_->SetPosition(row, col);
					block_->SetParent(this);
				}
			}
		}
	}

	//	���[���h���̃J�������������Ĕz�u
	TSubclassOf<ACameraActor> findClass;
	findClass = ACameraActor::StaticClass();
	TArray<AActor*> actorsArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), findClass, actorsArray);
	if (actorsArray.Num())
	{
		m_pCamera = Cast<ACameraActor>(actorsArray[0]);
		m_pCamera->SetActorLocation(FVector(width * 0.5f, 120.f, height * 0.5f));
	}

	//	�w�i��z�u
	{
		if (m_BGBPRef)
		{
			APaperSpriteActor* bg_ = GetWorld()->SpawnActor<APaperSpriteActor>(m_BGBPRef, FVector(width *0.5f, 0, height * 0.5f), FRotator(0, 0, 0));
		}
	}


	UE_LOG(LogTemp, Warning, TEXT("GameManager Finish BeginPlay()."));
}



//-------------------------------------------------------------
// Name: EndPlay()
// Desc: �Q�[���I�����O�ɌĂ΂�鏈��
//-------------------------------------------------------------
void AGameManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (instance != nullptr)
	{
		instance = nullptr;
	}
}



//-------------------------------------------------------------
// Name: Tick()
// Desc: ���t���[���Ă΂��֐�
// Parm: DeltaTime / �O�t���[������̌o�ߎ���
//-------------------------------------------------------------
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



//-------------------------------------------------------------
// Name: GetInstance()
// Desc: instance�̃|�C���^��Ԃ��Bnullptr�̎��͌������s���B
//-------------------------------------------------------------
AGameManager* AGameManager::GetInstance()
{
	if (instance == nullptr)
	{
		if (GEngine->GameViewport != nullptr)
		{
			UWorld* world = GEngine->GameViewport->GetWorld();
			TSubclassOf<AGameManager> findClass = AGameManager::StaticClass();
			TArray<AActor*> actors;

			UGameplayStatics::GetAllActorsOfClass(world, findClass, actors);
			if (actors.Num())
			{
				instance = Cast<AGameManager>(actors[0]);
			}
		}
	}

	return instance;
}



//-------------------------------------------------------------
// Name: GetStageStatus()
// Desc: �w����W(Col, Row)�̃X�e�[�W�z��̒l��Ԃ��B�͈͊O�̎���-1��Ԃ�
//-------------------------------------------------------------
int AGameManager::GetStageStatus(int col, int row) const
{
	int retVal = -1;

	if (0 <= col && m_iCol > col && 0 <= row && m_iRow > row)
	{
		retVal = m_StageArray[row * m_iCol + col];
	}

	return retVal;
}



//-------------------------------------------------------------
// Name: SetStageStatus()
// Desc: 
//-------------------------------------------------------------
void AGameManager::SetStageStatus(int col, int row, EBlockType bt)
{
	if (0 <= col && m_iCol > col && 0 <= row && m_iRow > row)
	{
		m_StageArray[row*m_iCol + col] = (int)bt;
	}
}



//-------------------------------------------------------------
// ���͂ŌĂ΂��֐�
//-------------------------------------------------------------
void AGameManager::LeftClickEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("Left Click!"));

	APlayerController* pController = UGameplayStatics::GetPlayerController(this, 0);
	if (pController)
	{
		FVector WorldPos, WorldVec;
		pController->DeprojectMousePositionToWorld(WorldPos, WorldVec);

		//	�u���b�N�ړ��֘A����
		if (!m_bBlockMoving)	//	�ړ����̃u���b�N���Ȃ���Ώ����J�n
		{
			if (0 <= WorldPos.X && (m_iCol * BLOCK_SIZE) >= WorldPos.X && 0 <= WorldPos.Z && (m_iRow * BLOCK_SIZE) >= WorldPos.Z)
			{
				FHitResult	TraceResult(ForceInit);
				pController->GetHitResultUnderCursor(ECollisionChannel::ECC_WorldDynamic, false, TraceResult);
				AActor* HitActor = TraceResult.GetActor();
				if (HitActor)
				{
					ASuperBlock* HitBlock = Cast<ASuperBlock>(HitActor);
					if (HitBlock)
					{
						HitBlock->Clicked(WorldPos.X, WorldPos.Z);
					}
				}

			}
		}
	}

}