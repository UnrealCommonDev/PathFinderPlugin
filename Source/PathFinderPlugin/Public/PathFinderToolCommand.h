#pragma once

#include "CoreMinimal.h"


//-----------------------------------------------------------
// ����� Ŀ�ǵ�
//-----------------------------------------------------------
class FPathFinderToolCommand : public TCommands<FPathFinderToolCommand>
{
public:
	FPathFinderToolCommand();

public:
	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> MenuCommand_GridManager;

	//test
};

