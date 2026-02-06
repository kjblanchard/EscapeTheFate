#include <Supergoon/Tweening/easing.h>
#include <Supergoon/filesystem.h>
#include <Supergoon/json.h>
#include <Supergoon/log.h>

#include <filesystem>
#include <format>
#include <gameState.hpp>
#include <gameobject/gameobjects/Textbox.hpp>
#include <systems/dialogSystem.hpp>
#include <ui/ui.hpp>
#include <ui/uiText.hpp>
#include <unordered_map>

#include "bindings/engine.hpp"

using namespace Etf;
using namespace std;

enum class DialogBoxStates {
	Unloaded,
	Closed,
	AnimatingOpen,
	DisplayingText,
	DisplayingFinished,
	AnimatingClosed,
};

// Time in seconds each letter is displayed
static const float Display_Time_Per_Letter = 0.03;
// Distance to move the dialog box when opening
static const float Animation_Offset = 240.0f;
static const float Animation_Open_Time = 0.45f;
static const float Animation_Close_Time = 0.45;
static DialogBoxStates _currentState = DialogBoxStates::Unloaded;
static string _currentText;
static string _currentMap;
static UIObject* _dialogBoxObject;
static UIText* _dialogBoxTextObject;
static Textbox* _currentTextbox = nullptr;
static float _currentTimeOnLetter = 0;
static unsigned int _currentDisplayedNumChars = 0;
static float _currentAnimationTime = 0;
static float _dialogBoxStartX, _dialogBoxStartY;
static unsigned int _currentNumTextInDialog = 0;
static unsigned int _currentTextInDialog = 0;
static unordered_map<string, unordered_map<string, vector<string>>> _loadedDialog;
static vector<string>* _currentDialogVector;

static void updateDialogText(const std::string& newText, int lettersToDisplay) {
	if (!_dialogBoxTextObject) {
		sgLogWarn("Could not update text properly, no textObject");
		return;
	}
	_dialogBoxTextObject->UpdateText(newText, lettersToDisplay);
	_dialogBoxTextObject->UpdateTextNumLetters(lettersToDisplay);
}

static void updateDialogTextLetters(int lettersToDisplay) {
	if (!_dialogBoxTextObject) {
		sgLogWarn("Could not update text properly, no textObject");
		return;
	}
	_dialogBoxTextObject->UpdateTextNumLetters(lettersToDisplay);
}

static void initializeDialogBox() {
	if (!_dialogBoxObject) {
		_dialogBoxObject = UI::RootUIObject->GetChildByName("DialogBox");
	}
	if (_dialogBoxObject && !_dialogBoxTextObject) {
		_dialogBoxTextObject = (UIText*)_dialogBoxObject->GetChildByName("DialogBoxText");
	}
	if (!_dialogBoxObject || !_dialogBoxTextObject) {
		sgLogWarn("Could not initialize dialog box, no ui object found properly");
		return;
	}
	_dialogBoxObject->SetVisible(false);
	_currentState = DialogBoxStates::Closed;
	_dialogBoxStartX = _dialogBoxObject->X();
	_dialogBoxStartY = _dialogBoxObject->Y();
}

static void startDialogText() {
	sgLogDebug("Current text is %s", _currentText.c_str());
	_currentText = _currentDialogVector->at(_currentTextInDialog);
	sgLogDebug("new text is %s", _currentText.c_str());
	_currentDisplayedNumChars = 0;
	_currentTimeOnLetter = 0;
	updateDialogText(_currentText, _currentDisplayedNumChars);
}

static void startNewDialogInteraction(Textbox* textbox, const std::string& newText) {
	auto mapIt = _loadedDialog.find(_currentMap);
	if (mapIt == _loadedDialog.end()) {
		sgLogError("Map dialog not loaded for some reason");
		return;
	}
	auto& dialogMap = mapIt->second;
	auto dialogIt = dialogMap.find(newText);
	if (dialogIt == dialogMap.end()) {
		sgLogError("dialog key not found for %s", newText.c_str());
		return;
	}
	_currentDialogVector = &dialogIt->second;
	_currentTextbox = textbox;
	_currentTextInDialog = 0;
	_currentAnimationTime = 0;
	startDialogText();
	sgLogDebug("Updating dialog box: %s", newText.c_str());
	_currentNumTextInDialog = _currentDialogVector->size();
	_dialogBoxTextObject->SetVisible(false);
	_dialogBoxObject->SetVisible(true);
	GameState::InDialog = true;
	_dialogBoxObject->SetX(_dialogBoxStartX - Animation_Offset);
	_currentState = DialogBoxStates::AnimatingOpen;
}

static void closeCurrentInteraction() {
	_dialogBoxTextObject->SetVisible(false);
	_currentState = DialogBoxStates::AnimatingClosed;
	GameState::InDialog = false;
}

static void progressCurrentInteraction() {
	sgLogWarn("Current text is %d and current num is %d", _currentTextInDialog, _currentNumTextInDialog);
	if (_currentTextInDialog >= _currentNumTextInDialog - 1) {
		closeCurrentInteraction();
	} else {
		++_currentTextInDialog;
		startDialogText();
		_currentState = DialogBoxStates::DisplayingText;
		Engine::Audio::PlayBGMBackground("typing");
	}
}

static void finishCurrentInteraction() {
	_dialogBoxTextObject->SetVisible(true);
	_currentAnimationTime = 0;
	_dialogBoxObject->SetX(_dialogBoxStartX);
	updateDialogTextLetters(_currentText.size());
	_currentState = DialogBoxStates::DisplayingFinished;
	Engine::Audio::StopBGMBackground();
}

static void updateDisplayingTextCharacters() {
	_currentTimeOnLetter += GameState::DeltaTimeSeconds;
	while (_currentTimeOnLetter >= Display_Time_Per_Letter) {
		++_currentDisplayedNumChars;
		_currentTimeOnLetter -= Display_Time_Per_Letter;
	}
	if (_currentDisplayedNumChars >= _currentText.size()) {
		finishCurrentInteraction();
		return;
	}
	updateDialogTextLetters(_currentDisplayedNumChars);
}

static void updateOpenDialogBoxAnimation() {
	_currentAnimationTime += GameState::DeltaTimeSeconds;
	if (_currentAnimationTime > Animation_Open_Time) {
		_dialogBoxObject->SetX(_dialogBoxStartX);
		_dialogBoxTextObject->SetVisible(true);
		_currentAnimationTime = 0.0f;
		_currentState = DialogBoxStates::DisplayingText;
		Engine::Audio::PlayBGMBackground("typing");
		return;
	}
	auto newX = Engine::Tweening::GetTweenedValue(_dialogBoxStartX - Animation_Offset, _dialogBoxStartX, _currentAnimationTime, Animation_Open_Time, Engine::Tweening::TweenEaseTypes::QuintOut);
	_dialogBoxObject->SetX(newX);
}

static void updateCloseDialogBoxAnimation() {
	_currentAnimationTime += GameState::DeltaTimeSeconds;
	if (_currentAnimationTime > Animation_Close_Time) {
		_dialogBoxObject->SetX(_dialogBoxStartX - Animation_Offset);
		_dialogBoxObject->SetVisible(false);
		_currentState = DialogBoxStates::Closed;
		return;
	}
	auto newX = Engine::Tweening::GetTweenedValue(_dialogBoxStartX, _dialogBoxStartX - Animation_Offset, _currentAnimationTime, Animation_Close_Time, Engine::Tweening::TweenEaseTypes::QuintOut);
	_dialogBoxObject->SetX(newX);
}

void DialogSystem::TextBoxInteractionUpdate(Textbox* textbox, const std::string& newText) {
	switch (_currentState) {
		// Initialize the text if the dialog box is fully closed
		case DialogBoxStates::Closed:
			startNewDialogInteraction(textbox, newText);
			break;
		// If player interacts when it is opening or displaying, finish animation and also update the text
		case DialogBoxStates::AnimatingOpen:
		case DialogBoxStates::DisplayingText:
			finishCurrentInteraction();
			break;
		case DialogBoxStates::DisplayingFinished:
			progressCurrentInteraction();
		case DialogBoxStates::AnimatingClosed:
		default:
			break;
	}
	return;
}

static void parseJsonIntoMap(json_object* dialogRoot) {
	Engine::Json::jforeach_lambda(dialogRoot, [&](const char* key, void* value) {
		json_object* child = (json_object*)value;
		auto arraySize = jGetObjectArrayLength(child);
		for (auto i = 0; i < arraySize; ++i) {
			_loadedDialog[_currentMap][key].push_back(jstrIndex(child, i));
		}
	});
}

void DialogSystem::LoadDialogFromJsonFile(const std::string& filename) {
	_currentMap = filename;
	if (_loadedDialog.find(filename) != _loadedDialog.end()) return;
	auto loadString = format("{}assets/dialog/{}.json", GetBasePath(), filename);
	if (!std::filesystem::exists(loadString)) {
		sgLogDebug("No dialog file for %s", loadString.c_str());
		return;
	}
	auto newDialog = jGetObjectFromFile(loadString.c_str());
	if (!newDialog) {
		return;
	}
	parseJsonIntoMap(newDialog);
	jReleaseObjectFromFile(newDialog);
}

void DialogSystem::UpdateDialogSystem() {
	switch (_currentState) {
		case DialogBoxStates::Unloaded:
			initializeDialogBox();
			return;
		case DialogBoxStates::AnimatingOpen:
			updateOpenDialogBoxAnimation();
			break;
		case DialogBoxStates::DisplayingText:
			updateDisplayingTextCharacters();
			break;
		case DialogBoxStates::AnimatingClosed:
			updateCloseDialogBoxAnimation();
		case DialogBoxStates::Closed:
			break;
		default:
			break;
	}
}

void DialogSystem::ShutdownDialogSystem() {
	_loadedDialog.clear();
}
