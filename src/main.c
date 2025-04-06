#include <SDL3/SDL_main.h>
#include <Supergoon/Input/keyboard.h>
#include <Supergoon/UI/ui.h>
#include <Supergoon/UI/uiimage.h>
#include <Supergoon/UI/uiobject.h>
#include <Supergoon/UI/uitext.h>
#include <Supergoon/engine.h>
#include <Supergoon/gameobject.h>
#include <Supergoon/graphics.h>
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
	UIObject* testImage = SDL_calloc(1, sizeof(*testObj));
	testImage->XOffset = testImage->YOffset = testImage->Location.w = testImage->Location.h = 64;
	testImage->Name = strdup("Test image");
	testImage->Flags |= UIObjectFlagActive | UIObjectFlagVisible | UIObjectFlagDirty;
	testImage->Type = UIObjectTypesImage;
	UIImageData* imageData = SDL_calloc(1, sizeof(*imageData));
	imageData->Texture = CreateTextureFromIndexedBMP("supergoonGamesLogo");
	testImage->Data = imageData;
	UIObject* testText = SDL_calloc(1, sizeof(*testText));
	testText->XOffset = testText->YOffset = 48;
	// testText->Location.w = 240;
	testText->Location.w = 260;
	testText->Location.h = 120;
	testText->Name = strdup("Test Text");
	testText->Flags |= UIObjectFlagActive | UIObjectFlagVisible | UIObjectFlagDirty;
	testText->Type = UIObjectTypesText;
	SetFont("PressStart2P", 16);
	UIText* textData = SDL_calloc(1, sizeof(*textData));
	testText->Data = textData;
	textData->FontSize = 16;
	textData->NumLettersToDraw = 500;
	textData->Text = "Welcome to the game! Hello above hi";
	textData->WordWrap = true;
	AddUIObject(testObj, NULL);
	AddUIObject(testImage, testObj);
	AddUIObject(testText, testObj);
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
