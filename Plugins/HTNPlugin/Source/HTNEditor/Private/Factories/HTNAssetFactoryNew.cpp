// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "HTNAssetFactoryNew.h"

#include "HTNAsset.h"


/* UTextAssetFactoryNew structors
 *****************************************************************************/

UHTNAssetFactoryNew::UHTNAssetFactoryNew(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UHTNAsset::StaticClass();
	bCreateNew = true;
	bEditAfterNew = true;
}


/* UFactory overrides
 *****************************************************************************/

UObject* UHTNAssetFactoryNew::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UHTNAsset>(InParent, InClass, InName, Flags);
}


bool UHTNAssetFactoryNew::ShouldShowInNewMenu() const
{
	return true;
}
