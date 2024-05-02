// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinderSubsystem.h"
#include "MPathFinder.h"
#include "PathFinderPluginUtil.h"
#include "Kismet/KismetSystemLibrary.h"


void UPathFinderSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	FString contentsPath = FPaths::ProjectContentDir();
	FString gridDataPath = FPaths::Combine(contentsPath, TEXT("GridData/"));

	PathFinder = new MPathFinder();
	PathFinder->InitPathFinder(*gridDataPath);
}



void UPathFinderSubsystem::Deinitialize()
{
	if (nullptr != PathFinder)
	{
		delete PathFinder;
		PathFinder = nullptr;
	}


	Super::Deinitialize();
}




bool UPathFinderSubsystem::LoadGridDataByPosition(const FVector& inPos, const int32 inExtend)
{
	MVector2 pos(inPos.X, inPos.Y);
	return PathFinder->LoadGridDataByPosition(pos, inExtend);
}


bool UPathFinderSubsystem::FindPath(TArray<FVector>& inList, const FVector& inStartPos, const FVector& inEndPos)
{
	// 들어온 정보 초기화
	inList.Reset();

	std::vector<MVector2> posList;
	MBOOL isSuccess = PathFinder->FindPath(posList, MVector2(inStartPos.X, inStartPos.Y), MVector2(inEndPos.X, inEndPos.Y));

	if (MTRUE == isSuccess)
	{
		const MINT32 count = posList.size();
		for (MINT32 i = 0; i < count; ++i) {
			inList.Emplace(FVector(posList[i].X, posList[i].Y, 0));
		}
	}

	return isSuccess;
}


void UPathFinderSubsystem::SetDrawLoadedGridData(bool inFlag)
{
	IsDrawLoadedGridData = inFlag;

	// 기존 정보 제거
	UKismetSystemLibrary::FlushPersistentDebugLines(GWorld);
	if (true == IsDrawLoadedGridData)
	{
		UPathFinderPluginUtil::DrawLoadedGridData(PathFinder->GetGridDataManager());
	}
	
}