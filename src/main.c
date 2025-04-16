#include <SDL3/SDL_main.h>
#include <Supergoon/Input/keyboard.h>
#include <Supergoon/UI/ui.h>
#include <Supergoon/UI/uiimage.h>
#include <Supergoon/UI/uilayoutgroup.h>
#include <Supergoon/UI/uiobject.h>
#include <Supergoon/UI/uirect.h>
#include <Supergoon/UI/uitext.h>
#include <Supergoon/engine.h>
#include <Supergoon/gameobject.h>
#include <Supergoon/graphics.h>
#include <Supergoon/log.h>
#include <Supergoon/lua.h>
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
	LuaRunFile("assets/lua/test.lua");
	// UIObject* testImage = SDL_calloc(1, sizeof(*testObj));
	// testImage->XOffset = testImage->YOffset = testImage->Location.w = testImage->Location.h = 64;
	// testImage->Name = strdup("Test image");
	// testImage->Flags |= UIObjectFlagActive | UIObjectFlagVisible | UIObjectFlagDirty;
	// testImage->Type = UIObjectTypesImage;
	// UIImageData* imageData = SDL_calloc(1, sizeof(*imageData));
	// imageData->Texture = CreateTextureFromIndexedBMP("uibase-export");
	// testImage->Data = imageData;
	// UIObject* testText = SDL_calloc(1, sizeof(*testText));
	// testText->XOffset = testText->YOffset = 48;
	// // testText->Location.w = 240;
	// testText->Location.w = 260;
	// testText->Location.h = 120;
	// testText->Name = strdup("Test Text");
	// testText->Flags |= UIObjectFlagActive | UIObjectFlagVisible | UIObjectFlagDirty;
	// testText->Type = UIObjectTypesText;
	// SetFont("PressStart2P", 16);
	// UIText* textData = SDL_calloc(1, sizeof(*textData));
	// textData->CenteredX = true;
	// textData->CenteredY = true;
	// testText->Data = textData;
	// textData->FontSize = 16;
	// textData->NumLettersToDraw = 500;
	// textData->Text = "Welcome to the game! Hello above hi";
	// textData->WordWrap = true;
	// UIObject* testRect = SDL_calloc(1, sizeof(*testRect));
	// testRect->XOffset = testRect->YOffset = 48;
	// testRect->Location.w = 260;
	// testRect->Location.h = 120;
	// testRect->Name = strdup("Test Rect");
	// testRect->Flags |= UIObjectFlagActive | UIObjectFlagVisible | UIObjectFlagDirty;
	// testRect->Type = UIObjectTypesRect;
	// UIRect* rectData = SDL_calloc(1, sizeof(*rectData));
	// rectData->Color = (sgColor){0, 0, 0, 0};
	// testRect->Data = rectData;
	// // hlg
	// UIObject* testHlg = SDL_calloc(1, sizeof(*testHlg));
	// testHlg->XOffset = testRect->YOffset = 48;
	// testHlg->Location.w = 260;
	// testHlg->Location.h = 120;
	// testHlg->Name = strdup("Test HLG");
	// testHlg->Flags |= UIObjectFlagActive | UIObjectFlagVisible | UIObjectFlagDirty;
	// testHlg->Type = UIObjectTypesLayoutGroup;
	// UILayoutGroup* layoutGroupData = SDL_calloc(1, sizeof(*layoutGroupData));
	// testHlg->Data = layoutGroupData;
	// layoutGroupData->IsHorizontal = true;
	// layoutGroupData->Spacing = 5;

	// AddUIObject(testObj, NULL);
	// AddUIObject(testImage, testObj);
	// AddUIObject(testText, testObj);
	// AddUIObject(testRect, testObj);
	// AddUIObject(testHlg, testObj);

	// for (size_t i = 0; i < 3; i++) {
	// 	UIObject* hlgTextOb = SDL_calloc(1, sizeof(*hlgTextOb));
	// 	hlgTextOb->XOffset = testText->YOffset = 0;
	// 	// testText->Location.w = 240;
	// 	hlgTextOb->Location.w = 32;
	// 	hlgTextOb->Location.h = 32;
	// 	const char* hello[3] = {"New Game", "Continue", "Option"};
	// 	hlgTextOb->Name = strdup(hello[i]);
	// 	hlgTextOb->Flags |= UIObjectFlagActive | UIObjectFlagVisible | UIObjectFlagDirty;
	// 	hlgTextOb->Type = UIObjectTypesText;
	// 	UIText* textDataman = SDL_calloc(1, sizeof(*textData));
	// 	// textData->CenteredX = true;
	// 	// textData->CenteredY = true;
	// 	hlgTextOb->Data = textDataman;
	// 	textDataman->FontSize = 16;
	// 	textDataman->NumLettersToDraw = 500;
	// 	textDataman->Text = hello[i];
	// 	// textData->WordWrap = true;
	// 	AddUIObject(hlgTextOb, testHlg);
	// }
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
