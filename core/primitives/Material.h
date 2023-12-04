#ifndef F_MATERIAL_H
#define F_MATERIAL_H

#include <raylib.h>

namespace tZ
{

typedef struct FMaterial
{
    Shader shader;          // Material shader
    MaterialMap *maps;      // Material maps array (MAX_MATERIAL_MAPS)
    float params[4];        // Material generic parameters (if required)
} FMaterial;

}

#endif //MATERIAL_H
