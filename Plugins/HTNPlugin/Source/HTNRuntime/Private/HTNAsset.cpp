#include "HTNAsset.h"

void UHTNAsset::SetBlackboardData(class UBlackboardData* NewBlackboard)
{
	BlackboardData = NewBlackboard;

#if WITH_EDITORONLY_DATA
	OnBlackboardChanged.Broadcast();
#endif // WITH_EDITORONLY_DATA
}