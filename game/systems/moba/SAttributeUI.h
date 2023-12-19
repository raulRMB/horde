#ifndef HORDE_SATTRIBUTEUI_H
#define HORDE_SATTRIBUTEUI_H

#include "systems/System.h"

namespace tX
{

class SAttributeUI final : public DrawingSystem
{
public:
    SAttributeUI();
    void Draw2D() override;
    void Draw() override {};
};

}

#endif //HORDE_SATTRIBUTEUI_H
