// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "HTNDomainWidget.h"

#include "Fonts/SlateFontInfo.h"
#include "HTNEditorLog.h"
#include "Toolkits/HTNEditorToolkit.h"
#include "HTNBuilder.h"
#include "HTNAsset.h"
#include "UObject/Class.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"

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

	ListPrimitiveTasks = MakeListWidget(&CachedPrimitiveTasks, LOCTEXT("PrimTaskListCaption", "Primitive Tasks"));
	ListCompositeTasks = MakeListWidget(&CachedCompositeTasks, LOCTEXT("CompTaskListCaption", "Composite Tasks"));

	ChildSlot
	[
		SNew(SBorder)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
				.FillHeight(1.0f)
					[
						ListPrimitiveTasks.ToSharedRef()
					]
			+ SVerticalBox::Slot()
				.FillHeight(1.0f)
					[
						ListCompositeTasks.ToSharedRef()
					]
		]
	];

	FCoreUObjectDelegates::OnObjectPropertyChanged.AddSP(this, &SHTNDomain::HandleAssetPropertyChanged);
	Update();
}

TSharedRef<SHTNDomain::SHTNTaskListView> SHTNDomain::MakeListWidget(const TArray<TSharedPtr<FHTNTaskViewModel>>* ListItemsSource, const FText& Caption)
{
	return SNew(SHTNTaskListView)
		.ItemHeight(24)
		.ListItemsSource(ListItemsSource)
		.OnGenerateRow(this, &SHTNDomain::CreateTaskWidget)
		.SelectionMode(ESelectionMode::Single)
		.OnSelectionChanged(this, &SHTNDomain::HandleTaskSelectionChanged)
		.HeaderRow(
			SNew(SHeaderRow)
			+ SHeaderRow::Column("TaskList")
			[
				SNew(STextBlock)
				.Text(Caption)
			]
		);
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

	for (const auto& Task : HTNAsset->PrimitiveTasks)
	{
		CachedPrimitiveTasks.Add(MakeShareable(new FHTNTaskViewModel(Task.Key, Task.Value)));
	}

	for (const auto& Task : HTNAsset->CompositeTasks)
	{
		CachedCompositeTasks.Add(MakeShareable(new FHTNTaskViewModel(Task.Key, Task.Value)));
	}

	ListPrimitiveTasks->RequestListRefresh();
	ListCompositeTasks->RequestListRefresh();
}

/*
TSharedRef<SWidget> SHTNDomain::CreateTaskWidget(const FHTNBuilder_PrimitiveTask& Task)
{
	return CreateTaskWidget(Task.Name, FText::FromName(Task.Name), false);
}

TSharedRef<SWidget> SHTNDomain::CreateTaskWidget(const FHTNBuilder_CompositeTask& Task)
{
	return CreateTaskWidget(Task.Name, FText::FromName(Task.Name), false);
}

TSharedRef<SWidget> SHTNDomain::CreateTaskWidget(const FName& Name, const FText& DisplayName, bool bRoot)
{
	return
		SNew(SButton)
//		.OnClicked(&SHTNDomain::HandleSelectTask, Name)
		[
			SNew(STextBlock)
			.Text(DisplayName)
		];
}
*/

TSharedRef<ITableRow> SHTNDomain::CreateTaskWidget(TSharedPtr<FHTNTaskViewModel> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	return
		SNew(STableRow<TSharedPtr<FHTNTaskViewModel>>, OwnerTable)
		[
			SNew(STextBlock)
			.Text(InItem->DisplayName)
		];
}

void SHTNDomain::HandleTaskSelectionChanged(TSharedPtr<FHTNTaskViewModel> SelectedItem, ESelectInfo::Type SelectInfo)
{
	if (SelectedItem.IsValid())
	{
		// Clear selection in the other list
		if (SelectedItem->bIsComposite)
		{
			ListPrimitiveTasks->ClearSelection();
		}
		else
		{
			ListCompositeTasks->ClearSelection();
		}

		if (HTNEditor.IsValid())
		{
			HTNEditor.Pin()->SelectTask(SelectedItem->Name);
		}
	}
}

#undef LOCTEXT_NAMESPACE

