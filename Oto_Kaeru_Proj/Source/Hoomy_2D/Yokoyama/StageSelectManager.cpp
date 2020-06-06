// Fill out your copyright notice in the Description page of Project Settings.


#include "StageSelectManager.h"
#include "Engine/World.h"
#include "Engine/GameEngine.h"
#include "Kismet/GameplayStatics.h"
#include "SelectStageWidget.h"


// Sets default values
AStageSelectManager::AStageSelectManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStageSelectManager::BeginPlay()
{
	Super::BeginPlay();
	
	if (m_WidgetBPRef)
	{
		UWorld *world = GEngine->GameViewport->GetWorld();
		m_pWidget = Cast<USelectStageWidget>(USelectStageWidget::CreateWidgetInstance(*world, m_WidgetBPRef, "StageSelectUI"));
		if (m_pWidget)
		{
			m_pWidget->AddToViewport();
			m_pWidget->SetParent(this);
		}
	}
}

// Called every frame
void AStageSelectManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AStageSelectManager::StartLoadMap(FString nextMapPath)
{
	m_LoadMapPath = nextMapPath;
	m_pWidget->StartLoadMap();
}

void AStageSelectManager::LoadNextMap()
{
	UGameplayStatics::OpenLevel(this, FName(*m_LoadMapPath));
}

