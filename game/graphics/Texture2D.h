#ifndef HORDE_TEXTURE2D_H
#define HORDE_TEXTURE2D_H

#include <bgfx/bgfx.h>
#include <string>
#include "util/defines.h"

namespace tX
{

class FTexture2D
{
    i32 Width, Height;
    bgfx::TextureHandle Handle;
public:
    bgfx::TextureHandle Load(const std::string& file);
};

}

#endif //HORDE_TEXTURE2D_H
