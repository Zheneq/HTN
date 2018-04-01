// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "HTNEditorModule.h"

#include "HTNEditorLog.h"
#include "Engine/Texture2D.h"
#include "Editor.h"
#include "EditorModeRegistry.h"
#include "Modules/ModuleManager.h"
//#include "UObject/UObjectHash.h"
//#include "UObject/UObjectIterator.h"
//#include "ThumbnailRendering/ThumbnailManager.h"

//#include "AssetToolsModule.h"
//#include "PropertyEditorModule.h"
//#include "ContentBrowserExtensions/ContentBrowserExtensions.h"
//#include "LevelEditorMenuExtensions/Paper2DLevelEditorExtensions.h"
//#include "PaperTileMap.h"
//#include "PaperImporterSettings.h"


// Settings
//#include "PaperRuntimeSettings.h"
#include "ISettingsModule.h"


DEFINE_LOG_CATEGORY(LogHTNEditor);

#define LOCTEXT_NAMESPACE "HTNEditor"

//////////////////////////////////////////////////////////////////////////
// HTNEditor

class FHTNEditor : public IHTNEditorModule
{
public:
/*
	// IPaper2DEditorModule interface
	virtual TSharedPtr<FExtensibilityManager> GetSpriteEditorMenuExtensibilityManager() override { return SpriteEditor_MenuExtensibilityManager; }
	virtual TSharedPtr<FExtensibilityManager> GetSpriteEditorToolBarExtensibilityManager() override { return SpriteEditor_ToolBarExtensibilityManager; }

	virtual TSharedPtr<FExtensibilityManager> GetFlipbookEditorMenuExtensibilityManager() override { return FlipbookEditor_MenuExtensibilityManager; }
	virtual TSharedPtr<FExtensibilityManager> GetFlipbookEditorToolBarExtensibilityManager() override { return FlipbookEditor_ToolBarExtensibilityManager; }
	virtual uint32 GetPaper2DAssetCategory() const override { return Paper2DAssetCategoryBit; }
	// End of IPaper2DEditorModule
*/
private:

public:
	virtual void StartupModule() override
	{
		UE_LOG(LogHTNEditor, Log, TEXT("HTN editor plugin is up and running!"));
	}

	virtual void ShutdownModule() override
	{

	}

};

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_MODULE(FHTNEditor, HTNEditor);

//////////////////////////////////////////////////////////////////////////

#undef LOCTEXT_NAMESPACE
