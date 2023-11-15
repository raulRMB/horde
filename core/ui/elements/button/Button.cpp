#include "Button.h"

bool Button::isHovered() {
    return CheckCollisionPointRec(GetMousePosition(), box);
} 

void Button::Draw() {
    DrawRectangleRounded(box, 0.2, 10, GetBgColor());
    DrawText("Click Me", 100, 100, 28, GetTextColor());
}

void Button::Update() {
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && isHovered()) {
        TraceLog(LOG_INFO, "Clicked");
    }
}

Color Button::GetBgColor() {
    if(isHovered())
        return baseBgColor;
    else
        return hoveredBgColor;
}

Color Button::GetTextColor() {
    if(isHovered())
        return baseTextColor;
    else
        return hoveredTextColor;
}