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
	// �׸��� ������ �Ŵ����� ��´� 
	UGridEditSubsystem* subSystem = GEditor->GetEditorSubsystem<UGridEditSubsystem>();
	MGridDataEditManager* editManager = subSystem->GetGridDataEditManager();

	// ��� ����
	TB_GridDataPath->SetText(FText::FromString(editManager->GetGridDataPath().GetStr()));
	TB_GridDataPath->SetIsEnabled(false);
	
	// Ÿ�� ������ ����� ó��
	ETB_TileSize->OnTextChanged.AddDynamic(this, &UGridEditWidget::OnChangedTileSize);

	// �׸��� Ÿ�� ���� ����� ó��
	ETB_GridSideTileCount->OnTextChanged.AddDynamic(this, &UGridEditWidget::OnChangedGridSideTileCount);

	// ��Ÿ ������ ���� ��ư ó��
	BTN_CreateMetaData->OnClicked.AddDynamic(this, &UGridEditWidget::OnClickedCreateMetaData);

	// ī�޶� ��ġ�� �׸��� ������ �ε� 
	BTN_LoadGridFromCameraPos->OnClicked.AddDynamic(this, &UGridEditWidget::OnClickedLoadGridFromCameraPos);
	

	BTN_UpdateLoadedGridData->OnClicked.AddDynamic(this, &UGridEditWidget::OnClickedUpdateLoadedGridData);

	// ������ ��Ÿ ������
	MGridMetaData metaData;
	EGridEditMode editMode;

	// ��Ÿ ������ �ε�
	if (MTRUE == editManager->LoadMetaData())
	{
		// �ε�� ��Ÿ ������ ����
		metaData = *(editManager->GetGridMetaData());

		// ��� ����
		editMode = EGridEditMode::GridDataEditMode;
	}
	else
	{
		// ��Ÿ �����Ͱ� �������� �ʰų� ��ȿ�� ���
		
		// �ʱ� ��Ÿ ������ ����
		metaData.TileSize = 100;
		metaData.GridSideTileCount = 100;
		
		// ��� ����
		editMode = EGridEditMode::MetaDataEditMode;
	}

	// ������ ����
	UpdateGridMetaData(metaData, true);
	ChangeGridEditMode(editMode);

	// Ÿ�̸� ����
	GetWorld()->GetTimerManager().SetTimer(TimerHandle,this, &UGridEditWidget::OnUpdateTimer, 60.0f / 1000.0f, true);
}


void UGridEditWidget::NativeDestruct()
{
	// Ÿ�̸� ����
	if (true == TimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	}

	// ���� ����� ���� ����
	UKismetSystemLibrary::FlushPersistentDebugLines(this);


	Super::NativeDestruct();
}

void UGridEditWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	
	
}

void UGridEditWidget::OnClickedCreateMetaData()
{
	// ��Ÿ ������ ����
	UGridEditSubsystem* subSystem = GEditor->GetEditorSubsystem<UGridEditSubsystem>();
	MGridDataEditManager* editManager = subSystem->GetGridDataEditManager();

	// ������ ��Ÿ ������ ��´�
	MGridMetaData metaData;
	UpdateGridMetaData(metaData, false);

	// ��Ÿ ���� �缳��
	if (MFALSE == editManager->ResetMetaData(metaData))
	{
		FMessageDialog::Open(EAppMsgType::Ok, FText::FromString(TEXT("Create Meta Failed")));
		return;
	}
	
	// �׸��� ������ ����Ʈ ���� ����
	ChangeGridEditMode(EGridEditMode::GridDataEditMode);
}


void UGridEditWidget::OnClickedLoadGridFromCameraPos()
{
	// ī�޶� ��ġ���� ���� ������ �ε�
	UGridEditSubsystem* gridEditSubsystem = GEditor->GetEditorSubsystem<UGridEditSubsystem>();
	if( nullptr == gridEditSubsystem) {
		return;
	}

	const FLevelEditorViewportClient* client = (FLevelEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
	if (nullptr == client) {
		return;
	}

	FVector viewPortPos = client->GetViewLocation();

	// X�� Y�� ���
	gridEditSubsystem->LoadGridData(FVector2D(viewPortPos.X, viewPortPos.Y), 0);

}


void UGridEditWidget::OnClickedUpdateLoadedGridData()
{
	// ī�޶� ��ġ���� ���� ������ �ε�
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
		// �׸��� ������ ����Ʈ ����ΰ�� 
		// ī�� ��ġ�� ����

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

	// ��Ÿ ������ ����
	UGridEditSubsystem* subSystem = GEditor->GetEditorSubsystem<UGridEditSubsystem>();
	MGridDataEditManager* editManager = subSystem->GetGridDataEditManager();

	MGridMetaData* metaData = editManager->GetGridMetaData();

	// ����
	EditMode = inEditMode;

	// ���� �ʱ�ȭ
	{
		TArray<UWidget*, TInlineAllocator<20>> widgetList
		{
			ETB_TileSize,
			ETB_GridSideTileCount,
		};

		// �켱 ��� �������� ��Ȱ��ó��
		for (auto widget : widgetList) {
			widget->SetIsEnabled(false);
		}
	}
	

	// ��Ȳ�� ���� ������ Ȱ��ȭ
	int32 activeIndex = 0;
	switch (inEditMode)
	{
	case EGridEditMode::MetaDataEditMode:
	{
		activeIndex = 0;

		// ��Ÿ ������ ����Ʈ ���

		// ��� ���� Ȱ��ȭ
		ETB_TileSize->SetIsEnabled(true);
		ETB_GridSideTileCount->SetIsEnabled(true);


	} break;
	case EGridEditMode::GridDataEditMode:
	{
		// �׸��� ������ ����Ʈ ���
		activeIndex = 1;

	} break;
	}


	WS_EditMode->SetActiveWidgetIndex(activeIndex);
}




void UGridEditWidget::UpdateGridMetaData(MGridMetaData& inMetaData, bool inToWidget)
{
	if (true == inToWidget)
	{
		// ������ ����
		ETB_TileSize->SetText(ConvertToText(inMetaData.TileSize));
		OnChangedTileSize(ETB_TileSize->GetText());

		ETB_GridSideTileCount->SetText(ConvertToText(inMetaData.GridSideTileCount));
		OnChangedGridSideTileCount(ETB_GridSideTileCount->GetText());

	}
	else
	{
		// �����Ϳ� ����
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
	// �������� üũ

	// Ÿ�� ������ ���
	TB_TileSize->SetText(FText::Format(FText::FromString(TEXT("({0}x{1})")), { inText , inText }));
}



void UGridEditWidget::OnChangedGridSideTileCount(const FText& inText)
{

	// �׸��� ī��Ʈ ���
	TB_GridTileCount->SetText(FText::Format(FText::FromString(TEXT("({0}x{1})")), { inText , inText }));
}