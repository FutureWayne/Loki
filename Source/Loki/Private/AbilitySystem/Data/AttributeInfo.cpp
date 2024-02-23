// Copyright Ludens Studio


#include "AbilitySystem/Data/AttributeInfo.h"

FLokiAttributeInfo UAttributeInfo::FindAttributeByTag(const FGameplayTag& Tag, bool bLogNotFound) const
{
	for (const FLokiAttributeInfo& AttributeInfo : AttributeInfos)
	{
		if (AttributeInfo.AttributeTag.MatchesTagExact(Tag))
		{
			return AttributeInfo;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, TEXT("AttributeInfo for tag %s not found"), *Tag.ToString());
	}

	return FLokiAttributeInfo();
}
