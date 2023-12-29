#include "Button.h"
#include "components/CRectangle.h"

namespace tX
{

Button::Button(CRectangle box, char* text)
{
    Text = text;
    Box = box;
}

bool Button::isHovered()
{
    //return CheckCollisionPointRec(raylib::GetMousePosition(), ToRaylibRect(Box));
}

void Button::Draw(DrawData data)
{
//    DrawRectangleRounded(ToRaylibRect(Box), 0.2, 10, ToRaylibColor(GetBgColor()));
//    raylib::rlDrawText(Text, static_cast<int>(Box.X), static_cast<int>(Box.Y), 28, ToRaylibColor(GetTextColor()));
}

void Button::OnAdded()
{
    
}

void Button::OnWindowResize(v2 screenSize)
{
    
}

std::any Button::OnDrag()
{
    return std::make_any<char*>(Text);
}

void Button::OnHover()
{
}

void Button::OnHoverExit()
{
}

void Button::OnDragCancelled() {
}

void Button::OnDrop(Element* source, std::any payload) {
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
//    if(IsMouseButtonPressed(raylib::MOUSE_LEFT_BUTTON) && hovered) {
//        bActiveClick = true;
//    }
//    if (IsMouseButtonReleased(raylib::MOUSE_LEFT_BUTTON)) {
//        if(bActiveClick && hovered) {
//            bActiveClick = false;
//        }
//    }
}

FColor Button::GetBgColor() {
    if(isHovered())
        return BaseBgColor;
    else
        return HoveredBgColor;
}

FColor Button::GetTextColor() {
    if(isHovered())
        return BaseTextColor;
    else
        return HoveredTextColor;
}

}