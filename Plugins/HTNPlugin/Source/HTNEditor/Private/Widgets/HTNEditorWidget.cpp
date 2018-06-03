// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "HTNEditorWidget.h"

#include "Fonts/SlateFontInfo.h"
#include "Internationalization/Text.h"
#include "HTNAsset.h"
#include "UObject/Class.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SMultiLineEditableTextBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"

#include "HTNEditorSettings.h"


#define LOCTEXT_NAMESPACE "SHTNEditor"


/* SHTNAssetEditor interface
 *****************************************************************************/

SHTNEditor::~SHTNEditor()
{
	FCoreUObjectDelegates::OnObjectPropertyChanged.RemoveAll(this);
}


void SHTNEditor::Construct(const FArguments& InArgs, UHTNAsset* InHTNAsset, const TSharedRef<ISlateStyle>& InStyle)
{
	HTNAsset = InHTNAsset;

	auto Settings = GetDefault<UHTNEditorSettings>();

	ChildSlot
	[
		SNew(SVerticalBox)

		+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SAssignNew(EditableTextBox, SMultiLineEditableTextBox)
					.BackgroundColor((Settings != nullptr) ? Settings->BackgroundColor : FLinearColor::White)
					.Font((Settings != nullptr) ? Settings->Font : FSlateFontInfo())
					.ForegroundColor((Settings != nullptr) ? Settings->ForegroundColor : FLinearColor::Black)
					.Margin((Settings != nullptr) ? Settings->Margin : 4.0f)
					.OnTextChanged(this, &SHTNEditor::HandleEditableTextBoxTextChanged)
					.OnTextCommitted(this, &SHTNEditor::HandleEditableTextBoxTextCommitted)
					.Text(HTNAsset->Text)
			]
		+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SNew(SButton)
				[
					SNew(STextBlock)
						.Font((Settings != nullptr) ? Settings->Font : FSlateFontInfo())
						.Text(LOCTEXT("NewPrimTaskBtnCaption", "New Primitive Task (disabled)"))
				]
			]
	];

	FCoreUObjectDelegates::OnObjectPropertyChanged.AddSP(this, &SHTNEditor::HandleHTNAssetPropertyChanged);
}


/* SHTNAssetEditor callbacks
 *****************************************************************************/

void SHTNEditor::HandleEditableTextBoxTextChanged(const FText& NewText)
{
	HTNAsset->MarkPackageDirty();
}


void SHTNEditor::HandleEditableTextBoxTextCommitted(const FText& Comment, ETextCommit::Type CommitType)
{
	HTNAsset->Text = EditableTextBox->GetText();
}


void SHTNEditor::HandleHTNAssetPropertyChanged(UObject* Object, FPropertyChangedEvent& PropertyChangedEvent)
{
	if (Object == HTNAsset)
	{
		EditableTextBox->SetText(HTNAsset->Text);
	}
}

#undef LOCTEXT_NAMESPACE
