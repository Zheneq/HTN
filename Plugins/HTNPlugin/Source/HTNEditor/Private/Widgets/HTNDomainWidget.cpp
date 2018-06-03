// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "HTNDomainWidget.h"

#include "Fonts/SlateFontInfo.h"
#include "HTNEditorLog.h"
#include "Toolkits/HTNEditorToolkit.h"
#include "HTNBuilder.h"
#include "HTNAsset.h"
#include "UObject/Class.h"
#include "Widgets/Views/SListView.h"
#include "Widgets/Text/STextBlock.h"

#include "HTNEditorSettings.h"


#define LOCTEXT_NAMESPACE "SHTNEditor"

SHTNDomain::~SHTNDomain()
{
	FCoreUObjectDelegates::OnObjectPropertyChanged.RemoveAll(this);
}

void SHTNDomain::Construct(const FArguments& InArgs, TWeakObjectPtr<UHTNAsset> InHTNAsset, const TSharedRef<ISlateStyle>& InStyle)
{
	HTNAsset = InHTNAsset;
	auto Settings = GetDefault<UHTNEditorSettings>();

	ChildSlot
	[
		SNew(SBorder)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
				.FillHeight(1.0f)
					[
						SAssignNew(ListPrimitiveTasks, SVerticalBox)
					]
			+ SVerticalBox::Slot()
				.FillHeight(1.0f)
					[
						SAssignNew(ListCompositeTasks, SVerticalBox)
					]
		]
	];

	FCoreUObjectDelegates::OnObjectPropertyChanged.AddSP(this, &SHTNDomain::HandleAssetPropertyChanged);
}

/* callbacks
 *****************************************************************************/

void SHTNDomain::HandleAssetPropertyChanged(UObject* Object, FPropertyChangedEvent& PropertyChangedEvent)
{
	if (Object == HTNAsset)
	{
		Update();
	}
}

void SHTNDomain::Update()
{
	ListPrimitiveTasks->ClearChildren();
	ListCompositeTasks->ClearChildren();

	for (const auto& Task : HTNAsset->PrimitiveTasks)
	{
		ListPrimitiveTasks->AddSlot()[CreateTaskWidget(Task.Value)];
	}

	for (const auto& Task : HTNAsset->CompositeTasks)
	{
		ListCompositeTasks->AddSlot()[CreateTaskWidget(Task.Value)];
	}
}

TSharedRef<SWidget> SHTNDomain::CreateTaskWidget(const FHTNBuilder_PrimitiveTask& Task)
{
	return SNew(STextBlock).Text(FText::FromName(Task.Name));
}

TSharedRef<SWidget> SHTNDomain::CreateTaskWidget(const FHTNBuilder_CompositeTask& Task)
{
	return SNew(STextBlock).Text(FText::FromName(Task.Name));
}


#undef LOCTEXT_NAMESPACE
