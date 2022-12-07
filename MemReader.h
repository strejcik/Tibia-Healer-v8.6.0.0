#pragma once

class MemReader : public ISingleton<MemReader>
{
private:

	Self* m_LocalPlayer;
	EquipmentSlots* m_EquipmentSlots;
public:


	MemReader();

	void ReadSelfCharacter(CSelfCharacter* character);
	std::vector<Entity*> ReadVisibleCreatures();




	std::vector<VipStruct*> GetVips();
	EquipmentSlots* GetItemOnEquipmentSlots();
	int32_t GetVipCount();
	Entity* GetEntityInEntityList(uint32_t characterId);


	std::vector<Entity*> GetEntityById(uint32_t creatureId);



	bool IsOnline();
	bool IsMonster(int32_t creatureId);
	bool IsPlayer(int32_t creatureId);
	bool IsNPC(int32_t creatureId);
	bool IsEntityOnScreen(Entity* ent);

};
