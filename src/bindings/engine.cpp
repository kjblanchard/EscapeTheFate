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

#include <algorithm>
#include <bindings/engine.hpp>
#include <filesystem>
#include <format>
#include <gameConfig.hpp>
#include <gameState.hpp>
#include <gameobject/GameObject.hpp>
#include <string>
#include <systems/GameObjectSystem.hpp>
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

#ifdef imgui
void Engine::DebugUI::Start() {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsClassic();
	ImGui_ImplSDL3_InitForOpenGL((SDL_Window*)WindowGet()->Handle, GraphicsGetContextPtr());
	ImGui_ImplOpenGL3_Init();
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
	DrawMainDebugWindow();
}

void Engine::DebugUI::Render() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Engine::DebugUI::AddTab(std::function<void()> func) {
	AddTabFuncToMainDebugWindow(func);
}
#else
void Engine::DebugUI::Start() {}
void Engine::DebugUI::HandleEvent(void* event) {}
void Engine::DebugUI::Draw() {}
void Engine::DebugUI::Render() {}
void Engine::DebugUI::AddTab(std::function<void()> func) {}
#endif

static Directory* sDirectory = nullptr;

static string _currentBGM = "";

static struct SceneData {
	string CurrentScene = "";
	string NextScene = "";
	float FadeOutTime = 0;
	float FadeInTime = 0;
	bool PlayTransitionSFX = false;
	Scene* SceneToLoad = nullptr;

} _sceneData;

static struct FadeData {
	Color LastFadeColor = {255, 255, 255, 255};
	Color CurrentFadeColor = {255, 255, 255, 255};
	unsigned int EndFadeAlpha = 255;
	float FadeTime = 1.0f;
	float CurrentFadeTime = 0;
	ScreenFadeTypes CurrentFadeStatus = ScreenFadeTypes::NotFading;
} _fadeData;

static CurrentSceneLoadingState _currentLoadingState = CurrentSceneLoadingState::NotLoading;

static void endScreenFade() {
	_fadeData.CurrentFadeColor.A = _fadeData.EndFadeAlpha;
	GraphicsUpdateFBOColor(&_fadeData.CurrentFadeColor);
}

void Engine::Audio::PlaySFX(const std::string& name, float volume) {
	// PlaySfxOneShot(name.c_str(), volume);
	sgLogWarn("Trying to play sfx %s", name.c_str());
	PlaySfxOneShotF(name.c_str(), volume);
}

void Engine::Audio::PlaySFXBuffer(const string& name, float volume) {
	auto fullPath = std::format("{}.ogg", name);
	char* buf;
	size_t sz;
	GetDataFromDirectory(fullPath.c_str(), &buf, &sz, sDirectory);
	PlaySfxOneShot(fullPath.c_str(), volume, buf, sz);
}

const std::string& Engine::CurrentScene() {
	return _sceneData.CurrentScene;
}

void Engine::InitializeEngine(EngineInitializeArgs args) {
	sgSetLogLevel(sgLogLevelWarn);
	SetInitializeFunction(args.InitializeFunc);
	SetStartFunction(args.StartFunc);
	SetUpdateFunction(args.UpdateFunc);
	SetHandleEventFunction(args.HandleEventFunc);
	SetDrawFunction(args.DrawFunc);
	SetDrawUIFunction(UI::DrawUI);
	SetQuitFunction(args.QuitFunc);
	auto filePath = GetBasePath();
	auto fullFile = string(filePath) + "data/etf.sg";
	sDirectory = LoadDirectoryFromFile(fullFile.c_str());
	AssetDirectory = sDirectory;
	ShaderSetDirectory(sDirectory);
	GameConfig::LoadGameConfig("./assets/config/gameConfig.json");
	auto& gameConfig = GameConfig::GetGameConfig();
	Engine::SetLogLevel(gameConfig.debug.debugLevel);
	Engine::SetupWindow(gameConfig.window.xWin, gameConfig.window.yWin, gameConfig.window.title);
	Engine::Audio::SetGlobalBGMVolume(gameConfig.audio.bgmVolume);
}

void Engine::StartEngine() {
	auto& gameConfig = GameConfig::GetGameConfig();
	GraphicsSetLogicalWorldSize(gameConfig.window.x, gameConfig.window.y);
}

void Engine::Update() {
	GameState::DeltaTimeSeconds = DeltaTimeSeconds;
	GameState::DeltaTimeMilliseconds = DeltaTimeMilliseconds;
}

void Engine::SetLogLevel(int logLevel) {
	sgSetLogLevel(logLevel);
}

void Engine::SetupWindow(int width, int height, std::string& windowName) {
	SetWindowOptions(width, height, windowName.c_str());
}

void Engine::Shutdown() {
	if (sDirectory) {
		sgFreeDirectory(sDirectory);
	}
#ifdef imgui
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL3_Shutdown();
	ImGui::DestroyContext();
#endif
}

static void loadSetupAndBgm() {
	sgLogDebug("Starting load setup/bgm");
	IsGameLoading = true;
	auto& gameSceneConfig = GameConfig::GetGameConfig().scene;
	const auto it = std::find_if(gameSceneConfig.scenes.begin(), gameSceneConfig.scenes.end(), [](Scene& scene) {
		return scene.MapName == _sceneData.NextScene;
	});
	if (it == gameSceneConfig.scenes.end()) {
		sgLogError("Could not find scene with name %s, not loading", _sceneData.NextScene.c_str());
		_currentLoadingState = CurrentSceneLoadingState::NotLoading;
		return;
	}
	_sceneData.SceneToLoad = &*it;
	auto& sceneToLoad = *it;
	// We should destroy all of the old gameobjects, and also load the ui if needed.
	ResetCameraFollow();
	if (_currentBGM != sceneToLoad.BGMName) {
		Engine::Audio::PlayBGM(sceneToLoad.BGMName, sceneToLoad.BGMVolume);
		_currentBGM = sceneToLoad.BGMName;
	}
}

static void loadUI() {
	sgLogDebug("Starting load ui");
	if (!_sceneData.SceneToLoad->UIName.empty()) {
		// UI::LoadUIFromFile(format("{}assets/ui/{}.json", GetBasePath(), _sceneData.SceneToLoad->UIName));
		UI::LoadUIFromFile(_sceneData.SceneToLoad->UIName);
	} else {
		UI::GetRootUIObject()->DestroyChildIfNotName("");
	}
}

static void loadDialog() {
	sgLogDebug("Starting load dialog");
	DialogSystem::LoadDialogFromJsonFile(_sceneData.SceneToLoad->MapName);
}

static void loadEnd() {
	sgLogDebug("Starting load end");
	_sceneData.CurrentScene = _sceneData.NextScene;
	_sceneData.NextScene = "";
	GameState::NextLoadMapName = "";
	GameState::Battle::ExitingFromBattle = false;
}

static void loadSceneInternal() {
	sgLogDebug("Starting load map");
	char* buf;
	size_t sz;
	string nextMapName = _sceneData.NextScene + ".tmj";
	auto result = GetDataFromDirectory(nextMapName.c_str(), &buf, &sz, sDirectory);
	if (!result) {
		sgLogCritical("Could not load map %s", nextMapName.c_str());
	}

	LoadMapFromBuffer(_sceneData.NextScene.c_str(), buf, sz);
}

void Engine::LoadScene(const string& name, float fadeOutTime, float fadeInTime, bool playTransitionSound) {
	if (_currentLoadingState != CurrentSceneLoadingState::NotLoading) return;
	sgLogDebug("Loading scene");
	auto newName = name;
	auto& gameSceneConfig = GameConfig::GetGameConfig().scene;
	if (newName.empty()) {
		newName = gameSceneConfig.defaultScene;
	}
	_currentLoadingState = CurrentSceneLoadingState::NextSceneQueued;
	_sceneData.PlayTransitionSFX = playTransitionSound;
	_sceneData.FadeOutTime = fadeOutTime;
	_sceneData.FadeInTime = fadeInTime;
	_sceneData.NextScene = newName;
}

bool Engine::HandleMapLoad() {
	// Handle screen fading as needed.
	Engine::UpdateScreenFade();
	switch (_currentLoadingState) {
		// If we are not loading, check to see if we should trigger it
		case CurrentSceneLoadingState::NotLoading:
			return true;
		case Etf::CurrentSceneLoadingState::NextSceneQueued:
			if (_sceneData.PlayTransitionSFX) Engine::Audio::PlaySFXBuffer("transition2", 0.5f);
			StartFullScreenFade(_sceneData.FadeOutTime, ScreenFadeTypes::FadeOut);
			_currentLoadingState = CurrentSceneLoadingState::WaitingForFadeOut;
			return false;
		case CurrentSceneLoadingState::WaitingForFadeOut:
			if (_fadeData.CurrentFadeStatus != ScreenFadeTypes::NotFading) return false;
			_currentLoadingState = CurrentSceneLoadingState::LoadingStart;
			// sgLogDebug("changing to load start");
			return false;
		case Etf::CurrentSceneLoadingState::LoadingStart:
			loadSetupAndBgm();
			// sgLogDebug("changing to load map");
			_currentLoadingState = CurrentSceneLoadingState::LoadingMap;
			return false;
		case Etf::CurrentSceneLoadingState::LoadingMap:
			loadSceneInternal();
			// sgLogDebug("changing to load gameobjects");
			_currentLoadingState = CurrentSceneLoadingState::LoadingGameObjects;
			return false;
		case Etf::CurrentSceneLoadingState::LoadingGameObjects:
			sgLogDebug("Starting load gameobjects");
			GameObjectSystem::LoadGameObjectSystem();
			_currentLoadingState = CurrentSceneLoadingState::LoadingUI;
			return false;
		case Etf::CurrentSceneLoadingState::LoadingUI:
			loadUI();
			// sgLogDebug("changing to load dialog");
			_currentLoadingState = CurrentSceneLoadingState::LoadingDialog;
			return false;
		case Etf::CurrentSceneLoadingState::LoadingDialog:
			loadDialog();
			// sgLogDebug("changing to load finish");
			_currentLoadingState = CurrentSceneLoadingState::LoadingFinish;
			return false;
		case Etf::CurrentSceneLoadingState::LoadingFinish:
			loadEnd();
			_currentLoadingState = CurrentSceneLoadingState::JustLoaded;
			return false;
		case Etf::CurrentSceneLoadingState::JustLoaded:
			sgLogDebug("Starting just loaded");
			StartFullScreenFade(_sceneData.FadeInTime, ScreenFadeTypes::FadeIn);
			_currentLoadingState = CurrentSceneLoadingState::FadingIn;
			return false;
		// After 50% of current time is done, we should allow updates from the gameobjects.
		case CurrentSceneLoadingState::FadingIn:
			sgLogDebug("Starting fading in");
			// Handle if fadetime is 0
			if (!_fadeData.FadeTime) {
				sgLogDebug("End screen fade early, switch to not loading");
				_currentLoadingState = CurrentSceneLoadingState::NotLoading;
				endScreenFade();
				return true;
			}
			if (!_fadeData.FadeTime || _fadeData.FadeTime / _fadeData.CurrentFadeTime >= 0.9f) {
				_currentLoadingState = CurrentSceneLoadingState::FadingInAllowUpdate;
			}
			return false;
		case CurrentSceneLoadingState::FadingInAllowUpdate:
			// sgLogDebug("Starting fading in allow update");
			if (_fadeData.CurrentFadeTime >= _fadeData.FadeTime) {
				IsGameLoading = false;
				_currentLoadingState = CurrentSceneLoadingState::NotLoading;
				sgLogDebug("End screen fade");
				endScreenFade();
			}
			return true;
	}
	return false;
}

void Engine::StartFullScreenFade(float time, ScreenFadeTypes fadeType) {
	if (fadeType == ScreenFadeTypes::NotFading || _fadeData.CurrentFadeStatus != ScreenFadeTypes::NotFading) {
		sgLogDebug("Cannot fade: request: %d, status: %d", fadeType, _fadeData.CurrentFadeStatus);
		return;
	}
	_fadeData.LastFadeColor = GraphicsGetFBOColor();
	_fadeData.CurrentFadeTime = 0;
	_fadeData.FadeTime = time;
	_fadeData.CurrentFadeColor = _fadeData.LastFadeColor;
	_fadeData.CurrentFadeStatus = fadeType;
	_fadeData.EndFadeAlpha = fadeType == ScreenFadeTypes::FadeIn ? 255 : 0;
	GameState::CurrentFadeState = (int)fadeType;
}

void Engine::UpdateScreenFade() {
	if (_fadeData.CurrentFadeStatus == ScreenFadeTypes::NotFading) return;
	_fadeData.CurrentFadeTime += GameState::DeltaTimeSeconds;
	// sgLogDebug("Fade time is %f of %f", _fadeData.CurrentFadeTime, _fadeData.FadeTime);
	// sgLogDebug("Screen is fading currently");
	if (_fadeData.CurrentFadeTime >= _fadeData.FadeTime) {
		_fadeData.CurrentFadeStatus = ScreenFadeTypes::NotFading;
		GameState::CurrentFadeState = (int)_fadeData.CurrentFadeStatus;
		return;
	}
	_fadeData.CurrentFadeColor.A = Tweening::GetTweenedValue(_fadeData.LastFadeColor.A, _fadeData.EndFadeAlpha, _fadeData.CurrentFadeTime, _fadeData.FadeTime);
	GraphicsUpdateFBOColor(&_fadeData.CurrentFadeColor);
}

Sprite* Engine::CreateSpriteFull(const std::string& name, float* followX, float* followY, RectangleF sourceRect, RectangleF offsetSizeRect) {
	auto sprite = NewSprite();
	sprite->parentX = followX;
	sprite->parentY = followY;
	sprite->prevParentX = followX ? *followX : 0;
	sprite->prevParentY = followY ? *followY : 0;
	sprite->Flags |= SpriteFlagVisible;
	sprite->Texture = TextureCreate(name.c_str());
	char* buf;
	size_t sz;
	auto result = GetDataFromDirectory(name.c_str(), &buf, &sz, sDirectory);
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

void Engine::Textures::LoadTextureFromBuffer(Texture* tex, const std::string& name) {
	char* buf;
	size_t sz;
	string filename = format("{}.png", name);
	auto result = GetDataFromDirectory(filename.c_str(), &buf, &sz, sDirectory);
	if (result) {
		TextureLoadFromPngBuffer(tex, name.c_str(), buf, sz);
	} else {
		sgLogDebug("Could not find!");
	}
}

// TODO this should be refactored instead of copy/paste from createspritefull
Sprite* Engine::CreateManualSpriteFull(const std::string& name, float* followX, float* followY, RectangleF sourceRect, RectangleF offsetSizeRect) {
	auto sprite = NewSpriteManual();
	sprite->parentX = followX;
	sprite->parentY = followY;
	sprite->prevParentX = followX ? *followX : 0;
	sprite->prevParentY = followY ? *followY : 0;
	sprite->Flags |= SpriteFlagVisible;
	sprite->Texture = TextureCreate(name.c_str());
	sgLogDebug("Loading sprite %s", name.c_str());
	// auto fullPath = std::format("{}.ogg", name);
	char* buf;
	size_t sz;
	auto result = GetDataFromDirectory(name.c_str(), &buf, &sz, sDirectory);
	if (result) {
		TextureLoadFromPngBuffer(sprite->Texture, name.c_str(), buf, sz);
	} else {
		;
	}
	sprite->Shader = GetDefaultShader();
	sprite->TextureSourceRect = sourceRect;
	sprite->OffsetAndSizeRectF = offsetSizeRect;
	return sprite;
}

void Engine::DrawRectPrimitive(RectangleF& rect, Color color, bool filled, bool cameraOffset) {
	DrawRect(&rect, &color, filled, cameraOffset);
}

void Engine::SetSpriteVisible(Sprite* sprite, bool visible) {
	if (visible)
		sprite->Flags |= SpriteFlagVisible;	 // set bit
	else
		sprite->Flags &= ~SpriteFlagVisible;  // clear bit
}

Text* Engine::TextBoi::CreateText(const std::string& fontName, unsigned int fontSize, RectangleF location, const std::string& text, unsigned int numChars, bool centeredX, bool centeredY) {
	TextSetFont(fontName.c_str(), fontSize, sDirectory);
	auto textPtr = TextCreate(&location, text.c_str());
	textPtr->NumLettersToDraw = numChars;
	textPtr->CenteredX = centeredX;
	textPtr->CenteredY = centeredY;
	textPtr->WordWrap = true;
	TextLoad(textPtr);
	TextOnDirty(textPtr);
	return textPtr;
}

void Engine::TextBoi::DrawText(Text* text, float xOffset, float yOffset, Color& color) {
	TextDraw(text, xOffset, yOffset, &color);
}

float Engine::Tweening::GetTweenedValue(float start, float end, float timeSeconds, float totalSeconds, TweenEaseTypes ease) {
	if (totalSeconds <= 0.0f)
		return end;

	auto func = geLinearInterpolation;

	switch (ease) {
		case TweenEaseTypes::Linear:
			break;

		case TweenEaseTypes::QuintOut:
			func = geQuinticEaseOut;
			break;

		default:
			sgLogDebug("Ease type not implemented, using linear");
			break;
	}
	double t = static_cast<double>(timeSeconds / totalSeconds);
	double progress = func(t);
	double startD = static_cast<double>(start);
	double endD = static_cast<double>(end);
	double value = startD + (endD - startD) * progress;
	if (startD < endD)
		value = std::min(value, endD);
	else
		value = std::max(value, endD);
	return static_cast<float>(value);
}

void Engine::Audio::PlayBGM(const std::string& name, float volume) {
	SetBgmTrack(0);
	// LoadBgm(name.c_str(), volume, -1);
	auto fullPath = std::format("{}.ogg", name);
	char* buf;
	size_t sz;
	GetDataFromDirectory(fullPath.c_str(), &buf, &sz, sDirectory);
	LoadBgmBuffer(fullPath.c_str(), volume, -1, buf, sz);
	PlayBgm();
}

void Engine::Audio::PlayBGMBackground(const std::string& name, float volume) {
	SetBgmTrack(1);
	auto fullPath = std::format("{}.ogg", name);
	char* buf;
	size_t sz;
	GetDataFromDirectory(fullPath.c_str(), &buf, &sz, sDirectory);
	LoadBgmBuffer(fullPath.c_str(), volume, -1, buf, sz);
	PlayBgm();
}

void Engine::Audio::SetGlobalBGMVolume(float volume) {
	SetGlobalBgmVolume(volume);
}

void Engine::Audio::StopBGMBackground() {
	SetBgmTrack(1);
	StopBgm();
}

void Engine::Json::GetJsonBufferFromDirectory(const char* name, char** buf, size_t* sz) {
	auto fullPath = std::format("{}.json", name);
	auto result = GetDataFromDirectory(fullPath.c_str(), buf, sz, sDirectory);
	if (!result) {
		*buf = NULL;
		*sz = 0;
		sgLogError("Could not load json for %s", name);
	}
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
static void loadAllMaps() {
	// auto& config = GameConfig::GetGameConfig();
	// for (auto& scene : config.scene.scenes) {
	// 	_sceneData.SceneToLoad = &scene;
	// 	LoadMap(scene.MapName.c_str());
	// 	// GameObject::LoadAllGameObjects();
	// 	loadUI();
	// 	loadDialog();
	// }
	// BattleSystem::InitializeBattleSystem();
	// // GameObject::DestroyAllGameObjects();
	// loadEnd();
	// // Load all textures
	// ResetCameraFollow();
}

void LoadAllTexturesFromFolder(const std::string& folderPath) {
	namespace fs = std::filesystem;
	for (const auto& entry : fs::directory_iterator(folderPath)) {
		if (!entry.is_regular_file())
			continue;
		std::string filename = entry.path().filename().string();
		sgLogDebug("checking to load %s", entry.path().stem().string().c_str());
		// Check for .png extension
		if (entry.path().extension() == ".png") {
			sgLogDebug("Loading %s", entry.path().stem().string().c_str());
			// Remove the .png suffix
			std::string baseName = entry.path().stem().string();
			// Call your texture loader with just the filename
			auto texture = TextureCreate(baseName.c_str());
			TextureLoadFromPng(texture, baseName.c_str());
		}
	}
}

void Engine::PreloadAssets() {
	std::string textureFolder = format("{}assets/img", GetBasePath());	// from your helper
	LoadAllTexturesFromFolder(textureFolder);
	loadAllMaps();
}

void (*GraphicsPostFBODrawDebugFunc)(void) = Etf::Engine::DebugUI::Render;
