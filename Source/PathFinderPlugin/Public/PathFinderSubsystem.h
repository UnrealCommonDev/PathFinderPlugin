// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PathFinderSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class PATHFINDERPLUGIN_API UPathFinderSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	//--------------------------------------------------------------------
	// 서브 시스템 인터페이스
	//--------------------------------------------------------------------
	// 초기화
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;

	//--------------------------------------------------------------------
	// 길찾기 인터페이스
	//--------------------------------------------------------------------
	// 특정 위치의 그리드 데이터를 로드
	bool LoadGridDataByPosition(const FVector& inPos, const int32 inExtend);

	// 경로를 찾는다
	bool FindPath(TArray<FVector>& inList, const FVector& inStartPos, const FVector& inEndPos);


	//--------------------------------------------------------------------
	// 디버깅
	//--------------------------------------------------------------------
	void SetDrawLoadedGridData(bool inFlag);


	

protected:
	// 패스 파인더
	class MPathFinder* PathFinder = nullptr;


	// 로드된 그리드 데이터를 그릴것인지
	bool IsDrawLoadedGridData = false;
};
