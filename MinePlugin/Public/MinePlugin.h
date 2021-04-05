// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FMinePluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:

	void OpenMineSweepWindow();
	void AddToolbarButton(FToolBarBuilder& Builder);
	
private:

	TSharedPtr<class FUICommandList> PluginCommands;
};
