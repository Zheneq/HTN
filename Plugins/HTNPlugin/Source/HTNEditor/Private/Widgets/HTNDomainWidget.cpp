// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "HTNDomainWidget.h"

#include "Fonts/SlateFontInfo.h"
#include "HTNEditorLog.h"
#include "Toolkits/HTNEditorToolkit.h"
#include "HTNBuilder.h"
#include "HTNAsset.h"
#include "UObject/Class.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Images/SImage.h"

#include "EditorStyleSet.h"
#include "DetailLayoutBuilder.h"

#include "HTNEditorSettings.h"


#define LOCTEXT_NAMESPACE "SHTNEditor"


/* HTN Domain Widget
************************************************************************************/

SHTNDomain::~SHTNDomain()
{
	FCoreUObjectDelegates::OnObjectPropertyChanged.RemoveAll(this);
}

void SHTNDomain::Construct(const FArguments& InArgs, TWeakPtr<FHTNEditorToolkit> InHTNEditor, const TSharedRef<ISlateStyle>& InStyle)
{
	check(InHTNEditor.IsValid());

	HTNEditor = InHTNEditor;
	HTNAsset = InHTNEditor.Pin()->GetAsset();
	auto Settings = GetDefault<UHTNEditorSettings>();

	ChildSlot
	[
		SNew(SBorder)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
				.FillHeight(1.0f)
					[
						SAssignNew(ListPrimitiveTasks, SHTNTaskListView)
							.OnGenerateRow(this, &SHTNDomain::CreateTaskWidget)
							.OnSelectionChanged(this, &SHTNDomain::HandleTaskSelectionChanged)
							.OnAddNewClicked(this, &SHTNDomain::HandleNewPrimitiveTask)
							.ListItemsSource(&CachedPrimitiveTasks)
							.ListCaption(LOCTEXT("PrimTaskListCaption", "Primitive Tasks"))
							.AddNewCaption(LOCTEXT("NewPrimTaskBtnCaption", "New Primitive Task"))
					]
			+ SVerticalBox::Slot()
				.FillHeight(1.0f)
					[
						SAssignNew(ListCompositeTasks, SHTNTaskListView)
							.OnGenerateRow(this, &SHTNDomain::CreateTaskWidget)
							.OnSelectionChanged(this, &SHTNDomain::HandleTaskSelectionChanged)
							.OnAddNewClicked(this, &SHTNDomain::HandleNewCompositeTask)
							.ListItemsSource(&CachedCompositeTasks)
							.ListCaption(LOCTEXT("CompTaskListCaption", "Composite Tasks"))
							.AddNewCaption(LOCTEXT("NewCompTaskBtnCaption", "New Composite Task"))
					]
		]
	];

	FCoreUObjectDelegates::OnObjectPropertyChanged.AddSP(this, &SHTNDomain::HandleAssetPropertyChanged);
	Update();
}

/* callbacks
 *****************************************************************************/

void SHTNDomain::HandleAssetPropertyChanged(UObject* Object, FPropertyChangedEvent& PropertyChangedEvent)
{
	if (Object == HTNAsset)
	{
		Update();
	}
}

void SHTNDomain::Update()
{
	CachedPrimitiveTasks.Empty();
	CachedCompositeTasks.Empty();

	for (const auto& Task : HTNAsset->GetPrimitiveTasks())
	{
		CachedPrimitiveTasks.Add(MakeShareable(new FHTNTaskViewModel(Task.Key, Task.Value)));
	}

	for (const auto& Task : HTNAsset->GetCompositeTasks())
	{
		CachedCompositeTasks.Add(MakeShareable(new FHTNTaskViewModel(Task.Key, Task.Value)));
	}

	ListPrimitiveTasks->Update();
	ListCompositeTasks->Update();

	if (PendingRenameTaskId > 0)
	{
		ListPrimitiveTasks->RequestRename(PendingRenameTaskId);
		ListPrimitiveTasks->RequestRename(PendingRenameTaskId);
		PendingRenameTaskId = INDEX_NONE;
	}
}

TSharedRef<ITableRow> SHTNDomain::CreateTaskWidget(TSharedPtr<FHTNTaskViewModel> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	return
		SNew(STableRow<TSharedPtr<FHTNTaskViewModel>>, OwnerTable)
		[
			SNew(SHTNTaskListItem)
			.TaskName(InItem->DisplayName)
			.TaskId(InItem->Id)
			.OnRenamed(this, &SHTNDomain::RenameTask)
		];
}

void SHTNDomain::RenameTask(int32 TaskId, const FText& NewName)
{
	if (HTNEditor.IsValid())
	{
		HTNEditor.Pin()->RenameTask(TaskId, NewName);
	}
}

FReply SHTNDomain::HandleNewPrimitiveTask()
{
	if (HTNEditor.IsValid())
	{
		PendingRenameTaskId = HTNEditor.Pin()->NewPrimitiveTask();
	}
	return FReply::Handled();
}

FReply SHTNDomain::HandleNewCompositeTask()
{
	if (HTNEditor.IsValid())
	{
		PendingRenameTaskId = HTNEditor.Pin()->NewCompositeTask();
	}
	return FReply::Handled();
}

void SHTNDomain::HandleTaskSelectionChanged(TSharedPtr<FHTNTaskViewModel> SelectedItem, ESelectInfo::Type SelectInfo)
{
	if (SelectedItem.IsValid())
	{
		// Clear selection in the other list
		if (SelectedItem->bIsComposite)
		{
			ListPrimitiveTasks->AsListView()->ClearSelection();
		}
		else
		{
			ListCompositeTasks->AsListView()->ClearSelection();
		}

		if (HTNEditor.IsValid())
		{
			HTNEditor.Pin()->SelectTask(SelectedItem->Id);
		}
	}
}

#undef LOCTEXT_NAMESPACE

