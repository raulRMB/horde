#ifndef HORDE_ATTRIBUTEUI_H
#define HORDE_ATTRIBUTEUI_H

#include "systems/System.h"

namespace tZ
{

class SAttributeUI final : public DrawingSystem
{
public:
    SAttributeUI();
    void Draw2D() override;
    void Draw() override {};
};

}

#endif //HORDE_ATTRIBUTEUI_H
