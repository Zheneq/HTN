// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Templates/SharedPointer.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

class FText;
class ISlateStyle;
class UHTNAsset;

class SButton;
class SWrapBox;


/**
 * Implements the UTextAsset asset editor widget.
 */
class SHTNCompositeTask
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SHTNCompositeTask) { }
	SLATE_END_ARGS()

public:

	/** Virtual destructor. */
	virtual ~SHTNCompositeTask();

	/**
	 * Construct this widget
	 *
	 * @param InArgs The declaration data for this widget.
	 * @param InHTNEditor The UHTNEditorToolkit to represent.
	 * @param InStyleSet The style set to use.
	 */
	void Construct(const FArguments& InArgs, TWeakPtr<class FHTNEditorToolkit> InHTNEditor, const TSharedRef<ISlateStyle>& InStyle);

private:

	void HandleAssetPropertyChanged(UObject* Object, FPropertyChangedEvent& PropertyChangedEvent);

	// Add method button
	FReply HandleNewMethod();

	// Update the widget
	void Update();

private:
	TSharedPtr<ISlateStyle> Style;

	TSharedPtr<SWrapBox> PanelRoot;
	TSharedPtr<SButton> ButtonNewMethod;

	TWeakPtr<class FHTNEditorToolkit> HTNEditor;
};
