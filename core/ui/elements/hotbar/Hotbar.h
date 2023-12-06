#include <vector>
#include "../../Element.h"
#include "../slot/Slot.h"

namespace tZ
{

class Hotbar : public Element {

    std::vector<Slot*> slots;
    Rectangle hotbarBox;

public:
    virtual bool isHovered() override;
    virtual std::any OnDrag() override;
    virtual void OnDragCancelled() override;
    virtual void OnHover() override;
    virtual void OnHoverExit() override;
    virtual void OnAdded() override;
    virtual void OnWindowResize(v2 screenSize) override;
    virtual void OnDrop(Element* source, std::any payload) override;
    virtual void Draw(DrawData data) override;
    virtual void Update() override;
    void AddSlot(std::string texturePath);
};

}