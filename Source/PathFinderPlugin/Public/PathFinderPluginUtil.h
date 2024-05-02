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
	// 모든 로드된 그리드 데이터 출력
	static void DrawLoadedGridData(class MGridDataManager* inGridDataManager);


protected:
	// 그리드 데이터를 출력
	static void DrawGridData(class MGridMetaData* inMetaData, class MGridData* inGridData);

	// 라인을 그린다
	static void DrawLines(UObject* inObject, FVector inStart, FVector inEnd, int32 inCount, FVector inSpacing, const FLinearColor& inColor);
};
