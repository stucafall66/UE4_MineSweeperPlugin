#pragma once

#include "SlateBasics.h"
#include "EditorStyleSet.h"

class FMinePluginCommands : public TCommands<FMinePluginCommands>
{
public:

	FMinePluginCommands() : TCommands<FMinePluginCommands>
		(
			TEXT("MinePlugin"),
			NSLOCTEXT("Contexts", "MinePlugin", "MinePlugin"),
			NAME_None,
			FEditorStyle::GetStyleSetName()
		){}

	virtual void RegisterCommands() override;

public:

	TSharedPtr<FUICommandInfo> OpenWidgetCommand;
};
