#ifndef SHAPEDRAWINGSYSTEM_H
#define SHAPEDRAWINGSYSTEM_H

#include "systems/System.h"

namespace tZ
{

class SShapeDrawing final : public DrawingSystem
{
public:
    void Draw() override;
};

}

#endif //SHAPEDRAWINGSYSTEM_H