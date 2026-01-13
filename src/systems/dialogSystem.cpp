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

string DialogSystem::_currentText;
std::string DialogSystem::_currentMap;
UIObject* DialogSystem::_dialogBoxObject;
UIText* DialogSystem::_dialogBoxTextObject;
Textbox* DialogSystem::_currentTextbox = nullptr;

static unordered_map<string, json_object*> _loadedDialog;

void DialogSystem::UpdateDialogText(const std::string& newText, int lettersToDisplay) {
	if (!_dialogBoxObject) {
		_dialogBoxObject = UI::RootUIObject->GetChildByName("DialogBox");
	}
	if (_dialogBoxObject && !_dialogBoxTextObject) {
		_dialogBoxTextObject = (UIText*)_dialogBoxObject->GetChildByName("DialogBoxText");
	}
	if (!_dialogBoxObject || !_dialogBoxTextObject) {
		sgLogWarn("Not updating dialog box, could not find dialog box or text");
		return;
	}
	_dialogBoxTextObject->UpdateText(newText);
}

void DialogSystem::TextBoxInteractionUpdate(Textbox* textbox, const std::string& newText) {
	if (_loadedDialog.find(_currentMap) == _loadedDialog.end()) {
		sgLogError("Map dialog not loaded for some reason");
		return;
	}
	auto textJson = _loadedDialog[_currentMap];
	if(!textJson) return;
	sgLogWarn("Trying to get json object with %s", newText.c_str());

	auto textArrayJson = jobj(textJson, newText.c_str());
	if(!textArrayJson) sgLogCritical("Bad thing2");
	_currentText = string(jstrIndex(textArrayJson, 0));
	_currentTextbox = textbox;
	sgLogDebug("Updating dialog box text to be %s", _currentText.c_str());
	UpdateDialogText(_currentText, _currentText.size());
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
