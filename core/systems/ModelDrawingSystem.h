#ifndef MODELDRAWINGSYSTEM_H
#define MODELDRAWINGSYSTEM_H

#include "System.h"

class ModelDrawingSystem final : public DrawingSystem
{
public:
    void Draw() override;
};

#endif
