// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "HTNEditorToolkit.h"

#include "Editor.h"
#include "EditorReimportHandler.h"
#include "EditorStyleSet.h"
#include "HTNEditorLog.h"
#include "HTNEditorWidget.h"
#include "HTNCompositeTaskWidget.h"
#include "HTNDomainWidget.h"
#include "HTNAsset.h"
#include "UObject/NameTypes.h"
#include "Widgets/Docking/SDockTab.h"
#include "SSingleObjectDetailsPanel.h"

#define LOCTEXT_NAMESPACE "FHTNEditorToolkit"

DEFINE_LOG_CATEGORY_STATIC(LogHTNAssetEditorNames, Log, All);


/* Local constants
 *****************************************************************************/

namespace HTNAssetEditorNames
{
	static const FName AppIdentifier("HTNEditorApp");
	static const FName TabId("HTNEditor");
	static const FName DetailsTabId("HTNEditorDetails");
	static const FName DomainTabId("HTNEditorDomain");
}


/* Nested classes
*****************************************************************************/

/////////////////////////////////////////////////////
// SHTNPropertiesTabBody

class SHTNPropertiesTabBody : public SSingleObjectDetailsPanel
{
public:
	SLATE_BEGIN_ARGS(SHTNPropertiesTabBody) {}
	SLATE_END_ARGS()

private:
	// Pointer back to owning sprite editor instance (the keeper of state)
	TWeakPtr<class FHTNEditorToolkit> HTNEditorPtr;
public:
	void Construct(const FArguments& InArgs, TSharedPtr<FHTNEditorToolkit> InHTNEditor, TSharedPtr<ISlateStyle> InStyle)
	{
		HTNEditorPtr = InHTNEditor;
		
		SSingleObjectDetailsPanel::Construct(SSingleObjectDetailsPanel::FArguments().HostCommandList(InHTNEditor->GetToolkitCommands()).HostTabManager(InHTNEditor->GetTabManager()), /*bAutomaticallyObserveViaGetObjectToObserve=*/ true, /*bAllowSearch=*/ true);
	}

	// SSingleObjectDetailsPanel interface
	virtual UObject* GetObjectToObserve() const override
	{
		return HTNEditorPtr.Pin()->GetObjectForDetailsPanel();
	}

	virtual TSharedRef<SWidget> PopulateSlot(TSharedRef<SWidget> PropertyEditorWidget) override
	{
		return SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.FillHeight(1)
			[
				PropertyEditorWidget
			];
	}
	// End of SSingleObjectDetailsPanel interface
};


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
							FTabManager::NewSplitter()
								->SetOrientation(Orient_Horizontal)
								->SetSizeCoefficient(0.9f)
								->Split
								(
									FTabManager::NewStack()
										->AddTab(HTNAssetEditorNames::DomainTabId, ETabState::OpenedTab)
										->SetHideTabWell(true)
										->SetSizeCoefficient(0.9f)
								)
								->Split
								(
									FTabManager::NewStack()
										->AddTab(HTNAssetEditorNames::TabId, ETabState::OpenedTab)
										->SetHideTabWell(true)
										->SetSizeCoefficient(0.9f)
								)
								->Split
								(
									FTabManager::NewStack()
										->AddTab(HTNAssetEditorNames::DetailsTabId, ETabState::OpenedTab)
										->SetHideTabWell(true)
										->SetSizeCoefficient(0.1f)
								)
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

	InTabManager->RegisterTabSpawner(HTNAssetEditorNames::DetailsTabId, FOnSpawnTab::CreateSP(this, &FHTNEditorToolkit::HandleTabManagerSpawnTab, HTNAssetEditorNames::DetailsTabId))
		.SetDisplayName(LOCTEXT("DetailsTabLabel", "Details"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));

	InTabManager->RegisterTabSpawner(HTNAssetEditorNames::DomainTabId, FOnSpawnTab::CreateSP(this, &FHTNEditorToolkit::HandleTabManagerSpawnTab, HTNAssetEditorNames::DomainTabId))
		.SetDisplayName(LOCTEXT("DomainTabLabel", "Domain"))
		.SetGroup(WorkspaceMenuCategoryRef)
		.SetIcon(FSlateIcon(FEditorStyle::GetStyleSetName(), "LevelEditor.Tabs.Details"));
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
		//TabWidget = SNew(SHTNEditor, HTNAsset, Style);
		TabWidget = SNew(SHTNCompositeTask, SharedThis(this), Style);
	}
	else if(TabIdentifier == HTNAssetEditorNames::DomainTabId)
	{
		TabWidget = SNew(SHTNDomain, SharedThis(this), Style);
	}
	else if (TabIdentifier == HTNAssetEditorNames::DetailsTabId)
	{
		TabWidget = SNew(SHTNPropertiesTabBody, SharedThis(this), Style);
	}

	return SNew(SDockTab)
		.TabRole(ETabRole::PanelTab)
		[
			TabWidget.ToSharedRef()
		];
}


/* 
*****************************************************************************/

UObject* FHTNEditorToolkit::GetObjectForDetailsPanel() const
{
	return HTNAsset;
}

FHTNBuilder_CompositeTask* FHTNEditorToolkit::GetSelectedCompositeTask() const
{
	if (HTNAsset && HTNAsset->CompositeTasks.Contains(SelectedTask))
	{
		return &HTNAsset->CompositeTasks[SelectedTask];
	}
	return nullptr;
}

FHTNBuilder_PrimitiveTask* FHTNEditorToolkit::GetSelectedPrimitiveTask() const
{
	if (HTNAsset && HTNAsset->PrimitiveTasks.Contains(SelectedTask))
	{
		return &HTNAsset->PrimitiveTasks[SelectedTask];
	}
	return nullptr;
}

void FHTNEditorToolkit::SelectTask(FName TaskName)
{
	if (HTNAsset)
	{
		if (HTNAsset->PrimitiveTasks.Contains(TaskName) || HTNAsset->CompositeTasks.Contains(TaskName))
		{
			SelectedTask = TaskName;
			// TODO: Update smth
		}
		else
		{
			UE_LOG(LogHTNEditor, Warning, TEXT("Attempted to select %s task which does not exist."), *TaskName.ToString());
		}
	}
}


#undef LOCTEXT_NAMESPACE
