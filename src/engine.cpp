#include <Supergoon/Animation/animator.h>
#include <Supergoon/Audio/Audio.h>
#include <Supergoon/Graphics/graphics.h>
#include <Supergoon/Graphics/shader.h>
#include <Supergoon/Graphics/texture.h>
#include <Supergoon/Tweening/easing.h>
#include <Supergoon/camera.h>
#include <Supergoon/engine.h>
#include <Supergoon/filesystem.h>
#include <Supergoon/json.h>
#include <Supergoon/map.h>
#include <Supergoon/sprite.h>
#include <Supergoon/state.h>
#include <Supergoon/text.h>
#include <Supergoon/window.h>
#include <sgforge/directory.h>
#include <sgforge/unpack.h>
#include <sgtools/log.h>
#include <stddef.h>

#include <algorithm>
#include <engine.hpp>
#include <format>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <gameobject/GameObject.hpp>
#include <string>
#include <systems/GameObjectSystem.hpp>
#include <systems/SystemCallbacks.hpp>
#include <systems/battleSystem.hpp>
#include <systems/dialogSystem.hpp>
#include <ui/ui.hpp>

#ifdef imgui
#include <SDL3/SDL_events.h>
#include <Supergoon/Platform/sdl/sdlWindow.h>
#include <imconfig.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_sdl3.h>

#include <debug/DebugCamera.hpp>
#include <debug/DebugPlayers.hpp>
#include <debug/DebugUI.hpp>
#include <debug/DebugWindow.hpp>
#endif

using namespace Etf;
using namespace std;

CurrentSceneLoadingState currentLoadingState_ = CurrentSceneLoadingState::NotLoading;

namespace {
void startEngine();
void draw();
int handleEngineEvents(void* event);
void shutdown();
void initializeEngine(const std::string& configFilename, void (*initializeFunc)(void));
// Fades out the full screen FBO if we aren't already fading
void startFullScreenFade(float time, LoadingScreenFadeTypes fadeType);
// if screen is fading, updates the time on it and tweens the fade.
void updateScreenFade();
// Returns if we are currently loading a scene
bool handleMapLoad();
void loadAllMaps();
void update();
void endScreenFade();
void loadSceneInternal();

struct SceneData {
	string CurrentScene = "";
	string NextScene = "";
	float FadeOutTime = 0;
	float FadeInTime = 0;
	bool PlayTransitionSFX = false;
	Scene* SceneToLoad = nullptr;

} sceneData_;

struct FadeData {
	Color LastFadeColor = {255, 255, 255, 255};
	Color CurrentFadeColor = {255, 255, 255, 255};
	unsigned int EndFadeAlpha = 255;
	float FadeTime = 1.0f;
	float CurrentFadeTime = 0;
	LoadingScreenFadeTypes CurrentFadeStatus = LoadingScreenFadeTypes::NotFading;
} fadeData_;
Directory* directory_ = nullptr;
vector<SystemCallbacks> systems_;
// non-statics for debugui showing
string currentBGM_ = "";

void initializeEngine(const std::string& configFilename, void (*initializefunc)(void)) {
	sgSetLogLevel(sgLogLevelWarn);
	SetInitializeFunction(initializefunc);
	SetStartFunction(startEngine);
	SetUpdateFunction(update);
	SetHandleEventFunction(handleEngineEvents);
	SetDrawFunction(draw);
	SetDrawUIFunction(UI::DrawUI);
	SetQuitFunction(shutdown);
	auto filePath = GetBasePath();
	auto fullFile = string(filePath) + "data/etf.sg";
	directory_ = LoadDirectoryFromFile(fullFile.c_str());
	AssetDirectory = directory_;
	ShaderSetDirectory(directory_);
	GameConfig::LoadGameConfig("./assets/config/gameConfig.json");
	auto& gameConfig = GameConfig::GetGameConfig();
	sgSetLogLevel(gameConfig.debug.debugLevel);
	SetWindowOptions(gameConfig.window.xWin, gameConfig.window.yWin, gameConfig.window.title.c_str());
	Engine::Audio::SetGlobalBGMVolume(gameConfig.audio.bgmVolume);
}

void startEngine() {
	auto& gameConfig = GameConfig::GetGameConfig();
	// preload all textures
	for (const auto& tex : gameConfig.PreloadTextures) {
		sgLogDebug("creating texture %s", tex.c_str());
		auto fullTex = tex + ".png";
		Texture* texture = TextureCreate(fullTex.c_str());
		Engine::Textures::LoadTextureFromBuffer(texture, tex);
	}
	loadAllMaps();
	GraphicsSetLogicalWorldSize(gameConfig.window.x, gameConfig.window.y);
	for (auto& system : systems_) {
		if (system.Start) system.Start();
	}
	Engine::LoadScene("", 0.1f, 1.75, false);
	Engine::DebugUI::Start();
}

void update() {
	GameState::DeltaTimeSeconds = DeltaTimeSeconds;
	GameState::DeltaTimeMilliseconds = DeltaTimeMilliseconds;
	if (!handleMapLoad()) return;
	for (auto& system : systems_) {
		system.Update();
	}
}
void draw() {
	for (auto& system : systems_) {
		if (system.Draw) system.Draw();
	}
	Engine::DebugUI::Draw();
}

int handleEngineEvents(void* event) {
	Engine::DebugUI::HandleEvent(event);
	return false;
}

void shutdown() {
	for (auto& system : systems_) {
		if (system.Shutdown) system.Shutdown();
	}
	if (directory_) {
		sgFreeDirectory(directory_);
	}
#ifdef imgui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
#endif
}

void loadSetupAndBgm() {
	sgLogDebug("Starting load setup/bgm");
	IsGameLoading = true;
	auto& gameSceneConfig = GameConfig::GetGameConfig().scene;
	const auto it = std::find_if(gameSceneConfig.scenes.begin(), gameSceneConfig.scenes.end(), [](Scene& scene) {
		return scene.MapName == sceneData_.NextScene;
	});
	if (it == gameSceneConfig.scenes.end()) {
		sgLogError("Could not find scene with name %s, not loading", sceneData_.NextScene.c_str());
		currentLoadingState_ = CurrentSceneLoadingState::NotLoading;
		return;
	}
	sceneData_.SceneToLoad = &*it;
	auto& sceneToLoad = *it;
	// We should destroy all of the old gameobjects, and also load the ui if needed.
	ResetCameraFollow();
	Engine::Audio::PlayBGM(sceneToLoad.BGMName, sceneToLoad.BGMVolume);
}

void loadUI() {
	sgLogDebug("Starting load ui");
	if (!sceneData_.SceneToLoad->UIName.empty()) {
		UI::LoadUIFromFile(sceneData_.SceneToLoad->UIName);
	} else {
		UI::GetRootUIObject()->DestroyChildIfNotName("");
	}
}

void loadDialog() {
	sgLogDebug("Starting load dialog");
	DialogSystem::LoadDialogFromJsonFile(sceneData_.SceneToLoad->MapName);
}

void loadEnd() {
	sgLogDebug("Starting load end");
	sceneData_.CurrentScene = sceneData_.NextScene;
	sceneData_.NextScene = "";
	GameState::NextLoadMapName = "";
	GameState::Battle::ExitingFromBattle = false;
}

bool handleMapLoad() {
	// Handle screen fading as needed.

	updateScreenFade();
	switch (currentLoadingState_) {
		// If we are not loading, check to see if we should trigger it
		case CurrentSceneLoadingState::NotLoading:
			return true;
		case Etf::CurrentSceneLoadingState::NextSceneQueued:
			if (sceneData_.PlayTransitionSFX) Engine::Audio::PlaySFXBuffer("transition2", 0.5f);
			startFullScreenFade(sceneData_.FadeOutTime, LoadingScreenFadeTypes::FadeOut);
			currentLoadingState_ = CurrentSceneLoadingState::WaitingForFadeOut;
			return false;
		case CurrentSceneLoadingState::WaitingForFadeOut:
			if (fadeData_.CurrentFadeStatus != LoadingScreenFadeTypes::NotFading) return false;
			currentLoadingState_ = CurrentSceneLoadingState::LoadingStart;
			return false;
		case Etf::CurrentSceneLoadingState::LoadingStart:
			loadSetupAndBgm();
			currentLoadingState_ = CurrentSceneLoadingState::LoadingMap;
			return false;
		case Etf::CurrentSceneLoadingState::LoadingMap:
			loadSceneInternal();
			currentLoadingState_ = CurrentSceneLoadingState::LoadingGameObjects;
			return false;
		case Etf::CurrentSceneLoadingState::LoadingGameObjects:
			sgLogDebug("Starting load gameobjects");
			GameObjectSystem::Load();
			currentLoadingState_ = CurrentSceneLoadingState::LoadingUI;
			return false;
		case Etf::CurrentSceneLoadingState::LoadingUI:
			loadUI();
			currentLoadingState_ = CurrentSceneLoadingState::LoadingDialog;
			return false;
		case Etf::CurrentSceneLoadingState::LoadingDialog:
			loadDialog();
			currentLoadingState_ = CurrentSceneLoadingState::LoadingFinish;
			return false;
		case Etf::CurrentSceneLoadingState::LoadingFinish:
			loadEnd();
			currentLoadingState_ = CurrentSceneLoadingState::JustLoaded;
			return false;
		case Etf::CurrentSceneLoadingState::JustLoaded:
			startFullScreenFade(sceneData_.FadeInTime, LoadingScreenFadeTypes::FadeIn);
			currentLoadingState_ = CurrentSceneLoadingState::FadingIn;
			return false;
		// After 50% of current time is done, we should allow updates from the gameobjects.
		case CurrentSceneLoadingState::FadingIn:
			sgLogDebug("Starting fading in");
			if (!fadeData_.FadeTime) {
				currentLoadingState_ = CurrentSceneLoadingState::NotLoading;
				endScreenFade();
				return true;
			}
			if (!fadeData_.FadeTime || fadeData_.FadeTime / fadeData_.CurrentFadeTime >= 0.9f) {
				currentLoadingState_ = CurrentSceneLoadingState::FadingInAllowUpdate;
			}
			return false;
		case CurrentSceneLoadingState::FadingInAllowUpdate:
			// sgLogDebug("Starting fading in allow update");
			if (fadeData_.CurrentFadeTime >= fadeData_.FadeTime) {
				IsGameLoading = false;
				currentLoadingState_ = CurrentSceneLoadingState::NotLoading;
				endScreenFade();
			}
			return true;
		case Etf::CurrentSceneLoadingState::Count:
			return true;
	}
	return false;
}

void startFullScreenFade(float time, LoadingScreenFadeTypes fadeType) {
	if (fadeType == LoadingScreenFadeTypes::NotFading || fadeData_.CurrentFadeStatus != LoadingScreenFadeTypes::NotFading) {
		sgLogDebug("Cannot fade: request: %d, status: %d", fadeType, fadeData_.CurrentFadeStatus);
		return;
	}
	fadeData_.LastFadeColor = GraphicsGetFBOColor();
	fadeData_.CurrentFadeTime = 0;
	fadeData_.FadeTime = time;
	fadeData_.CurrentFadeColor = fadeData_.LastFadeColor;
	fadeData_.CurrentFadeStatus = fadeType;
	fadeData_.EndFadeAlpha = fadeType == LoadingScreenFadeTypes::FadeIn ? 255 : 0;
	GameState::CurrentFadeState = (int)fadeType;
}

void updateScreenFade() {
	if (fadeData_.CurrentFadeStatus == LoadingScreenFadeTypes::NotFading) return;
	fadeData_.CurrentFadeTime += GameState::DeltaTimeSeconds;
	// sgLogDebug("Fade time is %f of %f", _fadeData.CurrentFadeTime, _fadeData.FadeTime);
	// sgLogDebug("Screen is fading currently");
	if (fadeData_.CurrentFadeTime >= fadeData_.FadeTime) {
		fadeData_.CurrentFadeStatus = LoadingScreenFadeTypes::NotFading;
		GameState::CurrentFadeState = (int)fadeData_.CurrentFadeStatus;
		return;
	}
	fadeData_.CurrentFadeColor.A = Engine::Tweening::GetTweenedValue(fadeData_.LastFadeColor.A, fadeData_.EndFadeAlpha, fadeData_.CurrentFadeTime, fadeData_.FadeTime);
	GraphicsUpdateFBOColor(&fadeData_.CurrentFadeColor);
}

void loadAllMaps() {
	auto& config = GameConfig::GetGameConfig();
	char* buf;
	size_t sz;
	for (auto& scene : config.scene.scenes) {
		sceneData_.SceneToLoad = &scene;
		auto name = scene.MapName + ".tmj";
		auto result = GetDataFromDirectory(name.c_str(), &buf, &sz, directory_);
		if(!result){
			sgLogDebug("Could not preload map file %s", name.c_str());
			continue;
		}
		CacheMapFromBuffer(scene.MapName.c_str(), buf, sz);
		loadUI();
		loadDialog();
	}
	BattleSystem::InitializeBattleSystem();
	// loadEnd();
	// Load all textures
	ResetCameraFollow();
}

void endScreenFade() {
	fadeData_.CurrentFadeColor.A = fadeData_.EndFadeAlpha;
	GraphicsUpdateFBOColor(&fadeData_.CurrentFadeColor);
}

Sprite* createSpriteInternal(const std::string& name, float* followX, float* followY, RectangleF sourceRect, RectangleF offsetSizeRect, bool manual) {
	auto func = manual ? NewSpriteManual : NewSprite;
	auto sprite = func();
	sprite->parentX = followX;
	sprite->parentY = followY;
	sprite->prevParentX = followX ? *followX : 0;
	sprite->prevParentY = followY ? *followY : 0;
	sprite->Flags |= SpriteFlagVisible;
	sprite->Texture = TextureCreate(name.c_str());
	char* buf;
	size_t sz;
	auto result = GetDataFromDirectory(name.c_str(), &buf, &sz, directory_);
	if (result) {
		TextureLoadFromPngBuffer(sprite->Texture, name.c_str(), buf, sz);
	} else {
		sgLogDebug("Could not find!");
	}

	sprite->Shader = GetDefaultShader();
	sprite->TextureSourceRect = sourceRect;
	sprite->OffsetAndSizeRectF = offsetSizeRect;
	return sprite;
}

void loadSceneInternal() {
	sgLogDebug("Starting load map");
	char* buf;
	size_t sz;
	string nextMapName = sceneData_.NextScene + ".tmj";
	auto result = GetDataFromDirectory(nextMapName.c_str(), &buf, &sz, directory_);
	if (!result) {
		sgLogCritical("Could not load map %s", nextMapName.c_str());
	}
	LoadMapFromBuffer(sceneData_.NextScene.c_str(), buf, sz);
}

void playBGMInternal(const string& name, float volume) {
	auto fullPath = std::format("{}.ogg", name);
	char* buf;
	size_t sz;
	GetDataFromDirectory(fullPath.c_str(), &buf, &sz, directory_);
	LoadBgmBuffer(fullPath.c_str(), volume, -1, buf, sz);
	PlayBgm();
}
}  // namespace

void Engine::RegisterSystems(const std::vector<SystemCallbacks>& systems) {
	for (auto& system : systems) {
		systems_.push_back(system);
	}
}

const std::string& Engine::CurrentSceneName() {
	return sceneData_.CurrentScene;
}

void Engine::Debug::DrawRectPrimitive(const RectangleF& rect, Color color, bool filled, bool cameraOffset) {
	DrawRect(&rect, &color, filled, cameraOffset);
}

void Engine::Textures::LoadTextureFromBuffer(Texture* tex, const std::string& name) {
	char* buf;
	size_t sz;
	string filename = format("{}.png", name);
	auto result = GetDataFromDirectory(filename.c_str(), &buf, &sz, directory_);
	if (result) {
		TextureLoadFromPngBuffer(tex, name.c_str(), buf, sz);
	} else {
		sgLogDebug("Could not find!");
	}
}

Sprite* Engine::Sprites::CreateSpriteFull(const std::string& name, float* followX, float* followY, RectangleF sourceRect, RectangleF offsetSizeRect) {
	auto sprite = createSpriteInternal(name, followX, followY, sourceRect, offsetSizeRect, false);
	return sprite;
}

// TODO this should be refactored instead of copy/paste from createspritefull
Sprite* Engine::Sprites::CreateManualSpriteFull(const std::string& name, float* followX, float* followY, RectangleF sourceRect, RectangleF offsetSizeRect) {
	auto sprite = createSpriteInternal(name, followX, followY, sourceRect, offsetSizeRect, true);
	return sprite;
}

void Engine::Sprites::SetSpriteVisible(Sprite* sprite, bool visible) {
	if (visible) {
		sprite->Flags |= SpriteFlagVisible;
	} else {
		sprite->Flags &= ~SpriteFlagVisible;
	}
}

void Engine::LoadScene(const string& name, float fadeOutTime, float fadeInTime, bool playTransitionSound) {
	if (currentLoadingState_ != CurrentSceneLoadingState::NotLoading) return;
	auto newName = name;
	auto& gameSceneConfig = GameConfig::GetGameConfig().scene;
	if (newName.empty()) {
		newName = gameSceneConfig.defaultScene;
	}
	sgLogDebug("Loading scene with name %s", newName.c_str());
	currentLoadingState_ = CurrentSceneLoadingState::NextSceneQueued;
	sceneData_.PlayTransitionSFX = playTransitionSound;
	sceneData_.FadeOutTime = fadeOutTime;
	sceneData_.FadeInTime = fadeInTime;
	sceneData_.NextScene = newName;
}

void Engine::Audio::PlayBGM(const std::string& name, float volume) {
	if (currentBGM_ == name) {
		sgLogDebug("Backing out, not playing bgm %s", name.c_str());
		return;
	}
	sgLogDebug("Playin, bgm %s", name.c_str());
	SetBgmTrack(0);
	playBGMInternal(name, volume);
	currentBGM_ = name;
}

void Engine::Audio::PlayBGMBackground(const std::string& name, float volume) {
	SetBgmTrack(1);
	playBGMInternal(name, volume);
}

void Engine::Audio::PlaySFXBuffer(const string& name, float volume) {
	sgLogDebug("Playing sfx %s", name.c_str());
	auto fullPath = std::format("{}.ogg", name);
	char* buf;
	size_t sz;
	GetDataFromDirectory(fullPath.c_str(), &buf, &sz, directory_);
	PlaySfxOneShot(fullPath.c_str(), volume, buf, sz);
}

void Engine::Audio::SetGlobalBGMVolume(float volume) {
	SetGlobalBgmVolume(volume);
}

void Engine::Audio::StopBGMBackground() {
	SetBgmTrack(1);
	StopBgm();
}

float Engine::Tweening::GetTweenedValue(float start, float end, float timeSeconds, float totalSeconds, TweenEaseTypes ease) {
	if (totalSeconds <= 0.0f) return end;
	auto easeFunc = geLinearInterpolation;
	switch (ease) {
		case TweenEaseTypes::Linear:
			break;
		case TweenEaseTypes::QuintOut:
			easeFunc = geQuinticEaseOut;
			break;
		default:
			sgLogDebug("Ease type not implemented, using linear");
			break;
	}
	double time = static_cast<double>(timeSeconds / totalSeconds);
	double progress = easeFunc(time);
	double startD = static_cast<double>(start);
	double endD = static_cast<double>(end);
	double value = startD + (endD - startD) * progress;
	if (startD < endD) {
		value = std::min(value, endD);
	} else {
		value = std::max(value, endD);
	}
	return static_cast<float>(value);
}

Text* Engine::TextUtils::CreateText(const std::string& fontName, unsigned int fontSize, RectangleF location, const std::string& text, unsigned int numChars, bool centeredX, bool centeredY) {
	TextSetFont(fontName.c_str(), fontSize, directory_);
	auto textPtr = TextCreate(&location, text.c_str());
	textPtr->NumLettersToDraw = numChars;
	textPtr->CenteredX = centeredX;
	textPtr->CenteredY = centeredY;
	textPtr->WordWrap = true;
	TextLoad(textPtr);
	TextOnDirty(textPtr);
	return textPtr;
}

void Engine::TextUtils::DrawText(Text* text, float xOffset, float yOffset, Color& color) {
	TextDraw(text, xOffset, yOffset, &color);
}

void Engine::Json::GetJsonBufferFromDirectory(const char* name, char** buf, size_t* sz) {
	auto fullPath = std::format("{}.json", name);
	auto result = GetDataFromDirectory(fullPath.c_str(), buf, sz, directory_);
	if (!result) {
		*buf = NULL;
		*sz = 0;
		sgLogDebug("Could not load json for %s", name);
	}
}

json_object* Engine::Json::GetJsonObjectFromDirectory(const char* name) {
	char* buf;
	size_t sz;
	GetJsonBufferFromDirectory(name, &buf, &sz);
	return jGetObjectFromBuffer(buf, sz);
}

RectangleF Engine::Json::GetRectFromObject(void* object, const std::string& key) {
	auto obj = static_cast<json_object*>(object);
	auto rectJson = jobj(obj, key.c_str());
	if (!rectJson) return {0, 0, 0, 0};
	return {
		jfloat(rectJson, "x"),
		jfloat(rectJson, "y"),
		jfloat(rectJson, "w"),
		jfloat(rectJson, "h"),
	};
}

#ifdef imgui
void Engine::DebugUI::Start() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsClassic();
	ImGui_ImplSDL3_InitForOpenGL((SDL_Window*)WindowGet()->Handle, GraphicsGetContextPtr());
	ImGui_ImplOpenGL3_Init();
	DebugWindow::Initialize();
}

void Engine::DebugUI::HandleEvent(void* event) {
	auto sdlEvent = static_cast<SDL_Event*>(event);
	ImGui_ImplSDL3_ProcessEvent(sdlEvent);
}
void Engine::DebugUI::Draw() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();
	DebugWindow::DrawWindows();
}

void Engine::DebugUI::Render() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Engine::DebugUI::AddTab(std::function<void()> func) {
	DebugWindow::AddTabFuncToMainDebugWindow(func);
}
void Engine::DebugUI::AddTab(const std::vector<std::function<void()>>& funcs) {
	for (auto& func : funcs) {
		DebugWindow::AddTabFuncToMainDebugWindow(func);
	}
}

void Engine::DebugUI::AddWindow(const std::vector<std::pair<const std::string&, std::function<void()>>>& funcs) {
	for (auto& [name, func] : funcs) {
		DebugWindow::AddWindowFunc(name, func);
	}
}
#else
void Engine::DebugUI::Start() {}
void Engine::DebugUI::HandleEvent(void* event) {}
void Engine::DebugUI::Draw() {}
void Engine::DebugUI::Render() {}
void Engine::DebugUI::AddTab(std::function<void()> func) {}
void Engine::DebugUI::AddTab(const std::vector<std::function<void()>>& funcs) {}
void Engine::DebugUI::AddWindow(const std::vector<std::pair<const std::string&, std::function<void()>>>& funcs) {}
#endif

// Enable C engine
extern "C" {
void InitializeEngineFunctions() {
	initializeEngine("gameConfig.json", InitializeGame);
}
void (*GraphicsPostFBODrawDebugFunc)(void) = Etf::Engine::DebugUI::Render;
}
