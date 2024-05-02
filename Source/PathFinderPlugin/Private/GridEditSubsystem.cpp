// Fill out your copyright notice in the Description page of Project Settings.


#include "GridEditSubsystem.h"
#include "ObstacleActor.h"
#include "MGridDataManager.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Kismet/KismetMathLibrary.h"
#include "PathFinderPluginUtil.h"

UGridEditSubsystem::UGridEditSubsystem()
	: IsDrawObstacleActorCollider(false)
{
}


void UGridEditSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 콘솔 커맨드 등록
	{
		// 콘솔 커맨드 등록
		IConsoleCommand* command = IConsoleManager::Get().RegisterConsoleCommand(
			TEXT("Grid.ObstacleActorCount"),
			TEXT(""),
			FConsoleCommandWithWorldAndArgsDelegate::CreateUObject(this, &UGridEditSubsystem::ConsoleCommand_ObstacleActorCount),
			ECVF_Cheat
		);
	}


	// 그리드 데이터 매니저 생성 및 초기화
	{
		FString contentsPath = FPaths::ProjectContentDir();
		FString gridDataPath = FPaths::Combine(contentsPath, TEXT("GridData/"));

		GridDataEditManager = new MGridDataEditManager();
		GridDataEditManager->InitGridDataManager(*gridDataPath);
	}
}

void UGridEditSubsystem::Deinitialize()
{
	if (nullptr != GridDataEditManager)
	{
		delete GridDataEditManager;
		GridDataEditManager = nullptr;
	}

	Super::Deinitialize();
}


void UGridEditSubsystem::LoadGridData(const FVector2D& inCenterPos, int32 inExtend)
{
	GridDataEditManager->LoadGridDataByPosition(MVector2(inCenterPos.X, inCenterPos.Y), inExtend);

	// 기존 디버깅 도형 삭제
	UKismetSystemLibrary::FlushPersistentDebugLines(GWorld);

	// 다시 그린다
	UPathFinderPluginUtil::DrawLoadedGridData(GridDataEditManager);

	// 우선 그냥 그린다
	SetDrawObstacleActorCollider(true);
}


void UGridEditSubsystem::UpdateLoadedGridData()
{
	std::vector<MBoxCollider*> colliderList;
	colliderList.reserve(ObstacleActorSet.Num());

	for (AObstacleActor* obstacle : ObstacleActorSet) {
		colliderList.push_back(obstacle->GetBoxCollider());
	}

	GridDataEditManager->UpdateLoadedGridData(colliderList);

	// 기존 디버깅 도형 삭제
	UKismetSystemLibrary::FlushPersistentDebugLines(GWorld);

	// 다시 그린다
	UPathFinderPluginUtil::DrawLoadedGridData(GridDataEditManager);

	// 우선 그냥 그린다
	SetDrawObstacleActorCollider(true);
}


void UGridEditSubsystem::AddObstacleActor(class AObstacleActor* inObstacleActor)
{
	inObstacleActor->SetDrawCollider(IsDrawObstacleActorCollider);
	ObstacleActorSet.Add(inObstacleActor);
}

void UGridEditSubsystem::RemoveObstacleActor(class AObstacleActor* inObstacleActor)
{
	ObstacleActorSet.Remove(inObstacleActor);
}


void UGridEditSubsystem::SetDrawObstacleActorCollider(bool inFlag)
{
	if (IsDrawObstacleActorCollider == inFlag) {
		return;
	}

	// 플래그 설정
	IsDrawObstacleActorCollider = inFlag;

	// 등록된 액터를 돌면서 재설정
	for (AObstacleActor* obstacle : ObstacleActorSet) {
		obstacle->SetDrawCollider(inFlag);
	}
}

void UGridEditSubsystem::ConsoleCommand_ObstacleActorCount(const TArray<FString>& Args, UWorld* World)
{
	FString log = FString::Format(TEXT("Register ObstacleActor Count : {0}"), { ObstacleActorSet.Num() });
	UKismetSystemLibrary::PrintString(GetWorld(), log, true, true, FLinearColor::Green, 2.0f);
}