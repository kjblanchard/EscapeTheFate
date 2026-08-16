#include <Supergoon/Networking/client.h>
#include <sgtools/log.h>

#include <cstring>
#include <engine.hpp>
#include <gameState.hpp>
#include <gameobject/gameobjects/RemotePlayer.hpp>
#include <networking/NetPackets.hpp>
#include <systems/BattleTransitionSystem.hpp>
#include <systems/NetworkSystem.hpp>
#include <systems/battleSystem.hpp>

using namespace Etf;

namespace {

NetClient* _client = nullptr;
bool _connected = false;
bool _connecting = false;
bool _failed = false;

// Receive buffer for TCP stream reassembly
uint8_t _recvBuf[4096];
size_t _recvBufLen = 0;

size_t packetSizeForType(PacketType type) {
	switch (type) {
		case PacketType::PlayerMove: return sizeof(PlayerMovePacket);
		case PacketType::PlayerInteract: return sizeof(PlayerInteractPacket);
		case PacketType::BattleAction: return sizeof(BattleActionPacket);
		case PacketType::PlayerJoin: return sizeof(PlayerJoinPacket);
		case PacketType::PlayerLeave: return sizeof(PlayerLeavePacket);
		case PacketType::SceneChange: return sizeof(SceneChangePacket);
		case PacketType::BattleStart: return sizeof(BattleStartPacket);
		case PacketType::BattleUIState: return sizeof(BattleUIStatePacket);
		case PacketType::BattleEnemyAction: return sizeof(BattleEnemyActionPacket);
	}
	return 0;
}

void dispatchPacket(const uint8_t* data, size_t size) {
	if (size < sizeof(PacketHeader)) return;
	auto* hdr = reinterpret_cast<const PacketHeader*>(data);
	switch (hdr->type) {
		case PacketType::PlayerMove: {
			if (size < sizeof(PlayerMovePacket)) return;
			auto* pkt = reinterpret_cast<const PlayerMovePacket*>(data);
			RemotePlayer::ApplyNetworkMove(pkt->x, pkt->y, pkt->direction, pkt->moving != 0);
			break;
		}
		case PacketType::PlayerJoin: {
			if (size < sizeof(PlayerJoinPacket)) return;
			auto* pkt = reinterpret_cast<const PlayerJoinPacket*>(data);
			bool firstJoin = !RemotePlayer::HasInstance();
			GameState::RemotePlayerCharacterIndex = pkt->characterIndex;
			RemotePlayer::SpawnForOnline(pkt->characterIndex);
			if (firstJoin) {
				NetworkSystem::SendJoin(GameState::LocalPlayerCharacterIndex);
			}
			break;
		}
		case PacketType::PlayerLeave: {
			RemotePlayer::DestroyRemote();
			break;
		}
		case PacketType::BattleAction: {
			if (size < sizeof(BattleActionPacket)) return;
			auto* pkt = reinterpret_cast<const BattleActionPacket*>(data);
			BattleSystem::SendBattleDamage(pkt->battlerNum, pkt->damage);
			break;
		}
		case PacketType::SceneChange: {
			if (size < sizeof(SceneChangePacket)) return;
			auto* pkt = reinterpret_cast<const SceneChangePacket*>(data);
			GameState::NextLoadScreen = pkt->loadLocation;
			Engine::LoadScene(pkt->mapName, 0.25f, 0.35f);
			break;
		}
		case PacketType::BattleStart: {
			if (size < sizeof(BattleStartPacket)) return;
			auto* pkt = reinterpret_cast<const BattleStartPacket*>(data);
			GameState::NextLoadMapName = Engine::CurrentSceneName();
			GameState::Battle::NextBattleGroup = pkt->battleGroup;
			GameState::Battle::IsHost = false;
			BattleTransitionSystem::TriggerTransition(pkt->battleScene);
			GameState::Battle::CurrentStepsWithoutBattle = 0;
			break;
		}
		case PacketType::BattleUIState: {
			if (size < sizeof(BattleUIStatePacket)) return;
			auto* pkt = reinterpret_cast<const BattleUIStatePacket*>(data);
			BattleSystem::ApplyRemoteUIState(pkt->battlerState, pkt->menuCursor, pkt->magicMenuRow, pkt->magicMenuCol, pkt->targetIndex, pkt->targetingFriendly, pkt->selectedAbilityID);
			break;
		}
		case PacketType::BattleEnemyAction: {
			if (size < sizeof(BattleEnemyActionPacket)) return;
			auto* pkt = reinterpret_cast<const BattleEnemyActionPacket*>(data);
			BattleSystem::ApplyEnemyAction(pkt->enemySlot, pkt->abilityID, pkt->targetSlot);
			break;
		}
		default:
			break;
	}
}

void processBuffer() {
	while (_recvBufLen > 0) {
		auto type = static_cast<PacketType>(_recvBuf[0]);
		size_t needed = packetSizeForType(type);
		if (needed == 0) {
			// Unknown packet type — discard one byte and try again
			_recvBufLen--;
			memmove(_recvBuf, _recvBuf + 1, _recvBufLen);
			continue;
		}
		if (_recvBufLen < needed) break;
		dispatchPacket(_recvBuf, needed);
		_recvBufLen -= needed;
		memmove(_recvBuf, _recvBuf + needed, _recvBufLen);
	}
}

void onReceive(const void* data, size_t size, void* /*ctx*/) {
	size_t space = sizeof(_recvBuf) - _recvBufLen;
	size_t toCopy = size < space ? size : space;
	memcpy(_recvBuf + _recvBufLen, data, toCopy);
	_recvBufLen += toCopy;
	processBuffer();
}

}  // namespace

void NetworkSystem::Start() {
	_client = nullptr;
	_connected = false;
	_connecting = false;
	_failed = false;
	_recvBufLen = 0;
}

void NetworkSystem::Connect() {
	if (_client) return;
	_client = NetClientCreate();
	if (!_client) {
		_failed = true;
		return;
	}
	NetClientConnect(_client, "127.0.0.1", 7777);
	_connecting = true;
	_connected = false;
	_failed = false;
}

void NetworkSystem::Update() {
	if (!_client) return;
	NetClientUpdate(_client);
	if (_connecting) {
		if (_client->ConnectionStatus == NET_CONNECTED) {
			_connecting = false;
			_connected = true;
			sgLogInfo("NetworkSystem: connected to server");
		} else if (_client->ConnectionStatus == NET_CONNECTION_FAILED) {
			_connecting = false;
			_failed = true;
			sgLogWarn("NetworkSystem: connection failed");
		}
	}
	if (_connected) {
		NetClientReceive(_client, onReceive, nullptr);
	}
}

void NetworkSystem::Shutdown() {
	if (_client) {
		NetClientDestroy(_client);
		_client = nullptr;
	}
	_connected = false;
	_connecting = false;
	_failed = false;
	_recvBufLen = 0;
}

bool NetworkSystem::IsConnected() { return _connected; }
bool NetworkSystem::IsConnecting() { return _connecting; }
bool NetworkSystem::ConnectionFailed() { return _failed; }

void NetworkSystem::SendMove(float x, float y, uint8_t direction, bool moving) {
	if (!_connected || !_client) return;
	PlayerMovePacket pkt{};
	pkt.header.type = PacketType::PlayerMove;
	pkt.x = x;
	pkt.y = y;
	pkt.direction = direction;
	pkt.moving = moving ? 1 : 0;
	NetClientSend(_client, &pkt, sizeof(pkt));
}

void NetworkSystem::SendInteract(float x, float y) {
	if (!_connected || !_client) return;
	PlayerInteractPacket pkt{};
	pkt.header.type = PacketType::PlayerInteract;
	pkt.x = x;
	pkt.y = y;
	NetClientSend(_client, &pkt, sizeof(pkt));
}

void NetworkSystem::SendBattleAction(uint8_t battlerNum, int16_t damage) {
	if (!_connected || !_client) return;
	BattleActionPacket pkt{};
	pkt.header.type = PacketType::BattleAction;
	pkt.battlerNum = battlerNum;
	pkt.damage = damage;
	NetClientSend(_client, &pkt, sizeof(pkt));
}

void NetworkSystem::SendJoin(uint8_t characterIndex) {
	if (!_connected || !_client) return;
	PlayerJoinPacket pkt{};
	pkt.header.type = PacketType::PlayerJoin;
	pkt.playerSlot = 1;
	pkt.characterIndex = characterIndex;
	NetClientSend(_client, &pkt, sizeof(pkt));
}

void NetworkSystem::SendSceneChange(const char* mapName, uint8_t loadLocation) {
	if (!_connected || !_client) return;
	SceneChangePacket pkt{};
	pkt.header.type = PacketType::SceneChange;
	pkt.loadLocation = loadLocation;
	strncpy(pkt.mapName, mapName, sizeof(pkt.mapName) - 1);
	pkt.mapName[sizeof(pkt.mapName) - 1] = '\0';
	NetClientSend(_client, &pkt, sizeof(pkt));
}

void NetworkSystem::SendBattleStart(uint8_t battleGroup, const char* battleScene) {
	if (!_connected || !_client) return;
	BattleStartPacket pkt{};
	pkt.header.type = PacketType::BattleStart;
	pkt.battleGroup = battleGroup;
	strncpy(pkt.battleScene, battleScene, sizeof(pkt.battleScene) - 1);
	pkt.battleScene[sizeof(pkt.battleScene) - 1] = '\0';
	NetClientSend(_client, &pkt, sizeof(pkt));
}

void NetworkSystem::SendBattleUIState(uint8_t battlerState, uint8_t menuCursor, uint8_t magicRow, uint8_t magicCol, uint8_t targetIndex, uint8_t targetingFriendly, uint8_t selectedAbilityID) {
	if (!_connected || !_client) return;
	BattleUIStatePacket pkt{};
	pkt.header.type = PacketType::BattleUIState;
	pkt.battlerState = battlerState;
	pkt.menuCursor = menuCursor;
	pkt.magicMenuRow = magicRow;
	pkt.magicMenuCol = magicCol;
	pkt.targetIndex = targetIndex;
	pkt.targetingFriendly = targetingFriendly;
	pkt.selectedAbilityID = selectedAbilityID;
	NetClientSend(_client, &pkt, sizeof(pkt));
}

void NetworkSystem::SendBattleEnemyAction(uint8_t enemySlot, uint8_t abilityID, uint8_t targetSlot) {
	if (!_connected || !_client) return;
	BattleEnemyActionPacket pkt{};
	pkt.header.type = PacketType::BattleEnemyAction;
	pkt.enemySlot = enemySlot;
	pkt.abilityID = abilityID;
	pkt.targetSlot = targetSlot;
	NetClientSend(_client, &pkt, sizeof(pkt));
}
