// Copyright 1998 - 2018 Epic Games, Inc.All Rights Reserved.

#pragma once

#include "BehaviorTree/BehaviorTreeTypes.h"

#include "HTNBlackboardKeySelector.generated.h"


/** helper struct for defining types of allowed blackboard entries
*  (e.g. only entries holding points and objects derived form actor class) */
USTRUCT(BlueprintType)
struct HTNRUNTIME_API FHTNBlackboardKeySelector : public FBlackboardKeySelector
{
	GENERATED_BODY()
		// The sole purpose of this class is to be able to apply a custom detail customization to it.
};
