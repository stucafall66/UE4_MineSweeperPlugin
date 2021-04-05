#include "MinePlugin.h"
#include "MinePluginCommands.h"
#include "LevelEditor.h"
#include "SMineSweepWidget.h"

#define LOCTEXT_NAMESPACE "FMinePluginModule"

void FMinePluginModule::StartupModule()
{
	FMinePluginCommands::Register();
	PluginCommands = MakeShareable(new FUICommandList);
	PluginCommands->MapAction(
		FMinePluginCommands::Get().OpenWidgetCommand,
		FExecuteAction::CreateRaw(this, &FMinePluginModule::OpenMineSweepWindow)
	);

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr<FExtender> NewToolbarExtender = MakeShareable(new FExtender);
	NewToolbarExtender->AddToolBarExtension("Content",
		EExtensionHook::Before,
		PluginCommands,
		FToolBarExtensionDelegate::CreateRaw(this, &FMinePluginModule::AddToolbarButton));

	LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(NewToolbarExtender);
}

void FMinePluginModule::ShutdownModule()
{
	FMinePluginCommands::Unregister();
}

void FMinePluginModule::AddToolbarButton(FToolBarBuilder& Builder)
{
	//Set button icon
	const FText ButtonLabel = LOCTEXT("Button_Lable", "MineSweepBtn");
	const FText ButtonTooltip = LOCTEXT("Button_Tooltip", "Start Mine Sweeping");
	const FSlateIcon ButtonIcon = FSlateIcon(TEXT("EditorStyle"), "SessionConsole.Clear");

	Builder.AddToolBarButton(
		FMinePluginCommands::Get().OpenWidgetCommand,
		NAME_None,
		ButtonLabel,
		ButtonTooltip,
		ButtonIcon
		);
}

void FMinePluginModule::OpenMineSweepWindow()
{
	TSharedRef<SWindow> MineSweepingWindow = SNew(SWindow)
		.Title(FText::FromString(TEXT("Mine Sweeping Window")))
		.ClientSize(FVector2D(800, 600))
		[
			SNew(SMineSweepWidget)
		];
	FSlateApplication::Get().AddWindow(MineSweepingWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FMinePluginModule, MinePlugin)