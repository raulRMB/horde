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
        element->Draw();
    }
    if(activeDrag) {
        Vector2 currMouse = GetMousePosition();
        DrawRectangle(currMouse.x, currMouse.y, 50, 50, {255, 255, 255, 255});
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