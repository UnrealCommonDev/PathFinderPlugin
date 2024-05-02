// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "GridEditSubsystem.generated.h"



//----------------------------------------------------------
// �׸��� ������ ���Ǵ� ���� �ý���
// - Obstacle ���� ����
// - �׸��� ���� ����
// 
// ����
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

	// Ư�� ������ �׸��� ������ �ε�
	void LoadGridData(const FVector2D& inCenterPos, int32 inExtend);

	// �ε�� �������� �浹üũ�� ����
	void UpdateLoadedGridData();

	//-----------------------------------------------------------------
	// Obstacle �������̽�
	//-----------------------------------------------------------------
	// Obstacle ���� ��� / ����
	void AddObstacleActor(class AObstacleActor* inObstacleActor);
	void RemoveObstacleActor(class AObstacleActor* inObstacleActor);


	

	class MGridDataEditManager* GetGridDataEditManager() {
		return GridDataEditManager;
	}

protected:
	// ��ֹ� ������ �浹ü�� �׸��� 
	void SetDrawObstacleActorCollider(bool inFlag);


	//--------------------------------------------------------------
	// �׽�Ʈ�� �ܼ� Ŀ�ǵ� 
	//--------------------------------------------------------------
	// ��ϵ� ��ֹ� ���� ī��Ʈ�� ���
	void ConsoleCommand_ObstacleActorCount(const TArray<FString>& Args, UWorld* World);

protected:
	// Obstacle �����̳�
	UPROPERTY()
	TSet<class AObstacleActor*> ObstacleActorSet;
		
	// �׸��� ���� �Ŵ���
	class MGridDataEditManager* GridDataEditManager = nullptr;


	// �÷���
	bool IsDrawObstacleActorCollider;
};
