// Copyright Epic Games, Inc. All Rights Reserved.

#include "PathFinderPlugin.h"
#include "PathFinderToolCommand.h"

#include "LevelEditor.h"

#include "EditorUtilityWidgetBlueprint.h"
#include "EditorAssetLibrary.h"
#include "EditorUtilitySubsystem.h"


#define LOCTEXT_NAMESPACE "FPathFinderPluginModule"


void FPathFinderPluginModule::StartupModule()
{
	FPathFinderToolCommand::Register();

	// 커맨드 리스트 생성
	CommandList = MakeShareable(new FUICommandList);

	// 커맨드를 등록
	CommandList->MapAction(
		FPathFinderToolCommand::Get().MenuCommand1,
		FExecuteAction::CreateRaw(this, &FPathFinderPluginModule::OnClickMenuCommand1),
		FCanExecuteAction());

	// FExtender 생성
	TSharedPtr<FExtender> menuExtender = MakeShareable(new FExtender());
	menuExtender->AddMenuBarExtension(
		"Window",
		EExtensionHook::After,
		CommandList,
		FMenuBarExtensionDelegate::CreateRaw(this, &FPathFinderPluginModule::OnMakeMenuBar));


	// 레벨 에디터를 얻어와 extender를 등록
	FLevelEditorModule& levelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	levelEditorModule.GetMenuExtensibilityManager()->AddExtender(menuExtender);
}



void FPathFinderPluginModule::ShutdownModule()
{
	FPathFinderToolCommand::Unregister();
}

void FPathFinderPluginModule::OnClickMenuCommand1()
{
	FString path = TEXT("/PathFinderPlugin/EUW_GridEditWidget.EUW_GridEditWidget");
	UWidgetBlueprint* Blueprint = Cast<UWidgetBlueprint>(UEditorAssetLibrary::LoadAsset(path));

	UEditorUtilityWidgetBlueprint* editorWidget = Cast<UEditorUtilityWidgetBlueprint>(Blueprint);
	if (nullptr != editorWidget)
	{
		UEditorUtilitySubsystem* subsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
		subsystem->SpawnAndRegisterTab(editorWidget);
	}
}

void FPathFinderPluginModule::OnMakeMenuBar(FMenuBarBuilder& menuBarBuilder)
{
	menuBarBuilder.AddPullDownMenu(
		FText::FromString("MyTools"),
		FText::FromString(""),
		FNewMenuDelegate::CreateRaw(this, &FPathFinderPluginModule::OnMakeAddFullDownMenu),
		"MyTools",
		FName(TEXT("MyTools"))
	);
}

void FPathFinderPluginModule::OnMakeAddFullDownMenu(FMenuBuilder& menuBuilder)
{
	menuBuilder.BeginSection("Section", FText::FromString("SectionTool"));
	menuBuilder.AddMenuEntry(FPathFinderToolCommand::Get().MenuCommand1);
	menuBuilder.EndSection();
}



#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPathFinderPluginModule, PathFinderPlugin)