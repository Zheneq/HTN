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

private:

	UPROPERTY(EditDefaultsOnly)
		class UBlackboardData* BlackboardData;

public:
	class UBlackboardData* GetBlackboardData() { return BlackboardData; }

#if WITH_EDITORONLY_DATA
	DECLARE_MULTICAST_DELEGATE(FHTNEvent);

	FHTNEvent OnBlackboardChanged;

#endif // WITH_EDITORONLY_DATA

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere)
		TMap<int32, FHTNBuilder_PrimitiveTask> PrimitiveTasks;

	UPROPERTY(EditAnywhere)
		TMap<int32, FHTNBuilder_CompositeTask> CompositeTasks;


#endif // WITH_EDITORONLY_DATA

public:
	/** Holds the stored text. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="TextAsset")
	FText Text;

public:
	const TMap<int32, FHTNBuilder_PrimitiveTask>& GetPrimitiveTasks() { return PrimitiveTasks; }
	const TMap<int32, FHTNBuilder_CompositeTask>& GetCompositeTasks() { return CompositeTasks; }

	friend class FHTNEditorToolkit;
};
