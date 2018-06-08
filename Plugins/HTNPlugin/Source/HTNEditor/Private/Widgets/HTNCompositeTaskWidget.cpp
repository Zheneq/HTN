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


void SHTNCompositeTask::Construct(const FArguments& InArgs, TWeakPtr<FHTNEditorToolkit> InHTNEditor, const TSharedRef<ISlateStyle>& InStyle)
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

	const FHTNBuilder_CompositeTask* CompTask = HTNEditor.IsValid() ? HTNEditor.Pin()->GetSelectedCompositeTask() : nullptr;

	if (CompTask)
	{
		PanelRoot->ClearChildren();

		for (const FHTNBuilder_Method& Method : CompTask->Methods)
		{
			TSharedPtr<SHTNMethod> Widget = SNew(SHTNMethod, HTNEditor.Pin()->GetAsset(), Style.ToSharedRef());
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
	if (HTNEditor.IsValid() && Object == HTNEditor.Pin()->GetAsset())
	{
		Update();
	}
}


FReply SHTNCompositeTask::HandleNewMethod()
{
	if (HTNEditor.IsValid())
	{
		HTNEditor.Pin()->AddNewMethodToSelectedCompositeTask();
	}
	
	return FReply::Handled();
}


#undef LOCTEXT_NAMESPACE
