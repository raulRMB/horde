#include "Element.h"
#include <vector>
#include <raylib.h>
#include <raymath.h>
#include "Canvas.h"

void Canvas::Add(Element* element) {
    elements.push_back(element);
    element->canvas = this;
}

void Canvas::Draw() {
    for (Element* element : elements) {
        element->Draw(activeDrag);
    }
    if(activeDrag) {
        Vector2 currMouse = GetMousePosition();
        NPatchInfo npatchInfo = { 0 };
        Texture2D dragImage = (*(Texture2D*)draggedPayload);
        npatchInfo.source = { 0, 0, (float)dragImage.width, (float)dragImage.height };
        npatchInfo.left = 0;
        npatchInfo.top = 0;
        npatchInfo.right = 0;
        npatchInfo.bottom = 0;
        int size = 40;
        auto box = Rectangle{currMouse.x - (size / 2), currMouse.y - (size / 2), (float)size, (float)size};
        DrawTextureNPatch(dragImage, npatchInfo, box, {0,0}, 0, {255,255,255,200});
    }
}

void Canvas::Update() {
    HandleEvents();
    for (Element* element : elements) {
        element->Update();
    }
}

Element* Canvas::FindFirstHoveredElement() {
    for (Element* element : elements) {
        if(element->isHovered()) {
            return element;
        }
    }
    return nullptr;
}

void Canvas::HandleEvents() {
    float dragThreshold = 0.5;
    if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        activeClick = true;
        activeDragStartPos = GetMousePosition();
    }
    if(activeClick) {
        if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            activeClick = false;
            if(activeDrag) {
                activeDrag = false;
                Element* checkDrop = FindFirstHoveredElement();
                if(checkDrop != nullptr) {
                    checkDrop->OnDrop(dragged, draggedPayload);
                } else {
                    dragged->OnDragCancelled();
                }
            }
        } else if(!activeDrag && Vector2Distance(GetMousePosition(), activeDragStartPos) > dragThreshold) {
            Element* checkDrag = FindFirstHoveredElement();
            if(checkDrag != nullptr) {
                activeDrag = true;
                dragged = checkDrag;
                draggedPayload = dragged->OnDrag();
            }
        }
    }
}