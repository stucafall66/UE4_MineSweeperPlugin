#include "MinePluginCommands.h"
#include "MinePlugin.h"

#define LOCTEXT_NAMESPACE "FMinePluginModule"

void FMinePluginCommands::RegisterCommands()
{
	UI_COMMAND(OpenWidgetCommand, "OpenWIdgetCommand", "The command to open mine widget",
		EUserInterfaceActionType::Button, FInputGesture());
}
#undef LOCTEXT_NAMESPACE