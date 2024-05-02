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

	// �ܼ� Ŀ�ǵ� ���
	{
		// �ܼ� Ŀ�ǵ� ���
		IConsoleCommand* command = IConsoleManager::Get().RegisterConsoleCommand(
			TEXT("Grid.ObstacleActorCount"),
			TEXT(""),
			FConsoleCommandWithWorldAndArgsDelegate::CreateUObject(this, &UGridEditSubsystem::ConsoleCommand_ObstacleActorCount),
			ECVF_Cheat
		);
	}


	// �׸��� ������ �Ŵ��� ���� �� �ʱ�ȭ
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

	// ���� ����� ���� ����
	UKismetSystemLibrary::FlushPersistentDebugLines(GWorld);

	// �ٽ� �׸���
	UPathFinderPluginUtil::DrawLoadedGridData(GridDataEditManager);

	// �켱 �׳� �׸���
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

	// ���� ����� ���� ����
	UKismetSystemLibrary::FlushPersistentDebugLines(GWorld);

	// �ٽ� �׸���
	UPathFinderPluginUtil::DrawLoadedGridData(GridDataEditManager);

	// �켱 �׳� �׸���
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

	// �÷��� ����
	IsDrawObstacleActorCollider = inFlag;

	// ��ϵ� ���͸� ���鼭 �缳��
	for (AObstacleActor* obstacle : ObstacleActorSet) {
		obstacle->SetDrawCollider(inFlag);
	}
}

void UGridEditSubsystem::ConsoleCommand_ObstacleActorCount(const TArray<FString>& Args, UWorld* World)
{
	FString log = FString::Format(TEXT("Register ObstacleActor Count : {0}"), { ObstacleActorSet.Num() });
	UKismetSystemLibrary::PrintString(GetWorld(), log, true, true, FLinearColor::Green, 2.0f);
}