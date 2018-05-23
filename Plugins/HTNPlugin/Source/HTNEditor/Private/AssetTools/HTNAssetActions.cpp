// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "HTNAssetActions.h"

#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "HTNAsset.h"
#include "Styling/SlateStyle.h"

#include "HTNEditorToolkit.h"


#define LOCTEXT_NAMESPACE "AssetTypeActions"


/* FHTNAssetActions constructors
 *****************************************************************************/

FHTNAssetActions::FHTNAssetActions(const TSharedRef<ISlateStyle>& InStyle)
	: Style(InStyle)
{ }


/* FAssetTypeActions_Base overrides
 *****************************************************************************/

bool FHTNAssetActions::CanFilter()
{
	return true;
}


uint32 FHTNAssetActions::GetCategories()
{
	return EAssetTypeCategories::Misc;
}


FText FHTNAssetActions::GetName() const
{
	return NSLOCTEXT("AssetTypeActions", "AssetTypeActions_HTNAsset", "Hierarchial Task Network");
}


UClass* FHTNAssetActions::GetSupportedClass() const
{
	return UHTNAsset::StaticClass();
}


FColor FHTNAssetActions::GetTypeColor() const
{
	return FColor(149, 70, 255);
}


void FHTNAssetActions::OpenAssetEditor(const TArray<UObject*>& InObjects, TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	EToolkitMode::Type Mode = EditWithinLevelEditor.IsValid()
		? EToolkitMode::WorldCentric
		: EToolkitMode::Standalone;

	for (auto ObjIt = InObjects.CreateConstIterator(); ObjIt; ++ObjIt)
	{
		auto HTNAsset = Cast<UHTNAsset>(*ObjIt);

		if (HTNAsset != nullptr)
		{
			TSharedRef<FHTNEditorToolkit> EditorToolkit = MakeShareable(new FHTNEditorToolkit(Style));
			EditorToolkit->Initialize(HTNAsset, Mode, EditWithinLevelEditor);
		}
	}
}


#undef LOCTEXT_NAMESPACE
