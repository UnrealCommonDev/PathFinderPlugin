// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MPathFindData.h"
#include "GridEditWidget.generated.h"


// 맵 위치 정보
enum class EGridEditPosType
{
	Center = 0,
	LeftTop = 1,
};


// 그리드 에디트 상태
enum class EGridEditMode : uint8
{
	None = 0,
	MetaDataEditMode,		// 메타 데이터 에디트 모드
	GridDataEditMode,		// 그리드 데이터 에디트 모드
};


/**
 * 
 */
UCLASS()
class PATHFINDERPLUGIN_API UGridEditWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;


	//------------------------------------------------------------
	// 버튼 핸들러
	//------------------------------------------------------------
	

	UFUNCTION()
	void OnChanged_MapPos(const FText& inText);




	// 경로를 얻는다
	const FString& GetPathFindDataDir() const {
		return PathFindDataDir;
	}


	// 더티 플래그 설정
	void SetIsDirty();

	// 에디트 모드 변경
	void ChangeGridEditMode(EGridEditMode inEditMode);



	//----------------------------------------------------------
	// 사용 함수
	//----------------------------------------------------------
	// 위젯 <-> 데이타
	void UpdateGridMetaData(MGridMetaData& inMetaData, bool inToWidget);


	// 텍스트 -> int
	int32 ConvertToInt(const FText& inText);
	FText ConvertToText(int32 inValue);


	//-----------------------------------------------------------
	// 이벤트 처리
	//-----------------------------------------------------------
	// 타일 사이즈 변경시 호출
	UFUNCTION()
	void OnChangedTileSize(const FText& inText);

	// 타일 카운트 변경시 호출
	UFUNCTION()
	void OnChangedGridSideTileCount(const FText& inText);

	// 메타 데이터 생성 버튼 클릭
	UFUNCTION()
	void OnClickedCreateMetaData();

	// 그리드 로드 버튼 클릭
	UFUNCTION()
	void OnClickedLoadGridFromCameraPos();

	UFUNCTION()
	void OnClickedUpdateLoadedGridData();


	void OnUpdateTimer();


protected:

	//------------------------------------------------------------
	// 데이터 디렉토리
	//------------------------------------------------------------ 
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_GridDataPath = nullptr;


	//------------------------------------------------------------
	// 타일 사이즈
	//------------------------------------------------------------
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ETB_TileSize = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_TileSize = nullptr;

	//------------------------------------------------------------
	// 그리드
	//------------------------------------------------------------
	// 그리드 타일 개수
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ETB_GridSideTileCount = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_GridTileCount = nullptr;


	// 위젯 스위쳐
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WS_EditMode = nullptr;


	//------------------------------------------------------------
	// 메다 데이터 에디트 모드
	//------------------------------------------------------------
	// 메타 데이터 생성
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_CreateMetaData = nullptr;


	//------------------------------------------------------------
	// 그리드 데이터 에디트 모드
	//------------------------------------------------------------
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_ViewCameraPos = nullptr;

	// 메타 데이터 생성
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_LoadGridFromCameraPos = nullptr;

	// 메타 데이터 생성
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_UpdateLoadedGridData = nullptr;


	//------------------------------------------------------------
	// 패스 파인드 데이터
	//------------------------------------------------------------
	// 대상 디렉토리
	FString PathFindDataDir;

	// 데이터
	MPathFindData PathFindData;

	// 타이머 핸들
	FTimerHandle TimerHandle;

	// 패스 파인드정보를 다시 그릴것인지
	bool IsDirty = false;





	//------------------------------------------------------------
	//  
	//------------------------------------------------------------
	// 에디트 모드
	EGridEditMode EditMode = EGridEditMode::None;
};
