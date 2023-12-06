#ifndef F_MATERIAL_H
#define F_MATERIAL_H

namespace raylib
{
#include <raylib.h>
}

namespace tZ
{

typedef struct FMaterial
{
    raylib::Shader shader;          // Material shader
    raylib::MaterialMap *maps;      // Material maps array (MAX_MATERIAL_MAPS)
    float params[4];        // Material generic parameters (if required)
} FMaterial;

}

#endif //MATERIAL_H
