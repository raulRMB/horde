#include "Slot.h"

namespace tX
{

Slot::Slot(raylib::Rectangle box) {
    this->box = box;
}

bool Slot::isHovered() {
    return raylib::CheckCollisionPointRec(raylib::GetMousePosition(), box);
}

void Slot::OnHover() {
    //raylib::TraceLog(raylib::LOG_INFO, "Hovered slot");
}

void Slot::SetImage(raylib::Texture2D img) {
    image = img;
}

void Slot::OnHoverExit() {
    //raylib::TraceLog(raylib::LOG_INFO, "Exit hover slot");
}

void Slot::SetBox(raylib::Rectangle box) {
    this->box = box;
}

void Slot::Draw(DrawData data) {
    if(isHovered() && data.activeDrag) {
        int op = 3;
        int os = op * 2;
        raylib::DrawRectangleLines(box.x-op, box.y-op, box.width+os, box.height+os, {255,255,255,255});
    }
    if(image.id <= 0) {
        raylib::DrawRectangleRec(box, {255,255,255,255});
    } else {
        raylib::NPatchInfo npatchInfo = { 0 };
        npatchInfo.source = { 0, 0, (float)image.width, (float)image.height };
        npatchInfo.left = 0;
        npatchInfo.top = 0;
        npatchInfo.right = 0;
        npatchInfo.bottom = 0;
        raylib::DrawTextureNPatch(image, npatchInfo, box, {0,0}, 0, {255,255,255,255});
    }
}

void Slot::Update() {
    // TraceLog(LOG_INFO, "SLOT");
}

std::any Slot::OnDrag() {
    return std::make_any<raylib::Texture2D*>(&image);
}

void Slot::OnAdded() {

}

void Slot::OnWindowResize(v2 screenSize) {

}

void Slot::OnDragCancelled() {
    //raylib::TraceLog(raylib::LOG_INFO, "Drag Cancelled");
}

void Slot::OnDrop(Element* source, std::any payload) {
    auto tmp = image;
    try {
        raylib::Texture2D* texture = std::any_cast<raylib::Texture2D*>(payload);
        if(texture != nullptr) {
            image = *texture;
            Slot* slot = (Slot*)source;
            slot->image = tmp;
        }
    } catch (const std::bad_any_cast& e) {}
}

}