// Fill out your copyright notice in the Description page of Project Settings.


#include "GridEditWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Components/ComboBoxString.h"
#include "Components/WidgetSwitcher.h"

#include "Kismet/KismetSystemLibrary.h"


#include "GridEditSubsystem.h"
#include "MGridDataManager.h"
#include "MBoxCollider.h"

#include "Kismet/KismetMathLibrary.h"

#include "LevelEditorViewport.h"

void UGridEditWidget::NativeConstruct()
{
	// 그리드 데이터 매니저를 얻는다 
	UGridEditSubsystem* subSystem = GEditor->GetEditorSubsystem<UGridEditSubsystem>();
	MGridDataEditManager* editManager = subSystem->GetGridDataEditManager();

	// 경로 설정
	TB_GridDataPath->SetText(FText::FromString(editManager->GetGridDataPath().GetStr()));
	TB_GridDataPath->SetIsEnabled(false);
	
	// 타일 사이즈 변경시 처리
	ETB_TileSize->OnTextChanged.AddDynamic(this, &UGridEditWidget::OnChangedTileSize);

	// 그리드 타일 개수 변경시 처리
	ETB_GridSideTileCount->OnTextChanged.AddDynamic(this, &UGridEditWidget::OnChangedGridSideTileCount);

	// 메타 데이터 생성 버튼 처리
	BTN_CreateMetaData->OnClicked.AddDynamic(this, &UGridEditWidget::OnClickedCreateMetaData);

	// 카메라 위치의 그리드 데이터 로드 
	BTN_LoadGridFromCameraPos->OnClicked.AddDynamic(this, &UGridEditWidget::OnClickedLoadGridFromCameraPos);
	

	BTN_UpdateLoadedGridData->OnClicked.AddDynamic(this, &UGridEditWidget::OnClickedUpdateLoadedGridData);

	// 설정할 메타 데이터
	MGridMetaData metaData;
	EGridEditMode editMode;

	// 메타 데이터 로드
	if (MTRUE == editManager->LoadMetaData())
	{
		// 로드된 메타 데이터 설정
		metaData = *(editManager->GetGridMetaData());

		// 모드 설정
		editMode = EGridEditMode::GridDataEditMode;
	}
	else
	{
		// 메타 데이터가 존재하지 않거나 무효한 경우
		
		// 초기 메타 정보를 설정
		metaData.TileSize = 100;
		metaData.GridSideTileCount = 100;
		
		// 모드 설정
		editMode = EGridEditMode::MetaDataEditMode;
	}

	// 위젯에 설정
	UpdateGridMetaData(metaData, true);
	ChangeGridEditMode(editMode);

	// 타이머 동작
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this, &UGridEditWidget::OnUpdateTimer, 60.0f / 1000.0f, true);
}


void UGridEditWidget::NativeDestruct()
{
	// 타이머 제거
	if (true == TimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

	// 기존 디버깅 도형 삭제
	UKismetSystemLibrary::FlushPersistentDebugLines(this);


	Super::NativeDestruct();
}

void UGridEditWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	
	
}

void UGridEditWidget::OnClickedCreateMetaData()
{
	// 메타 파일을 저장
	UGridEditSubsystem* subSystem = GEditor->GetEditorSubsystem<UGridEditSubsystem>();
	MGridDataEditManager* editManager = subSystem->GetGridDataEditManager();

	// 설정된 메타 정보를 얻는다
	MGridMetaData metaData;
	UpdateGridMetaData(metaData, false);

	// 메타 정보 재설정
	if (MFALSE == editManager->ResetMetaData(metaData))
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Create Meta Failed")));
		return;
	}
	
	// 그리드 데이터 에디트 모드로 변경
	ChangeGridEditMode(EGridEditMode::GridDataEditMode);
}


void UGridEditWidget::OnClickedLoadGridFromCameraPos()
{
	// 카메라 위치에서 일정 영역을 로드
	UGridEditSubsystem* gridEditSubsystem = GEditor->GetEditorSubsystem<UGridEditSubsystem>();
	if( nullptr == gridEditSubsystem) {
		return;
	}

	const FLevelEditorViewportClient* client = (FLevelEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
	if (nullptr == client) {
		return;
	}

	FVector viewPortPos = client->GetViewLocation();

	// X와 Y만 사용
	gridEditSubsystem->LoadGridData(FVector2D(viewPortPos.X, viewPortPos.Y), 0);

}


void UGridEditWidget::OnClickedUpdateLoadedGridData()
{
	// 카메라 위치에서 일정 영역을 로드
	UGridEditSubsystem* gridEditSubsystem = GEditor->GetEditorSubsystem<UGridEditSubsystem>();
	if (nullptr == gridEditSubsystem) {
		return;
	}

	gridEditSubsystem->UpdateLoadedGridData();
}


void UGridEditWidget::OnUpdateTimer()
{
	if (EGridEditMode::GridDataEditMode == EditMode)
	{
		// 그리드 데이터 에디트 모드인경우 
		// 카멜 위치를 갱신

		if (const auto Client = (FLevelEditorViewportClient*)GEditor->GetActiveViewport()->GetClient())
		{
			FVector viewPos = Client->GetViewLocation();

			TB_ViewCameraPos->SetText(FText::FromString(FString::Printf(TEXT("(X:%.2ff, Y:%.2f)"), viewPos.X, viewPos.Y)));
		}
	}
}





void UGridEditWidget::OnChanged_MapPos(const FText& inText)
{
	
}








void UGridEditWidget::SetIsDirty()
{
	IsDirty = true;
}



void UGridEditWidget::ChangeGridEditMode(EGridEditMode inEditMode)
{
	if (EditMode == inEditMode) {
		return;
	}

	// 메타 파일을 저장
	UGridEditSubsystem* subSystem = GEditor->GetEditorSubsystem<UGridEditSubsystem>();
	MGridDataEditManager* editManager = subSystem->GetGridDataEditManager();

	MGridMetaData* metaData = editManager->GetGridMetaData();

	// 변경
	EditMode = inEditMode;

	// 위젯 초기화
	{
		TArray<UWidget*, TInlineAllocator<20>> widgetList
		{
			ETB_TileSize,
			ETB_GridSideTileCount,
		};

		// 우선 대상 위젯들은 비활성처리
		for (auto widget : widgetList) {
			widget->SetIsEnabled(false);
		}
	}
	

	// 상황에 따라 위젯을 활성화
	int32 activeIndex = 0;
	switch (inEditMode)
	{
	case EGridEditMode::MetaDataEditMode:
	{
		activeIndex = 0;

		// 메타 데이터 에디트 모드

		// 사용 위젯 활성화
		ETB_TileSize->SetIsEnabled(true);
		ETB_GridSideTileCount->SetIsEnabled(true);


	} break;
	case EGridEditMode::GridDataEditMode:
	{
		// 그리드 데이터 에디트 모드
		activeIndex = 1;

	} break;
	}


	WS_EditMode->SetActiveWidgetIndex(activeIndex);
}




void UGridEditWidget::UpdateGridMetaData(MGridMetaData& inMetaData, bool inToWidget)
{
	if (true == inToWidget)
	{
		// 위젯에 설정
		ETB_TileSize->SetText(ConvertToText(inMetaData.TileSize));
		OnChangedTileSize(ETB_TileSize->GetText());

		ETB_GridSideTileCount->SetText(ConvertToText(inMetaData.GridSideTileCount));
		OnChangedGridSideTileCount(ETB_GridSideTileCount->GetText());

	}
	else
	{
		// 데이터에 설정
		inMetaData.TileSize = ConvertToInt(ETB_TileSize->GetText());
		inMetaData.GridSideTileCount = ConvertToInt(ETB_GridSideTileCount->GetText());
	}
}



int32 UGridEditWidget::ConvertToInt(const FText& inText)
{
	return FCString::Atoi(*inText.ToString());
}

FText UGridEditWidget::ConvertToText(int32 inValue)
{
	return FText::FromString(FString::Printf(TEXT("%d"), inValue));
}



void UGridEditWidget::OnChangedTileSize(const FText& inText)
{
	// 숫자인지 체크

	// 타일 사이즈 출력
	TB_TileSize->SetText(FText::Format(FText::FromString(TEXT("({0}x{1})")), { inText , inText }));
}



void UGridEditWidget::OnChangedGridSideTileCount(const FText& inText)
{

	// 그리드 카운트 출력
	TB_GridTileCount->SetText(FText::Format(FText::FromString(TEXT("({0}x{1})")), { inText , inText }));
}