// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Internationalization/Text.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "HTNBuilder.h"

#include "HTNAsset.generated.h"


/**
 * Implements a Hierarchial Task Network asset.
 */
UCLASS(BlueprintType, hidecategories=(Object))
class HTNRUNTIME_API UHTNAsset
	: public UObject
{
	GENERATED_BODY()

public:

#if WITH_EDITORONLY_DATA

	UPROPERTY(EditAnywhere)
		TArray<FHTNBuilder_PrimitiveTask> PrimitiveTasks;


#endif // WITH_EDITORONLY_DATA

	/** Holds the stored text. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="TextAsset")
	FText Text;
};
