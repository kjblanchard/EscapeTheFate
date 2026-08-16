#pragma once
#include <cstdint>

#pragma pack(push, 1)

enum class PacketType : uint8_t {
	PlayerMove = 0x01,
	PlayerInteract = 0x02,
	BattleAction = 0x03,
	PlayerJoin = 0x04,
	PlayerLeave = 0x05,
	SceneChange = 0x06,
	BattleStart = 0x07,
	BattleUIState = 0x08,
	BattleEnemyAction = 0x09,
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

struct SceneChangePacket {
	PacketHeader header;
	uint8_t loadLocation;
	char mapName[32];
};

struct BattleStartPacket {
	PacketHeader header;
	uint8_t battleGroup;
	char battleScene[32];
};

struct BattleUIStatePacket {
	PacketHeader header;
	uint8_t battlerState;
	uint8_t menuCursor;
	uint8_t magicMenuRow;
	uint8_t magicMenuCol;
	uint8_t targetIndex;
	uint8_t targetingFriendly;
	uint8_t selectedAbilityID;
};

struct BattleEnemyActionPacket {
	PacketHeader header;
	uint8_t enemySlot;
	uint8_t abilityID;
	uint8_t targetSlot;
};

#pragma pack(pop)
