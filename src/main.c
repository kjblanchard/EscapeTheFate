#include <SDL3/SDL_main.h>
#include <Supergoon/Input/keyboard.h>
#include <Supergoon/UI/ui.h>
#include <Supergoon/UI/uiobject.h>
#include <Supergoon/engine.h>
#include <Supergoon/gameobject.h>
#include <Supergoon/log.h>
#include <Supergoon/map.h>
#include <Supergoon/window.h>
#include <stdio.h>

static void CreatePlayer(void* userdata, GameObject* go) {
	TiledObject* mapData = (TiledObject*)userdata;
	if (!mapData) {
		return;
	}
	go->X = mapData->X;
	go->Y = mapData->Y;
	go->W = mapData->Width;
	go->H = mapData->Height;
}
static void StartPlayer(GameObject* go) {
	(void)go;
}
static void UpdatePlayer(GameObject* go) {
	(void)go;
	if (IsKeyboardKeyJustPressed(SDL_SCANCODE_W)) {
		sgLogWarn("W key is just pressed!");
	}
}

static void Start(void) {
	LoadMap("debugTown");
	LoadObjectsFromMap();
	UIObject* testObj = SDL_calloc(1, sizeof(*testObj));
	testObj->XOffset = testObj->YOffset = testObj->Location.w = testObj->Location.h = 32;
	testObj->Name = strdup("Test panel");
	testObj->Flags |= UIObjectFlagActive | UIObjectFlagVisible | UIObjectFlagDirty;
	AddUIObject(testObj, NULL);
}

static void Update(void) {
	(void)"Hello";
}

int main(int argc, char* argv[]) {
	(void)argc;
	(void)argv;
	// SetWindowOptions(480, 270, "Escape The Fate");
	SetWindowOptions(1920, 1080, "Escape The Fate");
	SetScalingOptions(480, 270);
	ObjectSetCreateFunction(1, CreatePlayer);
	ObjectSetStartFunction(1, StartPlayer);
	ObjectSetUpdateFunction(1, UpdatePlayer);
	SetStartFunction(Start);
	SetUpdateFunction(Update);
	Run();
}
