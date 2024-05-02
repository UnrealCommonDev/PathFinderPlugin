// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MPathFindData.h"
#include "GridEditWidget.generated.h"


// �� ��ġ ����
enum class EGridEditPosType
{
	Center = 0,
	LeftTop = 1,
};


// �׸��� ����Ʈ ����
enum class EGridEditMode : uint8
{
	None = 0,
	MetaDataEditMode,		// ��Ÿ ������ ����Ʈ ���
	GridDataEditMode,		// �׸��� ������ ����Ʈ ���
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
	// ��ư �ڵ鷯
	//------------------------------------------------------------
	

	UFUNCTION()
	void OnChanged_MapPos(const FText& inText);




	// ��θ� ��´�
	const FString& GetPathFindDataDir() const {
		return PathFindDataDir;
	}


	// ��Ƽ �÷��� ����
	void SetIsDirty();

	// ����Ʈ ��� ����
	void ChangeGridEditMode(EGridEditMode inEditMode);



	//----------------------------------------------------------
	// ��� �Լ�
	//----------------------------------------------------------
	// ���� <-> ����Ÿ
	void UpdateGridMetaData(MGridMetaData& inMetaData, bool inToWidget);


	// �ؽ�Ʈ -> int
	int32 ConvertToInt(const FText& inText);
	FText ConvertToText(int32 inValue);


	//-----------------------------------------------------------
	// �̺�Ʈ ó��
	//-----------------------------------------------------------
	// Ÿ�� ������ ����� ȣ��
	UFUNCTION()
	void OnChangedTileSize(const FText& inText);

	// Ÿ�� ī��Ʈ ����� ȣ��
	UFUNCTION()
	void OnChangedGridSideTileCount(const FText& inText);

	// ��Ÿ ������ ���� ��ư Ŭ��
	UFUNCTION()
	void OnClickedCreateMetaData();

	// �׸��� �ε� ��ư Ŭ��
	UFUNCTION()
	void OnClickedLoadGridFromCameraPos();

	UFUNCTION()
	void OnClickedUpdateLoadedGridData();


	void OnUpdateTimer();


protected:

	//------------------------------------------------------------
	// ������ ���丮
	//------------------------------------------------------------ 
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_GridDataPath = nullptr;


	//------------------------------------------------------------
	// Ÿ�� ������
	//------------------------------------------------------------
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ETB_TileSize = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_TileSize = nullptr;

	//------------------------------------------------------------
	// �׸���
	//------------------------------------------------------------
	// �׸��� Ÿ�� ����
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ETB_GridSideTileCount = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_GridTileCount = nullptr;


	// ���� ������
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WS_EditMode = nullptr;


	//------------------------------------------------------------
	// �޴� ������ ����Ʈ ���
	//------------------------------------------------------------
	// ��Ÿ ������ ����
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_CreateMetaData = nullptr;


	//------------------------------------------------------------
	// �׸��� ������ ����Ʈ ���
	//------------------------------------------------------------
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TB_ViewCameraPos = nullptr;

	// ��Ÿ ������ ����
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_LoadGridFromCameraPos = nullptr;

	// ��Ÿ ������ ����
	UPROPERTY(meta = (BindWidget))
	class UButton* BTN_UpdateLoadedGridData = nullptr;


	//------------------------------------------------------------
	// �н� ���ε� ������
	//------------------------------------------------------------
	// ��� ���丮
	FString PathFindDataDir;

	// ������
	MPathFindData PathFindData;

	// Ÿ�̸� �ڵ�
	FTimerHandle TimerHandle;

	// �н� ���ε������� �ٽ� �׸�������
	bool IsDirty = false;





	//------------------------------------------------------------
	//  
	//------------------------------------------------------------
	// ����Ʈ ���
	EGridEditMode EditMode = EGridEditMode::None;
};
