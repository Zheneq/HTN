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

	UPROPERTY(EditDefaultsOnly, BlueprintSetter = SetBlackboardData)
		class UBlackboardData* BlackboardData;

public:
	class UBlackboardData* GetBlackboardData() { return BlackboardData; }

	UFUNCTION(BlueprintSetter)
		void SetBlackboardData(class UBlackboardData* NewBlackboard);

#if WITH_EDITORONLY_DATA
	DECLARE_MULTICAST_DELEGATE(FHTNEvent);

	FHTNEvent OnBlackboardChanged;

	UPROPERTY(EditAnywhere)
		TMap<FName, FHTNBuilder_PrimitiveTask> PrimitiveTasks;

	UPROPERTY(EditAnywhere)
		TMap<FName, FHTNBuilder_CompositeTask> CompositeTasks;


#endif // WITH_EDITORONLY_DATA

	/** Holds the stored text. */
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="TextAsset")
	FText Text;
};
