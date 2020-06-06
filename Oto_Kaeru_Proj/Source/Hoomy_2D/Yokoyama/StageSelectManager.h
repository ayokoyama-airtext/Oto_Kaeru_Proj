// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StageSelectManager.generated.h"

UCLASS()
class HOOMY_2D_API AStageSelectManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStageSelectManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void StartLoadMap(FString nextMapPath);

	UFUNCTION()
		void LoadNextMap();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<class USelectStageWidget>	m_WidgetBPRef;
	UPROPERTY(VisibleAnywhere)
		class USelectStageWidget*	m_pWidget;

	UPROPERTY()
		FString	m_LoadMapPath;
};
