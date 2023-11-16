#ifndef SLOT_H
#define SLOT_H

#include "../../Element.h"
#include <raylib.h>

class Slot : public Element
{
    Rectangle box;
    Texture2D image;

public:
    Slot(Rectangle box);
    virtual bool isHovered() override;
    virtual void* OnDrag() override;
    virtual void OnDragCancelled() override;
    virtual void OnDrop(Element* source, void* payload) override;
    virtual void Draw(bool dragActive) override;
    virtual void Update() override;
    void SetImage(Texture2D image);
};

#endif