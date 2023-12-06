#ifndef MODELDRAWINGSYSTEM_H
#define MODELDRAWINGSYSTEM_H

#include "systems/System.h"

namespace tZ
{

class SModelDrawing final : public DrawingSystem
{
public:
    void Draw() override;
};

}

#endif
