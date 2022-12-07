#pragma once

//types of functions
using FnPacketLPCSTR = void(__stdcall*)(const char* msg);
using FnPacketInt = void(__stdcall*)(int32_t num);
using FnPacketByte = void(__stdcall*)(BYTE num);
using FnPacketIntRetInt = int(__stdcall*)(int32_t num);

class PacketSend : public ISingleton<PacketSend>
{
private:
	uintptr_t moduleBase;
public:
	//Function Pointers
	FnPacketLPCSTR PacketMsg = nullptr;

	FnPacketInt PacketStart = nullptr;
	FnPacketInt PacketItem = nullptr;
	FnPacketInt PacketContainer = nullptr;
	FnPacketInt PacketChannelId = nullptr;
	FnPacketInt PacketPlayerId = nullptr;


	
	FnPacketByte PacketEnd = nullptr;


	PacketSend();

	void Say(const char* msgToSay);
	void UseItemOn(int32_t itemId, uint32_t creatureId);
	void UseItemWithFromContainerToCreature(uint16_t itemId, uint32_t creatureId, BYTE slotNumber, int32_t toXPos, int32_t toYPos, int32_t toZPos);
};
