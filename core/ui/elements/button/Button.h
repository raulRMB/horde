#ifndef BUTTON_H
#define BUTTON_H

#include "../../Element.h"
#include "../../../primitives/Color.h"
#include "components/Rectangle.h"
#include "defines.h"

namespace tZ
{

class Button : public Element
{
    CColor BaseBgColor = {35, 45, 130, 255};
    CColor HoveredBgColor = {255, 50, 130, 255};
    CColor BaseTextColor = {184, 227, 195, 255};
    CColor HoveredTextColor = {255, 255, 255, 255};
    char* Text = nullptr;
    CRectangle Box{};

public:
    Button(CRectangle box, char* text);
    virtual bool isHovered() override;
    virtual std::any OnDrag() override;
    virtual void OnHover() override;
    virtual void OnHoverExit() override;
    virtual void OnAdded() override;
    virtual void OnDragCancelled() override;
    virtual void OnWindowResize(v2 screenSize) override;
    virtual void OnDrop(Element* source, std::any payload) override;
    virtual void Draw(DrawData data) override;
    virtual void Update() override;
private:
    bool bActiveClick = false;
    CColor GetBgColor();
    CColor GetTextColor();
};

}

#endif