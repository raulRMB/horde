#include "Button.h"
#include "components/Rectangle.h"
#include "raylibEx.h"

namespace tZ
{

Button::Button(CRectangle box, char* text)
{
    Text = text;
    Box = box;
}

bool Button::isHovered()
{
    return CheckCollisionPointRec(GetMousePosition(), ToRaylibRect(Box));
}

void Button::Draw(DrawData data)
{
    DrawRectangleRounded(ToRaylibRect(Box), 0.2, 10, ToRaylibColor(GetBgColor()));
    DrawText(Text, static_cast<int>(Box.X), static_cast<int>(Box.Y), 28, ToRaylibColor(GetTextColor()));
}

void Button::OnAdded()
{
    
}

void Button::OnWindowResize(v2 screenSize)
{
    
}

std::any Button::OnDrag()
{
    TraceLog(LOG_INFO, "Drag Started %s", Text);
    return std::make_any<char*>(Text);
}

void Button::OnHover()
{
    TraceLog(LOG_INFO, "Hovered %s", Text);
}

void Button::OnHoverExit()
{
    TraceLog(LOG_INFO, "Exit hover %s", Text);
}

void Button::OnDragCancelled() {
    TraceLog(LOG_INFO, "Drag Cancelled %s", Text);
}

void Button::OnDrop(Element* source, std::any payload) {
    TraceLog(LOG_INFO, "Dropped %s", Text);
    try {
        char* castedPayload = std::any_cast<char*>(payload);
        char* tmp = Text;
        if(castedPayload != nullptr) {
            Text = castedPayload;
            Button* button = (Button*)source;
            button->Text = tmp;
        }
    } catch (const std::bad_any_cast& e) {}
}

void Button::Update() {
    bool hovered = isHovered();
    if(!hovered) {
        bActiveClick = false;
    }
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && hovered) {
        bActiveClick = true;
    }
    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
        if(bActiveClick && hovered) {
            bActiveClick = false;
            TraceLog(LOG_INFO, "Clicked %s", Text);
        }
    }
}

CColor Button::GetBgColor() {
    if(isHovered())
        return BaseBgColor;
    else
        return HoveredBgColor;
}

CColor Button::GetTextColor() {
    if(isHovered())
        return BaseTextColor;
    else
        return HoveredTextColor;
}

}