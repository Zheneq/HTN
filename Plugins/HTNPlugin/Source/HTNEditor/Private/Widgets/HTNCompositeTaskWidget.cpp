// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "HTNCompositeTaskWidget.h"

#include "Fonts/SlateFontInfo.h"
#include "HTNEditorLog.h"
#include "Toolkits/HTNEditorToolkit.h"
#include "HTNBuilder.h"
#include "UObject/Class.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SWrapBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/HTNMethodWidget.h"

#include "HTNEditorSettings.h"


#define LOCTEXT_NAMESPACE "SHTNEditor"


/* SHTNAssetEditor interface
 *****************************************************************************/

SHTNCompositeTask::~SHTNCompositeTask()
{
	FCoreUObjectDelegates::OnObjectPropertyChanged.RemoveAll(this);
}


void SHTNCompositeTask::Construct(const FArguments& InArgs, FHTNEditorToolkit* InHTNEditor, const TSharedRef<ISlateStyle>& InStyle)
{
	HTNEditor = InHTNEditor;
	Style = InStyle;

	auto Settings = GetDefault<UHTNEditorSettings>();

	ChildSlot
	[
		SAssignNew(PanelRoot, SWrapBox)
		+ SWrapBox::Slot()
			[
				SAssignNew(ButtonNewMethod, SButton)
				.OnClicked(this, &SHTNCompositeTask::HandleNewMethod)
				[
					SNew(STextBlock)
						.Font((Settings != nullptr) ? Settings->Font : FSlateFontInfo())
						.Text(LOCTEXT("NewMethodBtnCaption", "Add Method"))
				]
			]
	];

	FCoreUObjectDelegates::OnObjectPropertyChanged.AddSP(this, &SHTNCompositeTask::HandleAssetPropertyChanged);
}


void SHTNCompositeTask::Update()
{
	UE_LOG(LogHTNEditor, Log, TEXT("SHTNCompositeTask::Update"));

	FHTNBuilder_CompositeTask* CompTask = HTNEditor ? HTNEditor->GetSelectedCompositeTask() : nullptr;

	if (CompTask)
	{
		PanelRoot->ClearChildren();

		for (const FHTNBuilder_Method& Method : CompTask->Methods)
		{
			TSharedPtr<SHTNMethod> Widget = SNew(SHTNMethod, HTNEditor->GetAsset(), Style.ToSharedRef());
			Widget->Update(Method);
			PanelRoot->AddSlot()[Widget.ToSharedRef()];
		}

		PanelRoot->AddSlot()[ButtonNewMethod.ToSharedRef()];
	}
}

/* callbacks
 *****************************************************************************/

void SHTNCompositeTask::HandleAssetPropertyChanged(UObject* Object, FPropertyChangedEvent& PropertyChangedEvent)
{
	if (HTNEditor && Object == HTNEditor->GetAsset())
	{
		Update();
	}
}


FReply SHTNCompositeTask::HandleNewMethod()
{
	FHTNBuilder_CompositeTask* CompTask = HTNEditor ? HTNEditor->GetSelectedCompositeTask() : nullptr;
	if (CompTask)
	{
		CompTask->Methods.AddDefaulted();

		// TODO: Remove if redundant
		Update();
	}
	else
	{
		UE_LOG(LogHTNEditor, Warning, TEXT("SHTNCompositeTask::HandleNewMethod: No Composite task selected!"));
	}
	return FReply::Handled();
}


#undef LOCTEXT_NAMESPACE
