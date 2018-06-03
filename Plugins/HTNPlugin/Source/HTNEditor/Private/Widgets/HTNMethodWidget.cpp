// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "HTNMethodWidget.h"

#include "Fonts/SlateFontInfo.h"
#include "HTNEditorLog.h"
#include "Toolkits/HTNEditorToolkit.h"
#include "HTNBuilder.h"
#include "UObject/Class.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SGridPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"

#include "HTNEditorSettings.h"


#define LOCTEXT_NAMESPACE "SHTNEditor"


void SHTNMethod::Construct(const FArguments& InArgs, TWeakObjectPtr<UHTNAsset> InHTNAsset, const TSharedRef<ISlateStyle>& InStyle)
{
	HTNAsset = InHTNAsset;
	auto Settings = GetDefault<UHTNEditorSettings>();

	ChildSlot
	[
		SNew(SBorder)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
				.FillHeight(0.1f)
					[
						SNew(SHorizontalBox)
						+ SHorizontalBox::Slot()
						.FillWidth(1.0f)
							[
								SNew(SButton)
								.OnClicked(this, &SHTNMethod::HandleMove, true)
									[
										SNew(STextBlock)
//											.Font((Settings != nullptr) ? Settings->Font : FSlateFontInfo())
											.Text(LOCTEXT("MoveMethodUpBtnCaption", "<"))
									]
							]
						.FillWidth(1.0f)
							[
								SNew(SButton)
								.OnClicked(this, &SHTNMethod::HandleMove, false)
									[
										SNew(STextBlock)
//											.Font((Settings != nullptr) ? Settings->Font : FSlateFontInfo())
											.Text(LOCTEXT("MoveMethodDownBtnCaption", ">"))
									]
							]
					]
				.FillHeight(0.4f)
					[
						SAssignNew(VBConditions, SVerticalBox)
					]
				.FillHeight(0.1f)
					[
						SNew(SBorder)
					]
				.FillHeight(1.0f)
					[
						SAssignNew(VBTasks, SVerticalBox)
					]
		]
	];
}


void SHTNMethod::Update(const FHTNBuilder_Method& Method)
{
	UE_LOG(LogHTNEditor, Log, TEXT("SHTNMethod::Update"));

	if (VBConditions.IsValid() && VBTasks.IsValid() && HTNAsset.IsValid())
	{
		VBConditions->ClearChildren();
		VBTasks->ClearChildren();

		for (const auto& Cond : Method.Conditions)
		{
			VBConditions->AddSlot()[SNew(STextBlock).Text(FText::FromString(Cond.BuildDescription()))];
		}

		for (const auto& TaskName : Method.Tasks)
		{
			VBTasks->AddSlot()[SNew(STextBlock).Text(FText::FromName(TaskName))];
		}
	}
}

/* callbacks
 *****************************************************************************/

FReply SHTNMethod::HandleMove(bool bMoveUp)
{
	OnMethodMove.Broadcast(bMoveUp);
	return FReply::Handled();
}


#undef LOCTEXT_NAMESPACE
