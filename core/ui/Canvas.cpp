#include "Element.h"
#include <vector>

#include "Canvas.h"

void Canvas::Add(Element* element) {
    elements.push_back(element);
}

void Canvas::Draw() {
    for (Element* element : elements) {
        element->Draw();
    }
}

void Canvas::Update() {
    for (Element* element : elements) {
        element->Update();
    }
}