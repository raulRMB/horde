#ifndef HORDE_MODEL_H
#define HORDE_MODEL_H

namespace raylib
{
#include "raylib.h"
}

namespace tZ
{

struct CModel
{
    raylib::Model model;
    float scale;
    bool hidden;
};

}

#endif //HORDE_MODEL_H
