// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

//#include "CoreMinimal.h"
//#include "UObject/ObjectMacros.h"
#include "HTNCondition.h"
#include "HTNOperator.h"
#include "HTNEffect.h"
#include "BehaviorTree/BehaviorTreeTypes.h"

#include "HTNBuilder.generated.h"

USTRUCT()
struct HTNRUNTIME_API FHTNBuilder_PrimitiveTask
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FName Name;

	UPROPERTY(EditAnywhere)
		FText DisplayName;

	UPROPERTY(EditAnywhere)
		TArray<FHTNCondition> Conditions;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UHTNOperator> Operator;

	// TODO: Detail customization for this
	UPROPERTY(EditAnywhere, EditFixedSize)
		TArray<FBlackboardKeySelector> BlackboardKeys;

	UPROPERTY(EditAnywhere)
		TArray<FHTNEffect> Effects;
};

USTRUCT()
struct HTNRUNTIME_API FHTNBuilder_Method
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		TArray<FHTNCondition> Conditions;

	UPROPERTY(EditAnywhere)
		TArray<FName> Tasks;
};

USTRUCT()
struct HTNRUNTIME_API FHTNBuilder_CompositeTask
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FName Name;

	UPROPERTY(EditAnywhere)
		FText DisplayName;

	UPROPERTY(EditAnywhere)
		TArray<FHTNBuilder_Method> Methods;
};

/*
struct HTNRUNTIME_API FHTNBuilder_Domain
{
	TSharedPtr<FHTNDomain> DomainInstance;
	
	FHTNBuilder_Domain();
	FHTNBuilder_Domain(const TSharedPtr<FHTNDomain>& InDomain);

	void SetRootName(const FName InRootName) { RootTaskName = InRootName; }
	FHTNBuilder_CompositeTask& AddCompositeTask(const FName& TaskName);
	FHTNBuilder_PrimitiveTask& AddPrimitiveTask(const FName& TaskName);

	FHTNBuilder_CompositeTask* FindCompositeTask(const FName& TaskName);
	FHTNBuilder_PrimitiveTask* FindPrimitiveTask(const FName& TaskName);

	FHTNBuilder_CompositeTask* GetRootAsCompositeTask() { return FindCompositeTask(RootTaskName); }
	FHTNBuilder_PrimitiveTask* GetRootAsPrimitiveTask() { return FindPrimitiveTask(RootTaskName); }
	
	/**	Optimizes stored information. After the Domain is compiled
	 *	it's impossible to extend it
	 *	@return true if compilation was successful. False otherwise, look for warnings and errors in the log *
	bool Compile();
	/** Using DomainInstance information populate this domain builder instance *
	void Decompile();

	FString GetDebugDescription() const;

	FName RootTaskName;
	TMap<FName, FHTNBuilder_PrimitiveTask> PrimitiveTasks;
	TMap<FName, FHTNBuilder_CompositeTask> CompositeTasks;
};
*/
