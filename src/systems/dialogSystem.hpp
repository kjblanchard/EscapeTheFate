#pragma once
#include <string>

namespace Etf {
	class UIObject;
	class UIText;
	class Textbox;
class DialogSystem {
	public:
		//If a interaction is started that needs dialog box, please send it here
		static void TextBoxInteractionUpdate(Textbox* textbox, const std::string& newText);
		//Loads all the dialog from a file, so it can be referenced.  Caches it
		static void LoadDialogFromJsonFile(const std::string& filename);
		//Updates the dialog system so it animates and draws the characters properly
		static void UpdateDialogSystem();
		//Clears up everything.
		static void ShutdownDialogSystem();
};



};	// namespace Etf
