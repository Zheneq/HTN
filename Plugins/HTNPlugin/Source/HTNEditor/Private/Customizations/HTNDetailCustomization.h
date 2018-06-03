// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"

class IDetailLayoutBuilder;
class IPropertyHandle;

/**
* Provides the customization for all UWidgets.  Bindings, style disabling...etc.
*/

class FHTNAssetCustomization : public IDetailCustomization
{
public:

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;
};

class FHTNEffectCustomization : public IDetailCustomization
{
public:

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailLayout) override;
};
