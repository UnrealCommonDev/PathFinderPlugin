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
	// ���� �ý��� �������̽�
	//--------------------------------------------------------------------
	// �ʱ�ȭ
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;

	//--------------------------------------------------------------------
	// ��ã�� �������̽�
	//--------------------------------------------------------------------
	// Ư�� ��ġ�� �׸��� �����͸� �ε�
	bool LoadGridDataByPosition(const FVector& inPos, const int32 inExtend);

	// ��θ� ã�´�
	bool FindPath(TArray<FVector>& inList, const FVector& inStartPos, const FVector& inEndPos);


	//--------------------------------------------------------------------
	// �����
	//--------------------------------------------------------------------
	void SetDrawLoadedGridData(bool inFlag);


	

protected:
	// �н� ���δ�
	class MPathFinder* PathFinder = nullptr;


	// �ε�� �׸��� �����͸� �׸�������
	bool IsDrawLoadedGridData = false;
};
