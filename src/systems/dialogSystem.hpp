#pragma once
#include <string>

namespace Etf {
	class UIObject;
	class UIText;
class DialogSystem {

	public:
		static void UpdateDialogText(const std::string& newText, int lettersToDisplay = 0);

	private:
		static std::string _currentText;
		static UIObject* _dialogBoxObject;
		static UIText* _dialogBoxTextObject;
};



};	// namespace Etf
