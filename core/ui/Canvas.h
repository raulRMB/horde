#ifndef CANVAS_H
#define CANVAS_H

#include "Element.h"
#include <vector>

class Canvas {
    std::vector<Element*> elements;

public:
    void Add(Element* element);
    void Draw();
    void Update();
};

#endif