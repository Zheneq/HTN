// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "HTNEditorToolkit.h"

#include "Editor.h"
#include "EditorReimportHandler.h"
#include "EditorStyleSet.h"
#include "SHTNEditorWidget.h"
#include "HTNAsset.h"
#include "UObject/NameTypes.h"
#include "Widgets/Docking/SDockTab.h"

#define LOCTEXT_NAMESPACE "FHTNEditorToolkit"

DEFINE_LOG_CATEGORY_STATIC(LogHTNAssetEditorNames, Log, All);


/* Local constants
 *****************************************************************************/

namespace HTNAssetEditorNames
{
	static const FName AppIdentifier("HTNEditorApp");
	static const FName TabId("HTNEditor");
}


/* FHTNEditorToolkit structors
 *****************************************************************************/

FHTNEditorToolkit::FHTNEditorToolkit(const TSharedRef<ISlateStyle>& InStyle)
	: HTNAsset(nullptr)
	, Style(InStyle)
{ }


FHTNEditorToolkit::~FHTNEditorToolkit()
{
	FReimportManager::Instance()->OnPreReimport().RemoveAll(this);
	FReimportManager::Instance()->OnPostReimport().RemoveAll(this);

	GEditor->UnregisterForUndo(this);
}


/* FHTNEditorToolkit interface
 *****************************************************************************/

void FHTNEditorToolkit::Initialize(UHTNAsset* InHTNAsset, const EToolkitMode::Type InMode, const TSharedPtr<class IToolkitHost>& InToolkitHost)
{
	HTNAsset = InHTNAsset;

	// Support undo/redo
	HTNAsset->SetFlags(RF_Transactional);
	GEditor->RegisterForUndo(this);

	// create tab layout
	const TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout("Standalone_HTNAssetEditor")
		->AddArea
		(
			FTabManager::NewPrimaryArea()
				->SetOrientation(Orient_Horizontal)
				->Split
				(
					FTabManager::NewSplitter()
						->SetOrientation(Orient_Vertical)
						->SetSizeCoefficient(0.66f)
						->Split
						(
							FTabManager::NewStack()
								->AddTab(GetToolbarTabId(), ETabState::OpenedTab)
								->SetHideTabWell(true)
								->SetSizeCoefficient(0.1f)
								
						)
						->Split
						(
							FTabManager::NewStack()
								->AddTab(HTNAssetEditorNames::TabId, ETabState::OpenedTab)
								->SetHideTabWell(true)
								->SetSizeCoefficient(0.9f)
						)
				)
		);

	FAssetEditorToolkit::InitAssetEditor(
		InMode,
		InToolkitHost,
		HTNAssetEditorNames::AppIdentifier,
		Layout,
		true /*bCreateDefaultStandaloneMenu*/,
		true /*bCreateDefaultToolbar*/,
		InHTNAsset
	);

	RegenerateMenusAndToolbars();
}


/* FAssetEditorToolkit interface
 *****************************************************************************/

/*
FString FHTNEditorToolkit::GetDocumentationLink() const
{
	return FString(TEXT("https://"));
}
*/

void FHTNEditorToolkit::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_HTNEditor", "Hierarchial Task Network Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(HTNAssetEditorNames::TabId, FOnSpawnTab::CreateSP(this, &FHTNEditorToolkit::HandleTabManagerSpawnTab, HTNAssetEditorNames::TabId))
		.SetDisplayName(LOCTEXT("HTNEditorTabName", "HTN Editor"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Viewports"));
}


void FHTNEditorToolkit::UnregisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	FAssetEditorToolkit::UnregisterTabSpawners(InTabManager);

	InTabManager->UnregisterTabSpawner(HTNAssetEditorNames::TabId);
}


/* IToolkit interface
 *****************************************************************************/

FText FHTNEditorToolkit::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "Hierarchial Task Network Editor");
}


FName FHTNEditorToolkit::GetToolkitFName() const
{
	return FName("HTNEditor");
}


FLinearColor FHTNEditorToolkit::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f);
}


FString FHTNEditorToolkit::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "HTN ").ToString();
}


/* FGCObject interface
 *****************************************************************************/

void FHTNEditorToolkit::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(HTNAsset);
}


/* FEditorUndoClient interface
*****************************************************************************/

void FHTNEditorToolkit::PostUndo(bool bSuccess)
{ }


void FHTNEditorToolkit::PostRedo(bool bSuccess)
{
	PostUndo(bSuccess);
}


/* FHTNAssetEditorNamesToolkit callbacks
 *****************************************************************************/

TSharedRef<SDockTab> FHTNEditorToolkit::HandleTabManagerSpawnTab(const FSpawnTabArgs& Args, FName TabIdentifier)
{
	TSharedPtr<SWidget> TabWidget = SNullWidget::NullWidget;

	if (TabIdentifier == HTNAssetEditorNames::TabId)
	{
		TabWidget = SNew(SHTNEditor, HTNAsset, Style);
	}

	return SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			TabWidget.ToSharedRef()
		];
}


#undef LOCTEXT_NAMESPACE
