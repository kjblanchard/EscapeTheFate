#pragma once
#include <string>
#include <gameobject/GameObject.hpp>
#include <interfaces/IInteractable.hpp>
struct TiledObject;
namespace Etf {
class Textbox : public GameObject, public IInteractable {
   public:
	static void Create(TiledObject* objData);
	Textbox(TiledObject* objData);
	~Textbox();
	void Draw() override;
	void Interact() override;

   private:
	std::string _textToWrite;
};

}  // namespace Etf
