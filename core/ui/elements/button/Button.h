#ifndef BUTTON_H
#define BUTTON_H

#include "../../Element.h"
#include <raylib.h>

class Button : public Element
{
    Color baseBgColor = {35, 45, 130, 255};
    Color hoveredBgColor = {255, 50, 130, 255};
    Color baseTextColor = {184, 227, 195, 255};
    Color hoveredTextColor = {255, 255, 255, 255};
    char* text = "";
    Rectangle box;

public:
    Button(Rectangle box, char* text);
    virtual bool isHovered() override;
    virtual void* OnDrag() override;
    virtual void OnDragCancelled() override;
    virtual void OnDrop(Element* source, void* payload) override;
    virtual void Draw(bool dragActive) override;
    virtual void Update() override;
private:
    bool activeClick = false;
    Color GetBgColor();
    Color GetTextColor();
};

#endif