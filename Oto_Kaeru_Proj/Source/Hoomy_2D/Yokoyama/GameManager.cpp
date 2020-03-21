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
#include "Paper2D/Classes/PaperFlipbookActor.h"
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
	:AActor(ObjectInitializer),m_iCol(0),m_iRow(0), m_bBlockMoving(false), m_iGoalNum(0), m_iClearedGoalNum(0), m_bClearStage(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	//	スポーンさせるためにブロックBPの参照を取りに行く
	m_BlocksRefArray.Init(nullptr, (uint8)EBlockType::EMax);
#if UE_GAME
	//	ランタイム
	//	ブロック
	TArray<FString>	BlockBPPathArray = {"",
										"/Game/Working/Yokoyama/BP/WallBlockBP",
										"/Game/Working/Yokoyama/BP/WaterBlockBP",
										};
	FString TonosamaPath = "/Game/Working/Yokoyama/BP/ANotInFlipBook_BP";
	FString TamagoPath = "/Game/Working/Yokoyama/BP/TamagoFlipbook_BP";
	FString OtamaPath = "/Game/Working/Yokoyama/BP/OtamaFlipbook_BP";

	for (int i = 1; i < (uint8)EBlockType::EStart; ++i)
	{
		ConstructorHelpers::FClassFinder<ASuperBlock> BluePrintFile(*BlockBPPathArray[i]);
		if (BluePrintFile.Class)
		{
			m_BlocksRefArray[i] = (UClass*)BluePrintFile.Class;
		}
	}
	{
		ConstructorHelpers::FClassFinder<UBlueprint> BluePrintFile(*TonosamaPath);
		if (BluePrintFile.Class)
		{
			m_TonosamaRef = (UClass*)BluePrintFile.Class;
		}
	}
	{
		ConstructorHelpers::FClassFinder<UBlueprint> BluePrintFile(*TamagoPath);
		if (BluePrintFile.Class)
		{
			m_TamagoRef = (UClass*)BluePrintFile.Class;
		}
	}
	{
		ConstructorHelpers::FClassFinder<UBlueprint> BluePrintFile(*OtamaPath);
		if (BluePrintFile.Class)
		{
			m_OtamaRef = (UClass*)BluePrintFile.Class;
		}
	}

#else
	//	エディター
	//	ブロック
	TArray<FString>	BlockBPPathArray = {"",
										"Blueprint'/Game/Working/Yokoyama/BP/WallBlockBP.WallBlockBP'",
										"Blueprint'/Game/Working/Yokoyama/BP/WaterBlockBP.WaterBlockBP'",
										 };
	FString TonosamaPath = "Blueprint'/Game/Working/Yokoyama/BP/ANotInFlipBook_BP.ANotInFlipBook_BP'";
	FString TamagoPath = "Blueprint'/Game/Working/Yokoyama/BP/TamagoFlipbook_BP.TamagoFlipbook_BP'";
	FString OtamaPath = "Blueprint'/Game/Working/Yokoyama/BP/OtamaFlipbook_BP.OtamaFlipbook_BP'";
	
	for (int i = 1; i < (uint8)EBlockType::EStart; ++i)
	{
		ConstructorHelpers::FObjectFinder<UBlueprint> BluePrintFile(*BlockBPPathArray[i]);
		if (BluePrintFile.Object)
		{
			m_BlocksRefArray[i] = (UClass*)BluePrintFile.Object->GeneratedClass;
		}
	}
	{
		ConstructorHelpers::FObjectFinder<UBlueprint> BluePrintFile(*TonosamaPath);
		if (BluePrintFile.Object)
		{
			m_TonosamaRef = (UClass*)BluePrintFile.Object->GeneratedClass;
		}
	}
	{
		ConstructorHelpers::FObjectFinder<UBlueprint> BluePrintFile(*TamagoPath);
		if (BluePrintFile.Object)
		{
			m_TamagoRef = (UClass*)BluePrintFile.Object->GeneratedClass;
		}
	}
	{
		ConstructorHelpers::FObjectFinder<UBlueprint> BluePrintFile(*OtamaPath);
		if (BluePrintFile.Object)
		{
			m_OtamaRef = (UClass*)BluePrintFile.Object->GeneratedClass;
		}
	}
#endif
}



//-------------------------------------------------------------
// Name: BeginPlay()
// Desc: ゲーム開始直前に呼ばれる初期化処理
//-------------------------------------------------------------
void AGameManager::BeginPlay()
{
	Super::BeginPlay();

	if (instance == nullptr)
	{
		instance = this;
	}
	
	//	ストリングテーブルからステージデータを読み込む
	if (m_pStringTable)
	{
		FName	tableID_ = m_pStringTable->GetStringTableId();
		m_ftStageText = FText::FromStringTable(tableID_, "stage");

		FString str_ = m_ftStageText.ToString();
		const TCHAR*  wchar_ = *str_;
		UE_LOG(LogTemp, Warning, TEXT("%s"), *str_);

		//	最初の2文字は行数と列数
		m_iRow = *wchar_++ - L'0';
		m_iCol = *wchar_++ - L'0';
		//	最後まで数字を見ていく
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

	//	ブロックをスポーンさせる
	float width, height;
	float x, y, z;

	width = BLOCK_SIZE * m_iCol;
	height = BLOCK_SIZE * m_iRow;
	y = BLOCK_Y_COORD;

	if (m_StageArray.Num())
	{
		for (int i = 0; i < m_StageArray.Num(); ++i)
		{
			int blockID_ = m_StageArray[i];
			if (blockID_ != 0)
			{
				int col = i % m_iCol;
				int row = i / m_iRow;
				x = col * BLOCK_SIZE + BLOCK_SIZE * 0.5f;
				z = height - (row * BLOCK_SIZE + BLOCK_SIZE * 0.5f);
				switch (blockID_)
				{
				case (int)EBlockType::EStart:
					{
						APaperFlipbookActor* act_ = GetWorld()->SpawnActor<APaperFlipbookActor>(m_TonosamaRef, FVector(x, y, z), FRotator(0, 0, 0));
						if (act_)
						{
							//	スタートの情報を保持
							m_StartBlock.col = col;
							m_StartBlock.row = row;
						}
					}
					break;
				case (int)EBlockType::EGoal:
					{
						APaperFlipbookActor* tamago_ = GetWorld()->SpawnActor<APaperFlipbookActor>(m_TamagoRef, FVector(x, y, z), FRotator(0, 0, 0));
						APaperFlipbookActor* otama_ = GetWorld()->SpawnActor<APaperFlipbookActor>(m_OtamaRef, FVector(x, y, z), FRotator(0, 0, 0));
						if (tamago_ && otama_)
						{
							//	ゴールの情報を保持
							m_iGoalNum++;
							FBlockInfo bInfo;
							bInfo.col = col; bInfo.row = row;
							bInfo.Tamago = tamago_;
							bInfo.Otama = otama_;
							bInfo.Otama->SetActorHiddenInGame(true);
							m_GoalBlockArray.Emplace(bInfo);
							UE_LOG(LogTemp, Warning, TEXT("goal col:%d, row:%d"), col, row);
						}
					}
					break;
				default:
					{
						ASuperBlock* block_ = GetWorld()->SpawnActor<ASuperBlock>(m_BlocksRefArray[blockID_], FVector(x, y, z), FRotator(0, 0, 0));
						if (block_)
						{
							block_->SetPosition(col, row);
							block_->SetParent(this);
						}
					}
					break;
				}
				//ASuperBlock* block_ = GetWorld()->SpawnActor<ASuperBlock>(m_BlocksRefArray[blockID_], FVector(x, y, z), FRotator(0, 0, 0));
				//if (block_)
				//{
				//	block_->SetPosition(col, row);
				//	block_->SetParent(this);
				//	if (blockID_ == (int)EBlockType::EGoal)
				//	{
				//		//	ゴールの情報を保持
				//		m_iGoalNum++;
				//		FBlockInfo bInfo;
				//		bInfo.col = col; bInfo.row = row;
				//		m_GoalBlockArray.Emplace(bInfo);
				//		UE_LOG(LogTemp, Warning, TEXT("goal col:%d, row:%d"), col, row);
				//	}
				//	else if (blockID_ == (int)EBlockType::EStart)
				//	{
				//		//	スタートの情報を保持
				//		m_StartBlock.col = col;
				//		m_StartBlock.row = row;
				//	}
				//}
			}
		}
	}

	//	ワールド内のカメラを検索して配置
	TSubclassOf<ACameraActor> findClass;
	findClass = ACameraActor::StaticClass();
	TArray<AActor*> actorsArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), findClass, actorsArray);
	if (actorsArray.Num())
	{
		m_pCamera = Cast<ACameraActor>(actorsArray[0]);
		m_pCamera->SetActorLocation(FVector(width * 0.5f, 120.f, height * 0.5f));
	}

	//	背景を配置
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
// Desc: ゲーム終了直前に呼ばれる処理
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
// Desc: 毎フレーム呼ばれる関数
// Parm: DeltaTime / 前フレームからの経過時間
//-------------------------------------------------------------
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!m_bClearStage)
	{
		CheckClear();
	}
	
}



//-------------------------------------------------------------
// Name: GetInstance()
// Desc: instanceのポインタを返す。nullptrの時は検索を行う。
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
// Desc: 指定座標(Col, Row)のステージ配列の値を返す。範囲外の時は-1を返す
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
// Name: SetStageStatus()
// Desc: 
//-------------------------------------------------------------
void AGameManager::CheckClear()
{
	if (m_iGoalNum == m_iClearedGoalNum)
		return;

	int count = 0;
	for (auto goal : m_GoalBlockArray)
	{
		int *map = new int[m_iCol*m_iRow]();

		if (CheckBlock(goal.col, goal.row, map, true))
		{
			if (goal.Otama->bHidden)
			{
				goal.Tamago->SetActorHiddenInGame(true);
				goal.Otama->SetActorHiddenInGame(false);
			}
			count++;
		}
		else
		{
			if (goal.Tamago->bHidden)
			{
				goal.Tamago->SetActorHiddenInGame(false);
				goal.Otama->SetActorHiddenInGame(true);
			}
		}

		delete[]map;
	}
	m_iClearedGoalNum = count;

	if (m_iGoalNum == m_iClearedGoalNum)
	{
		m_bClearStage = true;
		UE_LOG(LogTemp, Warning, TEXT("Stage Clear!"));
	}

}



//-------------------------------------------------------------
// Name: CheckBlock()
// Desc: 
//-------------------------------------------------------------
bool AGameManager::CheckBlock(int x, int y, int *map, bool bFirstCheck)
{
	if (x < 0 || x > m_iCol-1 || y < 0 || y > m_iRow-1 || map[x + m_iCol * y] != 0)
		return false;

	if (m_StageArray[x + m_iCol * y] == (int)EBlockType::EStart)
		return true;

	if (!bFirstCheck)
	{
	if (m_StageArray[x + m_iCol * y] != (int)EBlockType::EWater)
		return false;
	}


	map[x + m_iCol * y] = 1;

	if (CheckBlock(x + 1, y, map, false))
		return true;
	if (CheckBlock(x, y + 1, map, false))
		return true;
	if (CheckBlock(x - 1, y, map, false))
		return true;
	if (CheckBlock(x, y - 1, map, false))
		return true;
		
	return false;
}



//-------------------------------------------------------------
// 入力で呼ばれる関数
//-------------------------------------------------------------
void AGameManager::LeftClickEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("Left Click!"));

	if (m_bClearStage)
		return;

	APlayerController* pController = UGameplayStatics::GetPlayerController(this, 0);
	if (pController)
	{
		FVector WorldPos, WorldVec;
		pController->DeprojectMousePositionToWorld(WorldPos, WorldVec);

		//	ブロック移動関連処理
		if (!m_bBlockMoving)	//	移動中のブロックがなければ処理開始
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