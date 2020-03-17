// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"
#include "Engine/Blueprint.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "ConstructorHelpers.h"
#include "StringTableCore.h"
#include "StringTable.h"
#include "Camera/CameraActor.h"
#include "Paper2D/Classes/PaperSpriteActor.h"
#include "SuperBlock.h"
#include "WallBlock.h"


//-------------------------------------------------------------
// Name: AGameManager()
// Desc: Ctor
//-------------------------------------------------------------
AGameManager::AGameManager(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer),m_iCol(0),m_iRow(0)
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
	for (int i = 1; i < (uint8)EBlockType::EMax; ++i)
	{
		static ConstructorHelpers::FClassFinder<ASuperBlock> BluePrintFile(*BlockBPPathArray[i]);
		if (BluePrintFile.Class)
		{
			m_BlocksRefArray[i] = (UClass*)BluePrintFile.Class;
		}
	}
#else
	//	エディター
	//	ブロック
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
// Desc: ゲーム開始直前に呼ばれる初期化処理
//-------------------------------------------------------------
void AGameManager::BeginPlay()
{
	Super::BeginPlay();
	
	//	ストリングテーブルからステージデータを読み込む
	if (m_pStringTable)
	{
		FName	tableID_ = m_pStringTable->GetStringTableId();
		m_ftStageText = FText::FromStringTable(tableID_, "stage");

		FString str_ = m_ftStageText.ToString();
		const TCHAR*  wchar_ = *str_;
		UE_LOG(LogTemp, Warning, TEXT("%s"), *str_);

		//	最初の2文字は行数と列数
		m_iCol = *wchar_++ - L'0';
		m_iRow = *wchar_++ - L'0';
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
// Name: Tick()
// Desc: 毎フレーム呼ばれる関数
// Parm: DeltaTime / 前フレームからの経過時間
//-------------------------------------------------------------
void AGameManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

