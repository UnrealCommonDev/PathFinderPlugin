// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


//----------------------------------------------
// PathFinder �÷����� ���
//----------------------------------------------
class FPathFinderPluginModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

public:
	//--------------------------------------------------------
	// UI �̺�Ʈ ó��
	//--------------------------------------------------------
	// Ŭ�� ó��
	void OnClickMenuCommand_GridManager();

	// �޴� ���� ó��
	void OnMakeMenuBar(FMenuBarBuilder& menuBarBuilder);
	void OnMakeAddFullDownMenu(FMenuBuilder& menuBuilder);

protected:
	// Ŀ�ǵ� ����Ʈ
	TSharedPtr<class FUICommandList> CommandList;
};
