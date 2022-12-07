#include "pch.h"
#include "PacketSend.h"
#include "structs.h"

PacketSend::PacketSend()
{
	moduleBase = (uintptr_t)GetModuleHandle(NULL);
	PacketMsg = FnPacketLPCSTR(moduleBase + 0xF8A40);

	PacketStart = FnPacketInt(moduleBase + 0xF8290);

	PacketContainer = FnPacketInt(moduleBase + 0xF8700);
	PacketChannelId = FnPacketInt(moduleBase + 0xF8560);


	PacketEnd = FnPacketByte(moduleBase + 0xF8E40);
	PacketPlayerId = FnPacketInt(moduleBase + 0xF88A0);
}


void PacketSend::Say(const char* msgToSay)
{
	PacketStart(0x96);
	PacketChannelId(CHANNEL_ID::SAY);
	PacketMsg(msgToSay);
	PacketEnd(1);
}


void PacketSend::UseItemWithFromContainerToCreature(uint16_t itemId,uint32_t creatureId, BYTE slotNumber, int32_t toXPos, int32_t toYPos, int32_t toZPos)
{
	PacketStart(0x83);
	PacketContainer(65535);
	PacketContainer(64);
	PacketChannelId(slotNumber);
	PacketContainer(itemId);
	PacketChannelId(slotNumber);
	PacketContainer(toXPos);
	PacketContainer(toYPos);
	PacketChannelId(toZPos);
	PacketContainer(creatureId);
	PacketChannelId(1);
	PacketEnd(1);
}

void PacketSend::UseItemOn(int32_t itemId, uint32_t creatureId)
{
	PacketStart(0x84);
	PacketContainer(65535);
	PacketContainer(0);
	PacketChannelId(0);
	PacketContainer(itemId);
	PacketChannelId(0);
	PacketPlayerId(creatureId);
	PacketEnd(1);
}