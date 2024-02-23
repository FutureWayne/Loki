// Copyright Ludens Studio


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetCharacterClassDefaultInfo(const ECharacterClass CharacterClass)
{
	return CharacterClassInfo.FindChecked(CharacterClass);
}
