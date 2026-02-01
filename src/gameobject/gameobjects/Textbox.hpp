#pragma once
#include <gameobject/GameObject.hpp>
#include <interfaces/IInteractable.hpp>
struct TiledObject;
namespace Etf {
class Textbox : public GameObject, public IInteractable {
   public:
	static void Create(TiledObject* objData);
	Textbox(TiledObject* objData);
	void Draw() override;
	void Interact() override;

   private:
	std::string _textToWrite;
};

}  // namespace Etf
