// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

//#include "CoreMinimal.h"
//#include "UObject/ObjectMacros.h"
//#include "HTNOperator.h"
#include "HTNEffect.h"
#include "BehaviorTree/BehaviorTreeTypes.h"

#include "HTNBuilder.generated.h"


USTRUCT(BlueprintType)
struct HTNRUNTIME_API FHTNBuilder_PrimitiveTask
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		class UHTNOperator* Operator;

	UPROPERTY(EditAnywhere, EditFixedSize)
		TArray<FBlackboardKeySelector> BlackboardKeys;

	UPROPERTY(EditAnywhere)
		TArray<FHTNEffect> Effects;

	FHTNBuilder_PrimitiveTask() : Operator(nullptr) {}

};

	// TODO: Update keys array on operator change!
/*	
	void SetOperator(UHTNOperator* NewOperator)
	{
		Operator = NewOperator;
		BlackboardKeys.Empty();

		if (Operator)
		{
			Operator->GetBlackboardParameters(BlackboardKeys);
		}
	}
	*/
/*
struct HTNEDITOR_API FHTNBuilder_Method
{
	TArray<FHTNCondition> Conditions;
	TArray<FName> Tasks;

	FHTNBuilder_Method()
	{}
	explicit FHTNBuilder_Method(const FHTNCondition& InCondition)
	{
		if (ensure(InCondition.IsValid()))
		{
			Conditions.Add(InCondition);
		}
	}
	FHTNBuilder_Method(const TArray<FHTNCondition>& InConditions)
		: Conditions(InConditions)
	{}
	void AddTask(const FName& TaskName) { Tasks.Add(TaskName); }
};

struct HTNEDITOR_API FHTNBuilder_CompositeTask
{
	TArray<FHTNBuilder_Method> Methods;

	FHTNBuilder_Method& AddMethod()
	{
		return Methods[Methods.Add(FHTNBuilder_Method())];
	}

	FHTNBuilder_Method& AddMethod(const FHTNCondition& Condition)
	{
		return Methods[Methods.Add(FHTNBuilder_Method(Condition))];
	}

	FHTNBuilder_Method& AddMethod(const TArray<FHTNCondition>& Conditions)
	{
		return Methods[Methods.Add(FHTNBuilder_Method(Conditions))];
	}
};

struct HTNEDITOR_API FHTNBuilder_Domain
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
