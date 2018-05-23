// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "HTNCondition.generated.h"

USTRUCT(BlueprintType)
struct FHTNCondition
{
	GENERATED_BODY()

	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector BlackboardKey;

	/** operation type */
	UPROPERTY()
		uint8 OperationType;

	/** value for arithmetic operations */
	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Key Value"))
		int32 IntValue;

	/** value for arithmetic operations */
	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Key Value"))
		float FloatValue;

	/** value for string operations */
	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Key Value"))
		FString StringValue;

#if WITH_EDITORONLY_DATA

	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Key Query"))
		TEnumAsByte<EBasicKeyOperation::Type> BasicOperation;

	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Key Query"))
		TEnumAsByte<EArithmeticKeyOperation::Type> ArithmeticOperation;

	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Key Query"))
		TEnumAsByte<ETextKeyOperation::Type> TextOperation;

#endif

	bool EvaluateOnBlackboard(const UBlackboardComponent& BlackboardComp) const
	{
		bool bResult = false;
		if (BlackboardKey.SelectedKeyType)
		{
			UBlackboardKeyType* KeyCDO = BlackboardKey.SelectedKeyType->GetDefaultObject<UBlackboardKeyType>();
			const uint8* KeyMemory = BlackboardComp.GetKeyRawData(BlackboardKey.GetSelectedKeyID());

			// KeyMemory can be NULL if the blackboard has its data setup wrong, so we must conditionally handle that case.
			if (ensure(KeyCDO != NULL) && (KeyMemory != NULL))
			{
				const EBlackboardKeyOperation::Type Op = KeyCDO->GetTestOperation();
				switch (Op)
				{
				case EBlackboardKeyOperation::Basic:
					bResult = KeyCDO->WrappedTestBasicOperation(BlackboardComp, KeyMemory, (EBasicKeyOperation::Type)OperationType);
					break;

				case EBlackboardKeyOperation::Arithmetic:
					bResult = KeyCDO->WrappedTestArithmeticOperation(BlackboardComp, KeyMemory, (EArithmeticKeyOperation::Type)OperationType, IntValue, FloatValue);
					break;

				case EBlackboardKeyOperation::Text:
					bResult = KeyCDO->WrappedTestTextOperation(BlackboardComp, KeyMemory, (ETextKeyOperation::Type)OperationType, StringValue);
					break;

				default:
					break;
				}
			}
		}

		return bResult;
	}
};

