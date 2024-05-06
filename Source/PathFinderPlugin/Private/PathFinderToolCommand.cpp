#include "PathFinderToolCommand.h"



#define LOCTEXT_NAMESPACE "FPathFinderToolCommand"

FPathFinderToolCommand::FPathFinderToolCommand()
	: TCommands<FPathFinderToolCommand>(TEXT("MenuTool"), FText::FromString("EditorTools"), NAME_None, FAppStyle::GetAppStyleSetName())
{
}

void FPathFinderToolCommand::RegisterCommands()
{
	UI_COMMAND(MenuCommand_GridManager, "그리드 관리", "", EUserInterfaceActionType::Button, FInputChord());
}



#undef LOCTEXT_NAMESPACE