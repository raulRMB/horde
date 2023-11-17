#include "Button.h"

Button::Button(Rectangle box, char* text) {
    this->text = text;
    this->box = box;
}

bool Button::isHovered() {
    return CheckCollisionPointRec(GetMousePosition(), box);
}

void Button::Draw(DrawData data) {
    DrawRectangleRounded(box, 0.2, 10, GetBgColor());
    DrawText(text, box.x, box.y, 28, GetTextColor());
}

void Button::OnAdded() {
    
}

void Button::OnWindowResize(Vector2 screenSize) {
    
}

std::any Button::OnDrag() {
    TraceLog(LOG_INFO, "Drag Started %s", text);
    return std::make_any<char*>(text);
}

void Button::OnHover() {
    TraceLog(LOG_INFO, "Hovered %s", text);
}

void Button::OnHoverExit() {
    TraceLog(LOG_INFO, "Exit hover %s", text);
}

void Button::OnDragCancelled() {
    TraceLog(LOG_INFO, "Drag Cancelled %s", text);
}

void Button::OnDrop(Element* source, std::any payload) {
    TraceLog(LOG_INFO, "Dropped %s", text);
    try {
        char* castedPayload = std::any_cast<char*>(payload);
        char* tmp = text;
        if(castedPayload != nullptr) {
            text = castedPayload;
            Button* button = (Button*)source;
            button->text = tmp;
        }
    } catch (const std::bad_any_cast& e) {}
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