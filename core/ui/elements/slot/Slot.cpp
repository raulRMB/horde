#include "Slot.h"

namespace tZ
{

Slot::Slot(Rectangle box) {
    this->box = box;
}

bool Slot::isHovered() {
    return CheckCollisionPointRec(GetMousePosition(), box);
}

void Slot::OnHover() {
    TraceLog(LOG_INFO, "Hovered slot");
}

void Slot::SetImage(Texture2D img) {
    image = img;
}

void Slot::OnHoverExit() {
    TraceLog(LOG_INFO, "Exit hover slot");
}

void Slot::SetBox(Rectangle box) {
    this->box = box;
}

void Slot::Draw(DrawData data) {
    if(isHovered() && data.activeDrag) {
        int op = 3;
        int os = op * 2;
        DrawRectangleLines(box.x-op, box.y-op, box.width+os, box.height+os, {255,255,255,255});
    }
    if(image.id <= 0) {
        DrawRectangleRec(box, {255,255,255,255});        
    } else {
        NPatchInfo npatchInfo = { 0 };
        npatchInfo.source = { 0, 0, (float)image.width, (float)image.height };
        npatchInfo.left = 0;
        npatchInfo.top = 0;
        npatchInfo.right = 0;
        npatchInfo.bottom = 0;
        DrawTextureNPatch(image, npatchInfo, box, {0,0}, 0, {255,255,255,255});
    }
}

void Slot::Update() {
    // TraceLog(LOG_INFO, "SLOT");
}

std::any Slot::OnDrag() {
    return std::make_any<Texture2D*>(&image);
}

void Slot::OnAdded() {

}

void Slot::OnWindowResize(v2 screenSize) {

}

void Slot::OnDragCancelled() {
    TraceLog(LOG_INFO, "Drag Cancelled");
}

void Slot::OnDrop(Element* source, std::any payload) {
    auto tmp = image;
    try {
        Texture2D* texture = std::any_cast<Texture2D*>(payload);
        if(texture != nullptr) {
            image = *texture;
            Slot* slot = (Slot*)source;
            slot->image = tmp;
        }
    } catch (const std::bad_any_cast& e) {}
}

}