#ifndef SLOT_H
#define SLOT_H

#include "../../Element.h"
#include <raylib.h>

namespace tZ
{

class Slot : public Element
{
    Rectangle box;
    Texture2D image;

public:
    Slot(Rectangle box);
    virtual bool isHovered() override;
    virtual std::any OnDrag() override;
    virtual void OnHover() override;
    virtual void OnHoverExit() override;
    virtual void OnDragCancelled() override;
    virtual void OnAdded() override;
    virtual void OnWindowResize(v2 screenSize) override;
    virtual void OnDrop(Element* source, std::any payload) override;
    virtual void Draw(DrawData data) override;
    virtual void Update() override;
    void SetImage(Texture2D image);
    void SetBox(Rectangle box);
};

}

#endif