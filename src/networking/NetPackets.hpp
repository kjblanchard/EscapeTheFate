#pragma once
#include <cstdint>

#pragma pack(push, 1)

enum class PacketType : uint8_t {
	PlayerMove = 0x01,
	PlayerInteract = 0x02,
	BattleAction = 0x03,
	PlayerJoin = 0x04,
	PlayerLeave = 0x05,
};

struct PacketHeader {
	PacketType type;
};

struct PlayerMovePacket {
	PacketHeader header;
	float x;
	float y;
	uint8_t direction;
	uint8_t moving;
};

struct PlayerInteractPacket {
	PacketHeader header;
	float x;
	float y;
};

struct BattleActionPacket {
	PacketHeader header;
	uint8_t battlerNum;
	int16_t damage;
};

struct PlayerJoinPacket {
	PacketHeader header;
	uint8_t playerSlot;
	uint8_t characterIndex;
};

struct PlayerLeavePacket {
	PacketHeader header;
	uint8_t playerSlot;
};

#pragma pack(pop)
