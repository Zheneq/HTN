// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "HTNEffect.generated.h"

USTRUCT(BlueprintType)
struct FHTNEffect
{
	GENERATED_BODY()

	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category = Blackboard)
		FBlackboardKeySelector BlackboardKey;

	/** operation type */
//	UPROPERTY()
//		uint8 OperationType;

	/** whether the effect is guaranteed or expected */
	UPROPERTY(Category = Blackboard, EditAnywhere)
		bool bGuaranteed;

	/** value for operations */
	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Key Value"))
		bool BoolValue;

	/** value for operations */
	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Key Value"))
		float FloatValue;

	/** value for operations */
	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Key Value"))
		int32 IntValue;

	/** value for operations */
	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Key Value"))
		UObject* ObjectValue;

	/** value for operations */
	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Key Value"))
		FRotator RotatorValue;

	/** value for operations */
	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Key Value"))
		FString StringValue;

	/** value for operations */
	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Key Value"))
		FVector VectorValue;
/*
#if WITH_EDITORONLY_DATA

	UPROPERTY(Category = Blackboard, EditAnywhere, meta = (DisplayName = "Key Query"))
		TEnumAsByte<EHTNEffect::Type> Operation;

#endif
*/

	void ApplyToBlackboard(UBlackboardComponent& BlackboardComp) const
	{
		bool bResult = false;
		if (BlackboardKey.SelectedKeyType)
		{
			if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Bool::StaticClass())
			{
				BlackboardComp.SetValue<UBlackboardKeyType_Bool>(BlackboardKey.GetSelectedKeyID(), BoolValue);
			}
			else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Float::StaticClass())
			{
				BlackboardComp.SetValue<UBlackboardKeyType_Float>(BlackboardKey.GetSelectedKeyID(), FloatValue);
			}
			else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Int::StaticClass())
			{
				BlackboardComp.SetValue<UBlackboardKeyType_Int>(BlackboardKey.GetSelectedKeyID(), IntValue);
			}
			else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Object::StaticClass())
			{
				BlackboardComp.SetValue<UBlackboardKeyType_Object>(BlackboardKey.GetSelectedKeyID(), ObjectValue);
			}
			else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Rotator::StaticClass())
			{
				BlackboardComp.SetValue<UBlackboardKeyType_Rotator>(BlackboardKey.GetSelectedKeyID(), RotatorValue);
			}
			else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_String::StaticClass())
			{
				BlackboardComp.SetValue<UBlackboardKeyType_String>(BlackboardKey.GetSelectedKeyID(), StringValue);
			}
			else if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
			{
				BlackboardComp.SetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID(), VectorValue);
			}
		}
	}
};

