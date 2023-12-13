#ifndef SLOT_H
#define SLOT_H

#include "ui/Element.h"
namespace raylib
{
#include "raylib.h"
}
namespace tZ
{

class Slot : public Element
{
    raylib::Rectangle box;
    raylib::Texture2D image;

public:
    Slot(raylib::Rectangle box);
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
    void SetImage(raylib::Texture2D image);
    void SetBox(raylib::Rectangle box);
};

}

#endif