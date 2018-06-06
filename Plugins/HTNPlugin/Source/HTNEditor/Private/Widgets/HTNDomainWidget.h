// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Templates/SharedPointer.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

class FText;
class ISlateStyle;
class UHTNAsset;

template <typename ItemType> class SListView;

/**
 * Implements the UTextAsset asset editor widget.
 */
class SHTNDomain
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SHTNDomain) { }
	SLATE_END_ARGS()

public:

	/** Virtual destructor. */
	virtual ~SHTNDomain();

	/**
	 * Construct this widget
	 *
	 * @param InArgs The declaration data for this widget.
	 * @param InHTNEditor The UHTNEditorToolkit to represent.
	 * @param InStyleSet The style set to use.
	 */
	void Construct(const FArguments& InArgs, TWeakPtr<class FHTNEditorToolkit> InHTNEditor, const TSharedRef<ISlateStyle>& InStyle);

private:
	struct FHTNTaskViewModel
	{
		FName Name;
		FText DisplayName;
		bool bRoot;
		bool bIsComposite;

		FHTNTaskViewModel(FName TaskName, FHTNBuilder_PrimitiveTask Task) :
			Name(TaskName), DisplayName(Task.DisplayName), bRoot(false), bIsComposite(false) {}
		FHTNTaskViewModel(FName TaskName, FHTNBuilder_CompositeTask Task) :
			Name(TaskName), DisplayName(Task.DisplayName), bRoot(false), bIsComposite(true) {}
	};

	typedef SListView<TSharedPtr<FHTNTaskViewModel>> SHTNTaskListView;

	TArray<TSharedPtr<FHTNTaskViewModel>> CachedPrimitiveTasks;
	TArray<TSharedPtr<FHTNTaskViewModel>> CachedCompositeTasks;

	TSharedRef<SHTNTaskListView> MakeListWidget(const TArray<TSharedPtr<FHTNTaskViewModel>>* ListItemsSource, const FText& Caption);


private:
	/** Callback for property changes in the text asset. */
	void HandleAssetPropertyChanged(UObject* Object, FPropertyChangedEvent& PropertyChangedEvent);

	void HandleTaskSelectionChanged(TSharedPtr<FHTNTaskViewModel> SelectedItem, ESelectInfo::Type SelectInfo);

	void Update();

//	TSharedRef<SWidget> CreateTaskWidget(const struct FHTNBuilder_PrimitiveTask& Task);
//	TSharedRef<SWidget> CreateTaskWidget(const struct FHTNBuilder_CompositeTask& Task);
//	TSharedRef<SWidget> CreateTaskWidget(const FName& Name, const FText& DisplayName, bool bRoot);
	TSharedRef<class ITableRow> CreateTaskWidget(TSharedPtr<FHTNTaskViewModel> InItem, const TSharedRef<class STableViewBase>& OwnerTable);

private:
	TWeakPtr<class FHTNEditorToolkit> HTNEditor;
	TWeakObjectPtr<UHTNAsset> HTNAsset;

	TSharedPtr<SHTNTaskListView> ListPrimitiveTasks;
	TSharedPtr<SHTNTaskListView> ListCompositeTasks;

};
