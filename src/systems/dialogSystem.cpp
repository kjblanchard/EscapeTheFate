#include <Supergoon/Tweening/easing.h>
#include <Supergoon/filesystem.h>
#include <Supergoon/json.h>
#include <Supergoon/log.h>

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

// Time in seconds each letter is displayed
static const float displayTimePerLetter = 0.05;
// Distance to move the dialog box when opening
static const float animationOffset = 30.0f;
static const float animationOpenTime = 1.0f;
static const float animationCloseTime = 0.5f;

enum class DialogBoxStates {
	Unloaded,
	Closed,
	AnimatingOpen,
	DisplayingText,
	DisplayingFinished,
	AnimatingClosed,
};

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

static unordered_map<string, json_object*> _loadedDialog;

static void updateDialogText(const std::string& newText, int lettersToDisplay) {
	if (!_dialogBoxTextObject) {
		sgLogWarn("Could not update text properly, no textObject");
		return;
	}
	sgLogWarn("Letters to display are %d", lettersToDisplay);
	_dialogBoxTextObject->UpdateText(newText, lettersToDisplay);
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

static void startNewDialogInteraction(Textbox* textbox, const std::string& newText) {
	if (_loadedDialog.find(_currentMap) == _loadedDialog.end()) {
		sgLogError("Map dialog not loaded for some reason");
		return;
	}
	auto textJson = _loadedDialog[_currentMap];
	if (!textJson) return;
	auto textArrayJson = jobj(textJson, newText.c_str());
	if (!textArrayJson) sgLogCritical("Bad thing2");
	_currentText = string(jstrIndex(textArrayJson, 0));
	_currentTextbox = textbox;
	sgLogDebug("Updating dialog box: %s", newText.c_str());
	updateDialogText(_currentText, 0);
	_dialogBoxObject->SetVisible(true);
	_currentDisplayedNumChars = 0;
	GameState::InDialog = true;
	_dialogBoxObject->SetX(_dialogBoxStartX - animationOffset);
	_currentState = DialogBoxStates::AnimatingOpen;
}

static void finishCurrentInteraction() {
	_dialogBoxObject->SetX(_dialogBoxStartX);
	updateDialogTextLetters(_currentText.size());
	_currentState = DialogBoxStates::DisplayingFinished;
}

static void closeCurrentInteraction() {
	_dialogBoxObject->SetVisible(false);
	_currentState = DialogBoxStates::Closed;
	GameState::InDialog = false;
}

static void updateDisplayingTextCharacters() {
	_currentTimeOnLetter += GameState::DeltaTimeSeconds;
	while (_currentTimeOnLetter >= displayTimePerLetter) {
		++_currentDisplayedNumChars;
		_currentTimeOnLetter -= displayTimePerLetter;
	}
	updateDialogTextLetters(_currentDisplayedNumChars);
	if (_currentDisplayedNumChars >= _currentText.size()) _currentState = DialogBoxStates::DisplayingFinished;
}

static void openDialogBoxAnimation() {
	_currentAnimationTime += GameState::DeltaTimeSeconds;
	if (_currentAnimationTime > animationOpenTime) {
		_dialogBoxObject->SetX(_dialogBoxStartX);
		_currentState = DialogBoxStates::DisplayingText;
	}
	auto newX = Engine::Tweening::GetTweenedValue(_dialogBoxStartX - animationOffset, _dialogBoxStartX, _currentAnimationTime, animationOpenTime);
	_dialogBoxObject->SetX(newX);
}

void DialogSystem::TextBoxInteractionUpdate(Textbox* textbox, const std::string& newText) {
	switch (_currentState) {
		case DialogBoxStates::Closed:
			startNewDialogInteraction(textbox, newText);
			break;
		case DialogBoxStates::AnimatingOpen:
		case DialogBoxStates::DisplayingText:
			finishCurrentInteraction();
			break;
		case DialogBoxStates::DisplayingFinished:
			closeCurrentInteraction();
		default:
			break;
	}
	return;
}

void DialogSystem::LoadDialogFromJsonFile(const std::string& filename) {
	_currentMap = filename;
	if (_loadedDialog.find(filename) != _loadedDialog.end()) return;
	auto loadString = format("{}assets/dialog/{}.json", GetBasePath(), filename);
	sgLogDebug("Loading dialog for %s", loadString.c_str());
	auto newDialog = jGetObjectFromFile(loadString.c_str());
	if (!newDialog) {
		sgLogWarn("No dialog file found for %s", loadString.c_str());
		return;
	}
	_loadedDialog[filename] = newDialog;
}

void DialogSystem::UpdateDialogSystem() {
	switch (_currentState) {
		case DialogBoxStates::Unloaded:
			initializeDialogBox();
			return;
		case DialogBoxStates::AnimatingOpen:
			openDialogBoxAnimation();
		case DialogBoxStates::DisplayingText:
			updateDisplayingTextCharacters();
			break;
		case DialogBoxStates::Closed:
			break;
		default:
			break;
	}
}

void DialogSystem::ShutdownDialogSystem() {
	for (auto& [key, value] : _loadedDialog) {
		jReleaseObjectFromFile(value);
	}
	_loadedDialog.clear();
}
