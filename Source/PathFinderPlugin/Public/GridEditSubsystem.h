// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "GridEditSubsystem.generated.h"



//----------------------------------------------------------
// 그리드 편집에 사용되는 서브 시스템
// - Obstacle 액터 관리
// - 그리드 정보 관리
// 
// 참조
// GEditor->GetEditorSubsystem<UGridEditSubsystem>();
//----------------------------------------------------------
UCLASS()
class PATHFINDERPLUGIN_API UGridEditSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
	
public:
	UGridEditSubsystem();


public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	// 특정 영역의 그리드 정보를 로드
	void LoadGridData(const FVector2D& inCenterPos, int32 inExtend);

	// 로드된 데이터의 충돌체크를 갱신
	void UpdateLoadedGridData();

	//-----------------------------------------------------------------
	// Obstacle 인터페이스
	//-----------------------------------------------------------------
	// Obstacle 액터 등록 / 해제
	void AddObstacleActor(class AObstacleActor* inObstacleActor);
	void RemoveObstacleActor(class AObstacleActor* inObstacleActor);


	

	class MGridDataEditManager* GetGridDataEditManager() {
		return GridDataEditManager;
	}

protected:
	// 장애물 액터의 충돌체를 그릴지 
	void SetDrawObstacleActorCollider(bool inFlag);


	//--------------------------------------------------------------
	// 테스트용 콘솔 커맨드 
	//--------------------------------------------------------------
	// 등록된 장애물 액터 카운트를 출력
	void ConsoleCommand_ObstacleActorCount(const TArray<FString>& Args, UWorld* World);

protected:
	// Obstacle 컨테이너
	UPROPERTY()
	TSet<class AObstacleActor*> ObstacleActorSet;
		
	// 그리드 정보 매니저
	class MGridDataEditManager* GridDataEditManager = nullptr;


	// 플래그
	bool IsDrawObstacleActorCollider;
};
