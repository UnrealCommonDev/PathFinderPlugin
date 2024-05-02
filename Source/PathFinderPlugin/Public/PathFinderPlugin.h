// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


//----------------------------------------------
// PathFinder 플러그인 모듈
//----------------------------------------------
class FPathFinderPluginModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

public:
	//--------------------------------------------------------
	// UI 이벤트 처리
	//--------------------------------------------------------
	// 클릭 처리
	void OnClickMenuCommand1();

	// 메뉴 생성 처리
	void OnMakeMenuBar(FMenuBarBuilder& menuBarBuilder);
	void OnMakeAddFullDownMenu(FMenuBuilder& menuBuilder);

protected:
	// 커맨드 리스트
	TSharedPtr<class FUICommandList> CommandList;
};
