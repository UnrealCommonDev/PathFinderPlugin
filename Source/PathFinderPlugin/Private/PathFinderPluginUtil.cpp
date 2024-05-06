// Fill out your copyright notice in the Description page of Project Settings.


#include "PathFinderPluginUtil.h"
#include "MGridDataManager.h"
#include "Kismet/KismetSystemLibrary.h"


void UPathFinderPluginUtil::DrawLoadedGridData(class MGridDataManager* inGridDataManager)
{
	if (nullptr == inGridDataManager) {
		return;
	}

	// ��Ÿ ������ ��´�
	MGridMetaData* metaData = inGridDataManager->GetGridMetaData();

	// �Ѿ�� �׸��� ������ ���͸� �������鼭 
	if (std::vector<MGridData*>* gridDataContainer = inGridDataManager->GetLoadedGridDataContainer())
	{
		// �׸��� �����Ͱ� �����ϴ���(�����Ͱ� �����ϴ°�� �浹ü ���)
		// ���� �ʿ�
		//bool existData = false;

		const int32 count = gridDataContainer->size();
		if (0 < count)
		{
			//existData = true;
			for (int32 i = 0; i < count; ++i) {
				DrawGridData(metaData, (*gridDataContainer)[i]);
			}
		}

		//SetDrawObstacleActorCollider(existData);
	}
}





void UPathFinderPluginUtil::DrawGridData(class MGridMetaData* inMetaData, class MGridData* inGridData)
{
	if (nullptr == inGridData) {
		return;
	}

	int32 gridSideSize = inMetaData->GetGridSideSize();

	// ���� ��ġ
	FVector startPos(inGridData->LeftTop.X, inGridData->LeftTop.Y, 0);

	DrawLines(
		GWorld,
		startPos,
		startPos + FVector(0, gridSideSize, 0),
		inMetaData->GridSideTileCount,
		FVector(inMetaData->TileSize, 0, 0),
		FLinearColor::Red);

	// horizontal
	DrawLines(
		GWorld,
		startPos,
		startPos + FVector(gridSideSize, 0, 0),
		inMetaData->GridSideTileCount,
		FVector(0, inMetaData->TileSize, 0),
		FLinearColor::Red);


	// Ÿ���� ������ ���鼭 �浹 �Ȱ��� ��ĥ�Ѵ�
	for (int32 y = 0; y < inMetaData->GridSideTileCount; ++y)
	{
		int32 startIndex = (y * inMetaData->GridSideTileCount);
		for (int32 x = 0; x < inMetaData->GridSideTileCount; ++x)
		{
			int32 index = startIndex + x;

			MTileData* tileData = inGridData->GetTileData(index);
			if (true == tileData->IsObstacle)
			{
				FVector centerPos;
				centerPos.X = inGridData->LeftTop.X + (x * inMetaData->TileSize) + (inMetaData->TileSize / 2);
				centerPos.Y = inGridData->LeftTop.Y + (y * inMetaData->TileSize) + (inMetaData->TileSize / 2);
				centerPos.Z = 0;

				UKismetSystemLibrary::DrawDebugPlane(
					GWorld,
					UKismetMathLibrary::MakePlaneFromPointAndNormal(centerPos, FVector::ZAxisVector),
					centerPos,
					inMetaData->TileSize / 2,
					FLinearColor::Blue,
					999999999);
			}
		}
	}
}



void UPathFinderPluginUtil::DrawLines(UObject* inObject, FVector inStart, FVector inEnd, int32 inCount, FVector inSpacing, const FLinearColor& inColor)
{
	for (int32 i = 0; i <= inCount; ++i)
	{
		UKismetSystemLibrary::DrawDebugLine(inObject, inStart, inEnd, inColor, 9999999, 1);

		inStart += inSpacing;
		inEnd += inSpacing;
	}
}