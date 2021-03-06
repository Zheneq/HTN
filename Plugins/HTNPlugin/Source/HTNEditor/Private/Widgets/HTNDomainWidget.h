// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Templates/SharedPointer.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/HTNTaskListWidget.h"

class FText;
class ISlateStyle;
class UHTNAsset;

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
	SHTNDomain() :
		PendingRenameTaskId(INDEX_NONE)
	{}

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
		int32 Id;
		FText DisplayName;
		bool bRoot;
		bool bIsComposite;

		FHTNTaskViewModel(int32 TaskId, FHTNBuilder_PrimitiveTask Task) :
			Id(TaskId), DisplayName(Task.DisplayName), bRoot(false), bIsComposite(false) {}
		FHTNTaskViewModel(int32 TaskId, FHTNBuilder_CompositeTask Task) :
			Id(TaskId), DisplayName(Task.DisplayName), bRoot(false), bIsComposite(true) {}
	};

	typedef SHTNTaskList<TSharedPtr<FHTNTaskViewModel>> SHTNTaskListView;

	TArray<TSharedPtr<FHTNTaskViewModel>> CachedPrimitiveTasks;
	TArray<TSharedPtr<FHTNTaskViewModel>> CachedCompositeTasks;

private:
	/** Callback for property changes in the text asset. */
	void HandleAssetPropertyChanged(UObject* Object, FPropertyChangedEvent& PropertyChangedEvent);

	void HandleTaskSelectionChanged(TSharedPtr<FHTNTaskViewModel> SelectedItem, ESelectInfo::Type SelectInfo);

	void Update();

	TSharedRef<class ITableRow> CreateTaskWidget(TSharedPtr<FHTNTaskViewModel> InItem, const TSharedRef<class STableViewBase>& OwnerTable);

	// Button callbacks
	FReply HandleNewPrimitiveTask();
	FReply HandleNewCompositeTask();

	// Task we want to rename ASAP
	int32 PendingRenameTaskId;

	void RenameTask(int32 TaskId, const FText& NewName);


private:
	TWeakPtr<class FHTNEditorToolkit> HTNEditor;
	TWeakObjectPtr<UHTNAsset> HTNAsset;

	TSharedPtr<SHTNTaskListView> ListPrimitiveTasks;
	TSharedPtr<SHTNTaskListView> ListCompositeTasks;

};
