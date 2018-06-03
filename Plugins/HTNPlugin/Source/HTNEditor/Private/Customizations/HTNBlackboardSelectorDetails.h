// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"
#include "UnrealClient.h"
#include "IPropertyTypeCustomization.h"

class IPropertyHandle;
class UBlackboardData;

// Slightly modified version of FBlackboardKeySelectorCustomization
// The original class relies on UBTNode to find the Blackboard Data
class FHTNBlackboardKeySelectorCustomization : public IPropertyTypeCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	/** IPropertyTypeCustomization interface */
	virtual void CustomizeHeader( TSharedRef<class IPropertyHandle> StructPropertyHandle, class FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& StructCustomizationUtils ) override;
	virtual void CustomizeChildren( TSharedRef<class IPropertyHandle> StructPropertyHandle, class IDetailChildrenBuilder& StructBuilder, IPropertyTypeCustomizationUtils& StructCustomizationUtils ) override;

protected:

	void CacheBlackboardData();
	const class UHTNAsset* FindHTNAsset(UObject* InObj);

	// returns true if succesfully updated all the pointers
	bool UpdateCachedAssets(UObject* InObj);

	void InitKeyFromProperty();
	void OnKeyComboChange(int32 Index);
	TSharedRef<SWidget> OnGetKeyContent() const;
	FText GetCurrentKeyDesc() const;
	bool IsEditingEnabled() const;

	TSharedPtr<IPropertyHandle> MyStructProperty;
	TSharedPtr<IPropertyHandle> MyKeyNameProperty;
	TSharedPtr<IPropertyHandle> MyKeyIDProperty;
	TSharedPtr<IPropertyHandle> MyKeyClassProperty;

	/** cached names of keys */	
	TArray<FName> KeyValues;

	bool bNoneIsAllowedValue;

	/** cached blackboard asset */
	TWeakObjectPtr<class UBlackboardData> CachedBlackboardAsset;
	TWeakObjectPtr<class UHTNAsset> CachedHTNAsset;

	/** property utils */
	class IPropertyUtilities* PropUtils;

	/** callbacks */
	void OnBlackboardChanged();
	FDelegateHandle OnBlackboardChangedHandle;
};
