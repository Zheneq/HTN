// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "HTNOperator.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class HTNRUNTIME_API UHTNOperator : public UObject
{
	GENERATED_BODY()
	
public:
	// void GetBlackboardParameters(TArray<FBlackboardKeySelector>& outBlackboardKeys);
	
	
};
