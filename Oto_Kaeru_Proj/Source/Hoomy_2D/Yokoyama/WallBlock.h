/**
* @file		WallBlock.h
* @brief	壁ブロッククラス
* @author	Akihiro Yokoyama
*/
#pragma once

#include "CoreMinimal.h"
#include "Yokoyama/SuperBlock.h"
#include "WallBlock.generated.h"



//-------------------------------------------------------------
// Name: AWallBlock
// Desc: 
//-------------------------------------------------------------
UCLASS()
class HOOMY_2D_API AWallBlock : public ASuperBlock
{
	GENERATED_BODY()
	
public:
	AWallBlock(const FObjectInitializer& ObjectInitializer);
	virtual ~AWallBlock();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
