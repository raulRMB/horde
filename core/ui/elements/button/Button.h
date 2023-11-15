#ifndef BUTTON_H
#define BUTTON_H

#include "../../Element.h"
#include <raylib.h>

class Button : public Element {
    Color baseBgColor = {35, 45, 130, 255};
    Color hoveredBgColor = {255, 50, 130, 255};
    Color baseTextColor = {184, 227, 195, 255};
    Color hoveredTextColor = {255, 255, 255, 255};
    Rectangle box = {100, 100, 100, 50};

public:
    virtual bool isHovered() override;
    virtual void Draw() override;
    virtual void Update() override;
private:
    Color GetBgColor();
    Color GetTextColor();
};

#endif