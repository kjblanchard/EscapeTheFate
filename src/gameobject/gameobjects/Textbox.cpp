#include <Supergoon/log.h>
#include <Supergoon/map.h>

#include <bindings/engine.hpp>
#include <gameConfig.hpp>
#include <gameobject/gameobjects/Textbox.hpp>
#include <string_view>
#include <systems/GameObjectSystem.hpp>
#include <systems/dialogSystem.hpp>

using namespace Etf;
using namespace std;

void Textbox::Create(TiledObject* objData) {
	auto textbox = new Textbox(objData);
	GameObjectSystem::AddGameObject(textbox);
	GameObjectSystem::AddGameObjectOfType<IInteractable>(textbox);
}

Textbox::Textbox(TiledObject* objData) : GameObject(objData->X, objData->Y) {
	InteractionRect.x = X();
	InteractionRect.y = Y();
	InteractionRect.w = objData->Width;
	InteractionRect.h = objData->Height;
	for (int i = 0; i < objData->NumProperties; ++i) {
		TiledProperty* currentProp = &objData->Properties[i];
		if (string_view(currentProp->Name) != "name") continue;
		_textToWrite = currentProp->Data.StringData;
		break;
	}
}

Textbox::~Textbox() {
	GameObjectSystem::RemoveGameObjectOfType<IInteractable>(this);
}

void Textbox::Interact() {
	DialogSystem::TextBoxInteractionUpdate(this, _textToWrite);
}

void Textbox::Draw() {
	if (GameConfig::GetGameConfig().debug.interactions) Engine::DrawRectPrimitive(InteractionRect);
}
