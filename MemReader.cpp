#include "pch.h"
#include "MemReader.h"

MemReader::MemReader()
{
	m_ModuleBase = (uintptr_t)GetModuleHandle(NULL);
	m_LocalPlayer = (Self*)(m_ModuleBase + Offsets::selfPlayer);
	m_EquipmentSlots = (EquipmentSlots*)(m_ModuleBase + Offsets::equipmentStructAddy);
}

void MemReader::ReadSelfCharacter(CSelfCharacter* character)
{
	character->health = *(int*)(m_ModuleBase + Offsets::m_Health);
	character->maxHealth = *(int*)(m_ModuleBase + Offsets::m_MaxHealth);

	character->mana = *(int*)(m_ModuleBase + Offsets::m_Mana);
	character->maxMana = *(int*)(m_ModuleBase + Offsets::m_MaxMana);
	character->manaPercentage = (int)((float(character->mana) / float(character->maxMana)) * 100);

	character->flags = *(int*)(m_ModuleBase + Offsets::m_Flags);
	character->id = *(int*)(m_ModuleBase + Offsets::m_Id);

	character->level = *(int*)(m_ModuleBase + Offsets::m_Level);
	Entity* selfEnt = nullptr;
	selfEnt = GetEntityInEntityList(character->id);

	if (selfEnt)
	{
		character->hpPercentage = selfEnt->hpPercentage;

		character->xPos = selfEnt->xPos;
		character->yPos = selfEnt->yPos;
		character->zPos = selfEnt->zPos;
		strcpy_s(character->name, selfEnt->name);

		character->outfitId = selfEnt->outfitId;
		character->addonNumber = selfEnt->outfitId;
		character->bodyColor = selfEnt->primaryColor;
		character->bootsColor = selfEnt->detailColor;
		character->headColor = selfEnt->headColor;
		character->direction = selfEnt->direction;
		character->skull = selfEnt->skull;
		character->partyShield = selfEnt->partyShield;
		character->guildShield = selfEnt->guildShield;
	}
}

EquipmentSlots* MemReader::GetItemOnEquipmentSlots()
{
	return m_EquipmentSlots;
}

Entity* MemReader::GetEntityInEntityList(uint32_t characterId)
{
	for (uint32_t i = 0; i < 1300; i++)
	{
		Entity* entity = (Entity*)(m_ModuleBase + Offsets::entityListStartAddy + (Offsets::offsetToAnotherEntity * i));
		if (!entity) continue;

		if ((entity->id == characterId))
		{
			return entity;
		}
	}
	return nullptr;
}

bool MemReader::IsMonster(int32_t creatureId)
{
	return creatureId > 0x1000000000 && creatureId < 2000000000;
}

bool MemReader::IsPlayer(int32_t creatureId)
{
	return creatureId < 0x1000000000 && creatureId > 0x200000000;
}

bool MemReader::IsNPC(int32_t creatureId)
{
	return creatureId > 0x200000000;
}


std::vector<Entity*> MemReader::ReadVisibleCreatures()
{
	std::vector<Entity*> entities;
	CSelfCharacter selfCharacter;
	ReadSelfCharacter(&selfCharacter);

	for (uint32_t i = 0; i < 1300; i++)
	{
		Entity* entity = (Entity*)(m_ModuleBase + Offsets::entityListStartAddy + (Offsets::offsetToAnotherEntity * i));
		if (!entity) continue;

		if ((IsEntityOnScreen(entity)) && (entity->id != selfCharacter.id))
		{
			entities.push_back(entity);
		}
	}
	return entities;
}



bool MemReader::IsEntityOnScreen(Entity* ent)
{
	CSelfCharacter selfCharacter;
	ReadSelfCharacter(&selfCharacter);

	if ((ent->zPos == selfCharacter.zPos) && (abs(ent->xPos - selfCharacter.xPos) <= 7) && (abs(ent->yPos - selfCharacter.yPos) <= 5) && (ent->hpPercentage > 0))
	{
		return true;
	}
	return false;
}



std::vector<Entity*> MemReader::GetEntityById(uint32_t creatureId)
{
	std::vector<Entity*> entities;
	CSelfCharacter selfCharacter;
	ReadSelfCharacter(&selfCharacter);

	for (uint32_t i = 0; i < 1300; i++)
	{
		Entity* entity = (Entity*)(m_ModuleBase + Offsets::entityListStartAddy + (Offsets::offsetToAnotherEntity * i));
		if (!entity) continue;

		if ((IsEntityOnScreen(entity)) && (entity->id != selfCharacter.id) && entity->id == creatureId)
		{
			entities.push_back(entity);
		}
	}
	return entities;
}



std::vector<VipStruct*> MemReader::GetVips()
{
	std::vector<VipStruct*> vips;
	std::vector<uintptr_t> pointers;
	uintptr_t newNode = mem::FindDMAAddy2(m_ModuleBase + Offsets::vipEntryPoint, {});
	int32_t vipsCount = GetVipCount();

	for (uint32_t i = 0; i < vipsCount; i++)
	{
		newNode = mem::FindDMAAddy2(newNode, { });
		VipStruct* vipEnt = (VipStruct*)(mem::FindDMAAddy2(newNode, { }));
		vips.push_back(vipEnt);
	}


	return vips;
}

int32_t MemReader::GetVipCount()
{
	int32_t vipCount = *(int32_t*)(m_ModuleBase + Offsets::vipCount);
	return vipCount;
}



bool MemReader::IsOnline()
{
	return (*(BYTE*)(m_ModuleBase + Offsets::m_ConnectionState) == CONNECTION_STATE::ONLINE ? true : false);
}




