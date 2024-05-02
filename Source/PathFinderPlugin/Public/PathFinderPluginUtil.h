// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PathFinderPluginUtil.generated.h"

/**
 * 
 */
UCLASS()
class PATHFINDERPLUGIN_API UPathFinderPluginUtil : public UObject
{
	GENERATED_BODY()
	
public:
	// ��� �ε�� �׸��� ������ ���
	static void DrawLoadedGridData(class MGridDataManager* inGridDataManager);


protected:
	// �׸��� �����͸� ���
	static void DrawGridData(class MGridMetaData* inMetaData, class MGridData* inGridData);

	// ������ �׸���
	static void DrawLines(UObject* inObject, FVector inStart, FVector inEnd, int32 inCount, FVector inSpacing, const FLinearColor& inColor);
};
