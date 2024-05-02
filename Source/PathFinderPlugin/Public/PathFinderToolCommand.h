#pragma once

#include "CoreMinimal.h"


//-----------------------------------------------------------
// 사용할 커맨드
//-----------------------------------------------------------
class FPathFinderToolCommand : public TCommands<FPathFinderToolCommand>
{
public:
	FPathFinderToolCommand();

public:
	virtual void RegisterCommands() override;

public:
	TSharedPtr<FUICommandInfo> MenuCommand1;

	//test
};

