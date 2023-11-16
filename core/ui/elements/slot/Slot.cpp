#include "Slot.h"

Slot::Slot(Rectangle box) {
    this->box = box;
}

bool Slot::isHovered() {
    return CheckCollisionPointRec(GetMousePosition(), box);
}

void Slot::SetImage(Texture2D img) {
    image = img;
}

void Slot::Draw(bool dragActive) {
    if(isHovered() && dragActive) {
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

void* Slot::OnDrag() {
    return (void*)&image;
}

void Slot::OnDragCancelled() {
    TraceLog(LOG_INFO, "Drag Cancelled");
}

void Slot::OnDrop(Element* source, void* payload) {
    auto tmp = image;
    image = (*(Texture2D*)payload);
    Slot* slot = (Slot*)source;
    slot->image = tmp;
}