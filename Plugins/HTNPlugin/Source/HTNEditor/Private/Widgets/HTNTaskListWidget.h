// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Templates/SharedPointer.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Styling/SlateTypes.h"
#include "Framework/SlateDelegates.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"


template <typename ItemType>
class SHTNTaskList
	: public SCompoundWidget
{
public:
	typedef typename TListTypeTraits< ItemType >::NullableType NullableItemType;

	typedef typename TSlateDelegates< ItemType >::FOnGenerateRow FOnGenerateRow;
	typedef typename TSlateDelegates< ItemType >::FOnItemScrolledIntoView FOnItemScrolledIntoView;
	typedef typename TSlateDelegates< NullableItemType >::FOnSelectionChanged FOnSelectionChanged;
	typedef typename TSlateDelegates< ItemType >::FOnMouseButtonClick FOnMouseButtonClick;
	typedef typename TSlateDelegates< ItemType >::FOnMouseButtonDoubleClick FOnMouseButtonDoubleClick;
	typedef typename SListView< ItemType >::FOnWidgetToBeRemoved FOnWidgetToBeRemoved;

public:

	SLATE_BEGIN_ARGS(SHTNTaskList)
		: _ListItemsSource()
		{}

		SLATE_EVENT(FOnGenerateRow, OnGenerateRow);

		SLATE_EVENT(FOnWidgetToBeRemoved, OnRowReleased);

		SLATE_EVENT(FOnTableViewScrolled, OnListViewScrolled);

		SLATE_EVENT(FOnItemScrolledIntoView, OnItemScrolledIntoView);

		SLATE_EVENT(FOnContextMenuOpening, OnContextMenuOpening);

		SLATE_EVENT(FOnMouseButtonClick, OnMouseButtonClick);

		SLATE_EVENT(FOnMouseButtonDoubleClick, OnMouseButtonDoubleClick);

		SLATE_EVENT(FOnSelectionChanged, OnSelectionChanged);

		SLATE_EVENT(FOnClicked, OnAddNewClicked);

		SLATE_ARGUMENT(const TArray<ItemType>*, ListItemsSource);

		SLATE_ARGUMENT(FText, ListCaption);

		SLATE_ARGUMENT(FText, AddNewCaption);

	SLATE_END_ARGS()

public:

	/**
	* Construct this widget
	*
	* @param InArgs The declaration data for this widget.
	*/
	void Construct(const typename SHTNTaskList<ItemType>::FArguments& InArgs)
	{
		TSharedPtr<SHorizontalBox> HB;

		TSharedPtr<SHeaderRow> Header =
			SNew(SHeaderRow)
			+ SHeaderRow::Column("TaskList")
			[
				SAssignNew(HB, SHorizontalBox)
				+ SHorizontalBox::Slot()
				.FillWidth(1.0f)
					[
						SNew(STextBlock)
						.Text(InArgs._ListCaption)
					]
			];

		HB->AddSlot()
		.AutoWidth()
			[
				CreateAddButton(Header, InArgs._AddNewCaption)
			];

		ChildSlot
			[
				SAssignNew(ListWidget, SListView<ItemType>)
				.ItemHeight(24)
				.ListItemsSource(InArgs._ListItemsSource)
				.OnGenerateRow(InArgs._OnGenerateRow)
				.SelectionMode(ESelectionMode::Single)
				.OnSelectionChanged(InArgs._OnSelectionChanged)
				.OnRowReleased(InArgs._OnRowReleased)
				.OnItemScrolledIntoView(InArgs._OnItemScrolledIntoView)
				.OnContextMenuOpening(InArgs._OnContextMenuOpening)
				.OnMouseButtonClick(InArgs._OnMouseButtonClick)
				.OnMouseButtonDoubleClick(InArgs._OnMouseButtonDoubleClick)
				.HeaderRow(
					Header.ToSharedRef()
				)
			];

		OnAddNewClicked = InArgs._OnAddNewClicked;
	}

	void Update()
	{
		ListWidget->RequestListRefresh();
	}

	TSharedPtr<SListView<ItemType>> AsListView()
	{
		return ListWidget;
	}

private:
	TSharedPtr<SListView<ItemType>> ListWidget;

	FOnClicked OnAddNewClicked;

	TSharedRef<SWidget> CreateAddButton(TWeakPtr<SWidget> RowWidget, FText AddNewText)
	{
		return SNew(SButton)
			.ButtonStyle(FEditorStyle::Get(), "RoundButton")
			.ForegroundColor(FEditorStyle::GetSlateColor("DefaultForeground"))
			.ContentPadding(FMargin(2, 0))
			.OnClicked(this, &SHTNTaskList::OnAddButtonClicked)
	//		.IsEnabled(this, &SMyBlueprint::IsEditingMode)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
	//		.AddMetaData<FTagMetaData>(FTagMetaData(MetaDataTag))
			[
				SNew(SHorizontalBox)

				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(FMargin(0, 1))
				[
					SNew(SImage)
					.Image(FEditorStyle::GetBrush("Plus"))
				]

				+ SHorizontalBox::Slot()
				.VAlign(VAlign_Center)
				.AutoWidth()
				.Padding(FMargin(2,0,0,0))
				[
					SNew(STextBlock)
					.Font(IDetailLayoutBuilder::GetDetailFontBold())
					.Text(AddNewText)
					.Visibility(this, &SHTNTaskList::OnGetSectionTextVisibility, RowWidget)
					.ShadowOffset(FVector2D(1,1))
				]
			];
	}

	FReply OnAddButtonClicked()
	{
		return OnAddNewClicked.IsBound() ? OnAddNewClicked.Execute() : FReply::Handled();
	}

	EVisibility OnGetSectionTextVisibility(TWeakPtr<SWidget> RowWidget) const
	{
		bool ShowText = RowWidget.Pin()->IsHovered();
	//	if (InSectionID == NodeSectionID::FUNCTION && FunctionSectionButton.IsValid() && FunctionSectionButton->IsOpen())
	//	{
	//		ShowText = true;
	//	}

		// If the row is currently hovered, or a menu is being displayed for a button, keep the button expanded.
		if (ShowText)
		{
			return EVisibility::SelfHitTestInvisible;
		}
		else
		{
			return EVisibility::Collapsed;
		}
	}
};
