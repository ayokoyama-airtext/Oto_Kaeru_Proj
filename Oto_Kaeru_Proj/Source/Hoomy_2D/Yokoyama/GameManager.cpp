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
#include "GameUserWidget.h"

//-------------------------------------------------------------
// Static Fields Init
//-------------------------------------------------------------
AGameManager* AGameManager::instance = nullptr;



//-------------------------------------------------------------
// Name: AGameManager()
// Desc: Ctor
//-------------------------------------------------------------
AGameManager::AGameManager(const FObjectInitializer& ObjectInitializer)
	:AActor(ObjectInitializer),m_iCol(0),m_iRow(0), m_bBlockMoving(false), m_iGoalNum(0), m_iClearedGoalNum(0), m_bClearStage(false), m_bGameOver(false), m_iClickCount(0), m_iMaxClickNum(0)
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
	FString TonosamaInWaterPath = "Blueprint'/Game/Working/Yokoyama/BP/AInFlipBook_BP.AInFlipBook_BP'";

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
		ConstructorHelpers::FObjectFinder<UBlueprint> BluePrintFile(*TonosamaInWaterPath);
		if (BluePrintFile.Object)
		{
			m_TonosamaInWaterRef = (UClass*)BluePrintFile.Object->GeneratedClass;
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

		//	最初は行数と列数
		m_iRow += (*wchar_++ - L'0') * 10;	m_iRow += (*wchar_++ - L'0');
		m_iCol += (*wchar_++ - L'0') * 10;	m_iCol += (*wchar_++ - L'0');
		//	クリック回数のチェック
		m_iMaxClickNum += (*wchar_++ - L'0') * 10;
		m_iMaxClickNum += (*wchar_++ - L'0');


		//	ブロックをスポーンさせる
		float width, height;
		float x, y, z;

		width = BLOCK_SIZE * m_iCol;
		height = BLOCK_SIZE * m_iRow;
		y = BLOCK_Y_COORD;

		m_BlockArray.Init(nullptr, m_iCol*m_iRow);
		m_fWidth = width;
		m_fHeight = height;

		//	最後まで数字を見ていく
		while (*wchar_ != L'\0')
		{
			int c_ = *wchar_;
			if (L'0' <= c_ && c_ <= L'9')
			{
				m_StageArray.Emplace(c_ - L'0');
				int i = m_StageArray.Num() - 1;
				int blockID_ = c_ - L'0';
				if (blockID_ != 0)
				{
					int col = i % m_iCol;
					int row = i / m_iRow;
					x = col * BLOCK_SIZE + BLOCK_SIZE * 0.5f;
					z = height - (row * BLOCK_SIZE + BLOCK_SIZE * 0.5f);
					switch (blockID_)
					{
						case (int)EBlockType::EEmpty:
							break;
						case (int)EBlockType::EStart:
						{
							APaperFlipbookActor* act_ = GetWorld()->SpawnActor<APaperFlipbookActor>(m_TonosamaRef, FVector(x, y-5, z), FRotator(0, 0, 0));
							APaperFlipbookActor* actInWater_ = GetWorld()->SpawnActor<APaperFlipbookActor>(m_TonosamaInWaterRef, FVector(x, y-5, z), FRotator(0, 0, 0));
							ASuperBlock* waterBlock_ = GetWorld()->SpawnActor<ASuperBlock>(m_BlocksRefArray[(int)EBlockType::EWater], FVector(x, y, z), FRotator(0, 0, 0));
							if (act_ && actInWater_ && waterBlock_)
							{
								//	スタートの情報を保持
								m_StartBlock.col = col;
								m_StartBlock.row = row;
								m_StartBlock.Tonosama = act_;
								m_StartBlock.TonosamaInWater = actInWater_;
								actInWater_->SetActorHiddenInGame(true);
								m_StartBlock.WaterBlock = waterBlock_;
								waterBlock_->SetMovePossibility(false);
								waterBlock_->SetActorHiddenInGame(true);
							}
						}
						break;
						case (int)EBlockType::EGoal:
						{
							APaperFlipbookActor* tamago_ = GetWorld()->SpawnActor<APaperFlipbookActor>(m_TamagoRef, FVector(x, y-5, z), FRotator(0, 0, 0));
							APaperFlipbookActor* otama_ = GetWorld()->SpawnActor<APaperFlipbookActor>(m_OtamaRef, FVector(x, y-5, z), FRotator(0, 0, 0));
							ASuperBlock* waterBlock_ = GetWorld()->SpawnActor<ASuperBlock>(m_BlocksRefArray[(int)EBlockType::EWater], FVector(x, y, z), FRotator(0, 0, 0));
							if (tamago_ && otama_ && waterBlock_)
							{
								//	ゴールの情報を保持
								m_iGoalNum++;
								FBlockInfo bInfo;
								bInfo.col = col; bInfo.row = row;
								bInfo.Tamago = tamago_;
								bInfo.Otama = otama_;
								bInfo.Otama->SetActorHiddenInGame(true);
								bInfo.WaterBlock = waterBlock_;
								waterBlock_->SetMovePossibility(false);
								waterBlock_->SetActorHiddenInGame(true);
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
								m_BlockArray[i] = block_;
								block_->SetPosition(col, row);
								block_->SetParent(this);
								block_->SetMoveInfo(*(wchar_ + 1));
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
			wchar_++;
		}
	}

	////	ブロックをスポーンさせる
	//float width, height;
	//float x, y, z;
	//width = BLOCK_SIZE * m_iCol;
	//height = BLOCK_SIZE * m_iRow;
	//y = BLOCK_Y_COORD;
	//m_BlockArray.Init(nullptr, m_iCol*m_iRow);
	//m_fWidth = width;
	//m_fHeight = height;
	//if (m_StageArray.Num())
	//{
	//	for (int i = 0; i < m_StageArray.Num(); ++i)
	//	{
	//		int blockID_ = m_StageArray[i];
	//		if (blockID_ != 0)
	//		{
	//			int col = i % m_iCol;
	//			int row = i / m_iRow;
	//			x = col * BLOCK_SIZE + BLOCK_SIZE * 0.5f;
	//			z = height - (row * BLOCK_SIZE + BLOCK_SIZE * 0.5f);
	//			switch (blockID_)
	//			{
	//			case (int)EBlockType::EEmpty:
	//				break;
	//			case (int)EBlockType::EStart:
	//				{
	//					APaperFlipbookActor* act_ = GetWorld()->SpawnActor<APaperFlipbookActor>(m_TonosamaRef, FVector(x, y, z), FRotator(0, 0, 0));
	//					if (act_)
	//					{
	//						//	スタートの情報を保持
	//						m_StartBlock.col = col;
	//						m_StartBlock.row = row;
	//					}
	//				}
	//				break;
	//			case (int)EBlockType::EGoal:
	//				{
	//					APaperFlipbookActor* tamago_ = GetWorld()->SpawnActor<APaperFlipbookActor>(m_TamagoRef, FVector(x, y, z), FRotator(0, 0, 0));
	//					APaperFlipbookActor* otama_ = GetWorld()->SpawnActor<APaperFlipbookActor>(m_OtamaRef, FVector(x, y, z), FRotator(0, 0, 0));
	//					if (tamago_ && otama_)
	//					{
	//						//	ゴールの情報を保持
	//						m_iGoalNum++;
	//						FBlockInfo bInfo;
	//						bInfo.col = col; bInfo.row = row;
	//						bInfo.Tamago = tamago_;
	//						bInfo.Otama = otama_;
	//						bInfo.Otama->SetActorHiddenInGame(true);
	//						m_GoalBlockArray.Emplace(bInfo);
	//						UE_LOG(LogTemp, Warning, TEXT("goal col:%d, row:%d"), col, row);
	//					}
	//				}
	//				break;
	//			default:
	//				{
	//					ASuperBlock* block_ = GetWorld()->SpawnActor<ASuperBlock>(m_BlocksRefArray[blockID_], FVector(x, y, z), FRotator(0, 0, 0));
	//					if (block_)
	//					{
	//						m_BlockArray[i] = block_;
	//						block_->SetPosition(col, row);
	//						block_->SetParent(this);
	//					}
	//				}
	//				break;
	//			}
	//			//ASuperBlock* block_ = GetWorld()->SpawnActor<ASuperBlock>(m_BlocksRefArray[blockID_], FVector(x, y, z), FRotator(0, 0, 0));
	//			//if (block_)
	//			//{
	//			//	block_->SetPosition(col, row);
	//			//	block_->SetParent(this);
	//			//	if (blockID_ == (int)EBlockType::EGoal)
	//			//	{
	//			//		//	ゴールの情報を保持
	//			//		m_iGoalNum++;
	//			//		FBlockInfo bInfo;
	//			//		bInfo.col = col; bInfo.row = row;
	//			//		m_GoalBlockArray.Emplace(bInfo);
	//			//		UE_LOG(LogTemp, Warning, TEXT("goal col:%d, row:%d"), col, row);
	//			//	}
	//			//	else if (blockID_ == (int)EBlockType::EStart)
	//			//	{
	//			//		//	スタートの情報を保持
	//			//		m_StartBlock.col = col;
	//			//		m_StartBlock.row = row;
	//			//	}
	//			//}
	//		}
	//	}
	//}
	
	//	ワールド内のカメラを検索して配置
	TSubclassOf<ACameraActor> findClass;
	findClass = ACameraActor::StaticClass();
	TArray<AActor*> actorsArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), findClass, actorsArray);
	if (actorsArray.Num())
	{
		m_pCamera = Cast<ACameraActor>(actorsArray[0]);
		m_pCamera->SetActorLocation(FVector(m_fWidth * 0.5f, CAMERA_Y_COORD, m_fHeight * 0.5f));
	}

	//	背景を配置
	{
		if (m_BGBPRef)
		{
			APaperSpriteActor* bg_ = GetWorld()->SpawnActor<APaperSpriteActor>(m_BGBPRef, FVector(m_fWidth *0.5f, 0, m_fHeight * 0.5f), FRotator(0, 0, 0));
		}
	}

	//	Widgetを設定
	if (m_WidgetBPRef)
	{
		UWorld *world = GEngine->GameViewport->GetWorld();
		m_pWidget = Cast<UGameUserWidget>(UGameUserWidget::CreateWidgetInstance(*world, m_WidgetBPRef, "GameMainUI"));
		if (m_pWidget)
		{
			m_pWidget->AddToViewport();
			m_pWidget->SetMaxClickNum(m_iMaxClickNum);
			m_pWidget->UpdateClickNumText(0);
		}
	}

	//	トノサマと水ブロックが隣接しているかチェック
	CheckWaterBlockAroundTonosama();


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

	//	クリア&ゲームオーバーチェック
	if (!m_bClearStage && !m_bGameOver)
	{
		CheckClear();
	}

	//	ゲームオーバーチェック
	if (!m_bClearStage && !m_bGameOver)
	{
		if (m_iClickCount > m_iMaxClickNum)
		{
			m_bGameOver = true;
			m_pWidget->ShowGameOverImage();
			UE_LOG(LogTemp, Warning, TEXT("GameOver!"));
		}
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
void AGameManager::SetBlockStatus(int col, int row, ASuperBlock* pBlock)
{
	if (0 <= col && m_iCol > col && 0 <= row && m_iRow > row)
	{
		m_BlockArray[row*m_iCol + col] = pBlock;
	}
}



//-------------------------------------------------------------
// Name: CheckClear()
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
				goal.WaterBlock->SetActorHiddenInGame(false);
			}
			count++;
		}
		else
		{
			if (goal.Tamago->bHidden)
			{
				goal.Tamago->SetActorHiddenInGame(false);
				goal.Otama->SetActorHiddenInGame(true);
				goal.WaterBlock->SetActorHiddenInGame(true);
			}
		}

		delete[]map;
	}
	m_iClearedGoalNum = count;

	if (m_iGoalNum == m_iClearedGoalNum)
	{
		m_bClearStage = true;
		m_pWidget->ShowClearImage();
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
// Name: CheckBlock()
// Desc: 
//-------------------------------------------------------------
void AGameManager::CheckWaterBlockAroundTonosama()
{
	if (!m_StartBlock.WaterBlock)
		return;

	int col_ = m_StartBlock.col;
	int row_ = m_StartBlock.row;
	int waterID_ = (int)EBlockType::EWater;
	bool bWaterBlock = false;

	
	if (col_ - 1 >= 0)
	{
		if (waterID_ == GetStageStatus(col_ - 1, row_))
		{
			bWaterBlock = true;
			goto RESULT;
		}
	}
	if (col_ + 1 < m_iCol)
	{
		if (waterID_ == GetStageStatus(col_ + 1, row_))
		{
			bWaterBlock = true;
			goto RESULT;
		}
	}
	if (row_ - 1 >= 0)
	{
		if (waterID_ == GetStageStatus(col_, row_-1))
		{
			bWaterBlock = true;
			goto RESULT;
		}
	}
	if (row_ + 1 < m_iRow)
	{
		if (waterID_ == GetStageStatus(col_, row_ + 1))
		{
			bWaterBlock = true;
			goto RESULT;
		}
	}


	RESULT:
	if (bWaterBlock)
	{
		if (m_StartBlock.WaterBlock->bHidden)
		{
			m_StartBlock.WaterBlock->SetActorHiddenInGame(false);
			m_StartBlock.Tonosama->SetActorHiddenInGame(true);
			m_StartBlock.TonosamaInWater->SetActorHiddenInGame(false);
		}
	}
	else
	{
		if (!m_StartBlock.WaterBlock->bHidden)
		{
			m_StartBlock.WaterBlock->SetActorHiddenInGame(true);
			m_StartBlock.Tonosama->SetActorHiddenInGame(false);
			m_StartBlock.TonosamaInWater->SetActorHiddenInGame(true);
		}
	}
}



//-------------------------------------------------------------
// Name: IncreaseClickCount()
// Desc: 
//-------------------------------------------------------------
void AGameManager::IncreaseClickCount()
{
	m_iClickCount++;
	m_pWidget->UpdateClickNumText(m_iClickCount);
	CheckWaterBlockAroundTonosama();
}



//-------------------------------------------------------------
// 入力で呼ばれる関数
//-------------------------------------------------------------
void AGameManager::LeftClickEvent()
{
	UE_LOG(LogTemp, Warning, TEXT("Left Click!"));

	if (m_bClearStage || m_bGameOver)
		return;

	APlayerController* pController = UGameplayStatics::GetPlayerController(this, 0);
	if (pController)
	{
		//	マウスの座標をスクリーン座標からワールド座標に変換
		FVector WorldPos, WorldVec;
		pController->DeprojectMousePositionToWorld(WorldPos, WorldVec);
		UE_LOG(LogTemp, Warning, TEXT("Cursor Pos X:%.2f Y:%.2f Z:%.2f"), WorldPos.X, WorldPos.Y, WorldPos.Z);

		//	ブロック移動関連処理
		if (!m_bBlockMoving)	//	移動中のブロックがなければ処理開始
		{
			//	ブロックが置いてある面との交点座標を計算
			FVector ClickPos = FMath::LinePlaneIntersection(WorldPos, WorldPos + 10000.0f * WorldVec, FVector(0, BLOCK_Y_COORD, 0), FVector(0, 1, 0));
			UE_LOG(LogTemp, Warning, TEXT("Click Pos X:%.2f Y:%.2f Z:%.2f"), ClickPos.X, ClickPos.Y, ClickPos.Z);

			if (0 <= ClickPos.X && (m_fWidth) >= ClickPos.X && 0 <= ClickPos.Z && (m_fHeight) >= ClickPos.Z)
			{
				//	座標から配列添え字番号を計算
				int col_ = (int)ClickPos.X / (int)BLOCK_SIZE, row_ = (int)(m_fHeight - ClickPos.Z) / (int)BLOCK_SIZE;
				int index_ = col_ + m_iCol * row_;
				if (m_BlockArray[index_])
				{
					//	ブロックにクリックされたことを伝える
					m_BlockArray[index_]->Clicked(ClickPos.X, ClickPos.Z);
				}

			}
		}
	}

}