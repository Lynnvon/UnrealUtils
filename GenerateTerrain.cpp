// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "GenerateTerrain.h"
#include "GenerateTerrainStyle.h"
#include "GenerateTerrainCommands.h"
#include "Misc/MessageDialog.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Landscape/Classes/Landscape.h"
#include "LevelEditor.h"
#include "FS_Plane.h"
#include "LandscapeProxy.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/Texture2D.h"
#include "LandscapeComponent.h"
#include "Kismet/GameplayStatics.h"

static const FName GenerateTerrainTabName("GenerateTerrain");

#define LOCTEXT_NAMESPACE "FGenerateTerrainModule"

void FGenerateTerrainModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FGenerateTerrainStyle::Initialize();
	FGenerateTerrainStyle::ReloadTextures();

	FGenerateTerrainCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FGenerateTerrainCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FGenerateTerrainModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FGenerateTerrainModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FGenerateTerrainModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
}

void FGenerateTerrainModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FGenerateTerrainStyle::Shutdown();

	FGenerateTerrainCommands::Unregister();
}

void FGenerateTerrainModule::PluginButtonClicked()
{
	// Put your "OnButtonClicked" stuff here
	

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (world)
	{
		TArray<AActor*> _actor;
		UGameplayStatics::GetAllActorsOfClass(world, ALandscapeProxy::StaticClass(), _actor);
		if (_actor.Num() > 0)
		{
			mLandscape = (ALandscapeProxy*)_actor[0];
		}
	}
	if (mLandscape)
	{
		if (!mLandscape->LandscapeMaterial)
		{
			GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,FString("Landscape has not base material"));
			return;
		}
		mLandscape->bUseDynamicMaterialInstance = true;
	/*	mLandscape->SetActorLabel(MakeUniqueObjectName(NULL,ALandscapeProxy::StaticClass()).ToString());

		GEngine->AddOnScreenDebugMessage(-1,5,FColor::Red,FString("mLandscape->SetActorLabel not"));*/
		for (ULandscapeComponent* C : mLandscape->LandscapeComponents)
		{
			LoadTexture(C);
		}


	}	
	

}
FString FGenerateTerrainModule::GetSection(FString sectionName)
{
	FString val;
	if (!GConfig) return FString();
	FString RegistrySection = "Terrain";
	bool isSuccess = GConfig->GetString(
		*RegistrySection,
		(TEXT("%s"), *sectionName),
		val,
		GGameIni
	);
	if (isSuccess)
	{
		return val;
	}
	return FString();
}


void FGenerateTerrainModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FGenerateTerrainCommands::Get().PluginAction);
}

bool FGenerateTerrainModule::LoadTexture(ULandscapeComponent* C)
{
	for (int i = 0; i < C->MaterialInstancesDynamic.Num(); i++)
	{
		UMaterialInstanceDynamic* MID = C->MaterialInstancesDynamic[i];
		int XIndex = GetComponentIndexX(C);
		int YIndex = GetComponentIndexY(C);

		FString _fileName = GetSection(FString("FileName"));
		int rowOffset = FCString::Atoi(*GetSection(FString("rowOffset")));
		int columnOffset = FCString::Atoi(*GetSection(FString("columnOffset")));
		_fileName.Append(FString::FromInt(XIndex + rowOffset));
		_fileName.Append(FString("/"));
		_fileName.Append(FString::FromInt(YIndex + columnOffset));
		_fileName.Append(FString("."));
		_fileName.Append(FString::FromInt(YIndex + columnOffset));
		_fileName.Append(FString("'"));

		UTexture2D* texture = LoadObject<UTexture2D>(NULL, *_fileName);
		if (texture)
		{
			MID->SetTextureParameterValue(FName("Texture"), texture);
		
		}
		else
		{
			return false;
		}
	}
	return true;
}

int FGenerateTerrainModule::GetComponentIndexX(ULandscapeComponent* C)
{
	return  C->GetRelativeTransform().GetLocation().X / mLandscape->ComponentSizeQuads;
}

int FGenerateTerrainModule::GetComponentIndexY(ULandscapeComponent* C)
{
	return C->GetRelativeTransform().GetLocation().Y / mLandscape->ComponentSizeQuads;
}


void FGenerateTerrainModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FGenerateTerrainCommands::Get().PluginAction);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGenerateTerrainModule, GenerateTerrain)