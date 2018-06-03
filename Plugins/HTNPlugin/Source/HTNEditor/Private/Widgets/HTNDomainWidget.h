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
	void Construct(const FArguments& InArgs, TWeakObjectPtr<UHTNAsset> InHTNAsset, const TSharedRef<ISlateStyle>& InStyle);


private:
	/** Callback for property changes in the text asset. */
	void HandleAssetPropertyChanged(UObject* Object, FPropertyChangedEvent& PropertyChangedEvent);

	void Update();

	TSharedRef<SWidget> CreateTaskWidget(const struct FHTNBuilder_PrimitiveTask& Task);
	TSharedRef<SWidget> CreateTaskWidget(const struct FHTNBuilder_CompositeTask& Task);

private:
	TWeakObjectPtr<UHTNAsset> HTNAsset;

	TSharedPtr<SVerticalBox> ListPrimitiveTasks;
	TSharedPtr<SVerticalBox> ListCompositeTasks;

};
