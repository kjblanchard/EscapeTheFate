#include <Supergoon/filesystem.h>
#include <Supergoon/json.h>
#include <Supergoon/log.h>

#include <format>
#include <gameobject/gameobjects/Textbox.hpp>
#include <systems/dialogSystem.hpp>
#include <ui/ui.hpp>
#include <ui/uiText.hpp>
#include <unordered_map>

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

static DialogBoxStates _currentState = DialogBoxStates::Unloaded;
// static unsigned int _currentDisplayedNumChars = 0;
static string _currentText;
static string _currentMap;
static UIObject* _dialogBoxObject;
static UIText* _dialogBoxTextObject;
static Textbox* _currentTextbox = nullptr;

static unordered_map<string, json_object*> _loadedDialog;

static void updateDialogText(const std::string& newText, int lettersToDisplay) {
	if (!_dialogBoxTextObject) {
		sgLogWarn("Could not update text properly, no textObject");
		return;
	}
	_dialogBoxTextObject->UpdateText(newText);
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
	updateDialogText(_currentText, _currentText.size());
	_dialogBoxObject->SetVisible(true);
	_currentState = DialogBoxStates::DisplayingFinished;
}

static void finishCurrentInteraction() {
}

static void closeCurrentInteraction() {
	_dialogBoxObject->SetVisible(false);
	_currentState = DialogBoxStates::Closed;
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
		case DialogBoxStates::Closed:
			return;

		default:
			return;
	}
}

void DialogSystem::ShutdownDialogSystem() {
	for (auto& [key, value] : _loadedDialog) {
		jReleaseObjectFromFile(value);
	}
	_loadedDialog.clear();
}
