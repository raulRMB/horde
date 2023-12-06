#include <string>
#include "../../Canvas.h"
#include "Hotbar.h"

namespace tZ
{

bool Hotbar::isHovered()  {
    return false;
}

std::any Hotbar::OnDrag() {
    return nullptr;
}

void Hotbar::OnDragCancelled() {

}

void Hotbar::OnHover() {
}

void Hotbar::OnWindowResize(v2 screenSize) {
    float width = screenSize.x * 0.3;

    float slotWidth = width / slots.size();
    if(slotWidth > 80) {
        slotWidth = 80;
    }
    float slotHeight = slotWidth;
    float height = slotWidth;

    hotbarBox = {(screenSize.x/2) - (width/2), screenSize.y - height, width, height};
    int slotIndex = 0;

    float px = 6;
    float py = 10;
    for(Slot* slot : slots) {
        raylib::Rectangle slotBox = raylib::Rectangle{
            hotbarBox.x + (slotIndex * slotWidth) + (px * slotIndex) + slotIndex,
            hotbarBox.y - py,
            slotWidth,
            slotHeight
        };
        slot->SetBox(slotBox);
        slotIndex++;
    }
}

void Hotbar::OnHoverExit() {
}

void Hotbar::OnDrop(Element* source, std::any payload) {

}

void Hotbar::Draw(DrawData data) {
//    DrawRectangleRec(hotbarBox, RAYWHITE);
}

void Hotbar::Update() {

}

void Hotbar::OnAdded() {
    for(Slot* slot : slots) {
        canvas->Add(slot);
    }
}

void Hotbar::AddSlot(std::string texturePath) {
    Slot* slot = new Slot({0, 0, 0, 0});
    auto slotImg = raylib::LoadTexture(texturePath.c_str());
    slot->SetImage(slotImg);
    slots.push_back(slot);
}

}