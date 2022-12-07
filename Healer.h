#pragma once

class Healer : public ISingleton<Healer>
{
private:
	int32_t m_TimerHealerSpells;
	int32_t m_TimerHealerItems;
	int32_t m_TimerHealFriend;
	int32_t m_TimerEquipItem;

public:
	Healer();

	void HealWithSpells(const LightSpell& lightSpell, const MidSpell& midSpell, const HeavySpell& heavySpell);
	void HealWithItems(const HealthPotionItemHealer& healthPotion, const ManaPotionItemHealer& manaPotion);
	void HealPlayer(int32_t safeHp, int32_t healFriendOnHp, std::array<std::string, 100>& friendList, bool sioWithSpell, int32_t selectedMode);
};
