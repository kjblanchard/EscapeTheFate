#pragma once
#include <gameobject/GameObject.hpp>
#include <interfaces/IInteractable.hpp>
class TiledObject;
namespace Etf {
class Textbox: public GameObject, public IInteractable {
   public:
	static void Create(TiledObject* objData);
	Textbox(TiledObject* objData);
    void Draw() override;
    void Interact() override;
};

}  // namespace Etf
