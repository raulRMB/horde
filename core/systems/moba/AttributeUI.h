#ifndef HORDE_ATTRIBUTEUI_H
#define HORDE_ATTRIBUTEUI_H

#include "systems/System.h"

class SAttributeUI final : public DrawingSystem
{
    Font font;
public:
    SAttributeUI();
    void Draw2D() override;
    void Draw() override {};
};

#endif //HORDE_ATTRIBUTEUI_H
