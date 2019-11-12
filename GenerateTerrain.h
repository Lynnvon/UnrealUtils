// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "LandscapeProxy.h"

class FToolBarBuilder;
class FMenuBuilder;
class ULandscapeComponent;

class FGenerateTerrainModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	FString GetSection(FString sectionName);
	
private:

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

	bool LoadTexture(ULandscapeComponent* C);
	int GetComponentIndexX(ULandscapeComponent* C);
	int GetComponentIndexY(ULandscapeComponent* C);
private:
	TSharedPtr<class FUICommandList> PluginCommands;
	ALandscapeProxy* mLandscape = nullptr;
};
