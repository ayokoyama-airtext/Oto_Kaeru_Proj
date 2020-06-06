// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Yokoyama/SuperBlock.h"
#include "StoneBlock.generated.h"

/**
 * 
 */
UCLASS()
class HOOMY_2D_API AStoneBlock : public ASuperBlock
{
	GENERATED_BODY()

public:
	AStoneBlock(const FObjectInitializer& objInit);
	virtual ~AStoneBlock();
	
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
};
