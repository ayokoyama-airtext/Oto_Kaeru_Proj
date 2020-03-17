/**
* @file		WaterBlock.h
* @brief	水ブロッククラス
* @author	Akihiro Yokoyama
*/
#pragma once

#include "CoreMinimal.h"
#include "Yokoyama/SuperBlock.h"
#include "WaterBlock.generated.h"



//-------------------------------------------------------------
// Name: AWaterBlock
// Desc: 
//-------------------------------------------------------------
UCLASS()
class HOOMY_2D_API AWaterBlock : public ASuperBlock
{
	GENERATED_BODY()
public:
	AWaterBlock(const FObjectInitializer& ObjectInitializer);
	virtual ~AWaterBlock();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
};
