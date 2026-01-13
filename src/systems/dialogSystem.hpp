#pragma once
#include <string>

namespace Etf {
	class UIObject;
	class UIText;
	class Textbox;
class DialogSystem {
	public:
		static void TextBoxInteractionUpdate(Textbox* textbox, const std::string& newText);
		static void UpdateDialogText(const std::string& newText, int lettersToDisplay = 0);
		static void LoadDialogFromJsonFile(const std::string& filename);

	private:
		static std::string _currentText;
		static std::string _currentMap;
		static Textbox* _currentTextbox;
		static UIObject* _dialogBoxObject;
		static UIText* _dialogBoxTextObject;
};



};	// namespace Etf
