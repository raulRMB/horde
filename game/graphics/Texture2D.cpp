//
// Created by Raul Romero on 2023-12-30.
//

#include "Texture2D.h"
#include <stb_image.h>

namespace tX
{

bgfx::TextureHandle FTexture2D::Load(const std::string &file)
{
    stbi_uc *pixels = stbi_load(file.c_str(), &Width, &Height, nullptr, 0);

    Handle = bgfx::createTexture2D(
            (uint16_t)Width,
            (uint16_t)Height,
            false,
            1,
            bgfx::TextureFormat::RGB8,
            BGFX_TEXTURE_NONE,
            bgfx::copy(pixels, Width * Height * 3)
    );

    stbi_image_free(pixels);

    return Handle;
}

}