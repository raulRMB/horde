#include "Button.h"

Button::Button(Rectangle box, char* text) {
    this->text = text;
    this->box = box;
}

bool Button::isHovered() {
    return CheckCollisionPointRec(GetMousePosition(), box);
}

void Button::Draw(bool dragActive) {
    DrawRectangleRounded(box, 0.2, 10, GetBgColor());
    DrawText(text, box.x, box.y, 28, GetTextColor());
}

void* Button::OnDrag() {
    TraceLog(LOG_INFO, "Drag Started %s", text);
    return text;
}

void Button::OnDragCancelled() {
    TraceLog(LOG_INFO, "Drag Cancelled %s", text);
}

void Button::OnDrop(Element* source, void* payload) {
    TraceLog(LOG_INFO, "Dropped %s", text);
    char* tmp = text;
    text = (char*)payload;
    Button* button = (Button*)source;
    button->text = tmp;
}

void Button::Update() {
    bool hovered = isHovered();
    if(!hovered) {
        activeClick = false;
    }
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hovered) {
        activeClick = true;
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if(activeClick && hovered) {
            activeClick = false;
            TraceLog(LOG_INFO, "Clicked %s", text);
        }
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