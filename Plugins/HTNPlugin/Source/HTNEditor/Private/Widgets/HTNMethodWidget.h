// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Templates/SharedPointer.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SCompoundWidget.h"

class FText;
class ISlateStyle;
class UHTNAsset;

class SButton;
class SMultiLineEditableTextBox;


/**
 * Implements the UTextAsset asset editor widget.
 */
class SHTNMethod
	: public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SHTNMethod) { }
	SLATE_END_ARGS()

public:
	/**
	 * Construct this widget
	 *
	 * @param InArgs The declaration data for this widget.
	 * @param InHTNEditor The UHTNEditorToolkit to represent.
	 * @param InStyleSet The style set to use.
	 */
	void Construct(const FArguments& InArgs, TWeakObjectPtr<UHTNAsset> InHTNAsset, const TSharedRef<ISlateStyle>& InStyle);

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnMethodMove, bool);

	FOnMethodMove OnMethodMove;

	// Update the widget
	void Update(const struct FHTNBuilder_Method& Method);

private:
	// Move this method up or down
	FReply HandleMove(bool bMoveUp);

private:
	TWeakObjectPtr<UHTNAsset> HTNAsset;

	TSharedPtr<SVerticalBox> VBConditions;
	TSharedPtr<SVerticalBox> VBTasks;

};
