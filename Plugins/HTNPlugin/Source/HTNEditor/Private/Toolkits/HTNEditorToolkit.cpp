// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "HTNEditorToolkit.h"

#include "Editor.h"
#include "EditorReimportHandler.h"
#include "EditorStyleSet.h"
#include "ScopedTransaction.h"
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

	SelectedTaskId = INDEX_NONE;

	{
		TArray<int32> Keys;
		int32 max1, max2;
		HTNAsset->GetPrimitiveTasks().GetKeys(Keys);
		max1 = FMath::Max<int32>(Keys);
		HTNAsset->GetCompositeTasks().GetKeys(Keys);
		max2 = FMath::Max<int32>(Keys);
		MaxTaskId = max1 > max2 ? max1 : max2;
	}

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

const FHTNBuilder_CompositeTask* FHTNEditorToolkit::GetSelectedCompositeTask() const
{
	if (HTNAsset->GetCompositeTasks().Contains(SelectedTaskId))
	{
		return &HTNAsset->GetCompositeTasks()[SelectedTaskId];
	}
	return nullptr;
}

const FHTNBuilder_PrimitiveTask* FHTNEditorToolkit::GetSelectedPrimitiveTask() const
{
	if (HTNAsset->GetPrimitiveTasks().Contains(SelectedTaskId))
	{
		return &HTNAsset->GetPrimitiveTasks()[SelectedTaskId];
	}
	return nullptr;
}

void FHTNEditorToolkit::SelectTask(int32 TaskId)
{
	if (HTNAsset->GetPrimitiveTasks().Contains(TaskId) || HTNAsset->GetCompositeTasks().Contains(TaskId))
	{
		SelectedTaskId = TaskId;
		// TODO: Update smth
	}
	else
	{
		UE_LOG(LogHTNEditor, Warning, TEXT("Attempted to select %d task which does not exist."), TaskId);
	}
}

void FHTNEditorToolkit::AddNewMethodToSelectedCompositeTask()
{
	if (GetSelectedCompositeTask())
	{
		HTNAsset->CompositeTasks[SelectedTaskId].Methods.AddDefaulted();
	}
	else
	{
		UE_LOG(LogHTNEditor, Warning, TEXT("FHTNEditorToolkit::AddNewMethodToSelectedCompositeTask: No Composite task selected!"));
	}
}

void FHTNEditorToolkit::RenameTask(int TaskId, const FText& NewName)
{
	if (HTNAsset->GetPrimitiveTasks().Contains(TaskId) || HTNAsset->GetCompositeTasks().Contains(TaskId))
	{
		const FScopedTransaction Transaction(LOCTEXT("RenameTaskAction", "Rename task"));

		if (HTNAsset->Modify())
		{
			if (HTNAsset->GetPrimitiveTasks().Contains(TaskId))
			{
				HTNAsset->PrimitiveTasks[TaskId].DisplayName = NewName;

			}
			else
			{
				HTNAsset->CompositeTasks[TaskId].DisplayName = NewName;
			}
			HTNAsset->PostEditChange();
		}
		else
		{
			UE_LOG(LogHTNEditor, Warning, TEXT("FHTNEditorToolkit::RenameTask: Cannot modify the asset!"));
		}
	}
	else
	{
		UE_LOG(LogHTNEditor, Warning, TEXT("FHTNEditorToolkit::RenameTask (%s): Task %d does not exist!"), *NewName.ToString(), TaskId);
	}
}

int32 FHTNEditorToolkit::NewPrimitiveTask()
{
	const FScopedTransaction Transaction(LOCTEXT("NewPrimitiveTaskAction", "New primitive task"));
	return NewTask_internal(HTNAsset->PrimitiveTasks, LOCTEXT("NewPrimitiveTaskNameFormat", "PrimitiveTask{0}"));
}

int32 FHTNEditorToolkit::NewCompositeTask()
{
	const FScopedTransaction Transaction(LOCTEXT("NewCompositeTaskAction", "New composite task"));
	return NewTask_internal(HTNAsset->CompositeTasks, LOCTEXT("NewCompositeTaskNameFromat", "CompositeTask{0}"));
}

template<typename Task>
int32 FHTNEditorToolkit::NewTask_internal(TMap<int32, Task>& TaskMap, const FText& TaskNameFormat)
{
	check(!HTNAsset->GetPrimitiveTasks().Contains(MaxTaskId + 1) && !HTNAsset->GetCompositeTasks().Contains(MaxTaskId + 1));

	if (HTNAsset->Modify())
	{
		auto& NewTask = TaskMap.Add(++MaxTaskId);
		NewTask.DisplayName = FText::Format(TaskNameFormat, MaxTaskId);

		HTNAsset->PostEditChange();
		return MaxTaskId;
	}
	else
	{
		UE_LOG(LogHTNEditor, Warning, TEXT("FHTNEditorToolkit::NewTask_internal: Cannot modify the asset!"));
	}
	return INDEX_NONE;
}

#undef LOCTEXT_NAMESPACE
