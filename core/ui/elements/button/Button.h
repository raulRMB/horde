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
    char* text = nullptr;
    Rectangle box;

public:
    Button(Rectangle box, char* text);
    virtual bool isHovered() override;
    virtual std::any OnDrag() override;
    virtual void OnHover() override;
    virtual void OnHoverExit() override;
    virtual void OnAdded() override;
    virtual void OnDragCancelled() override;
    virtual void OnWindowResize(Vector2 screenSize) override;
    virtual void OnDrop(Element* source, std::any payload) override;
    virtual void Draw(DrawData data) override;
    virtual void Update() override;
private:
    bool activeClick = false;
    Color GetBgColor();
    Color GetTextColor();
};

#endif