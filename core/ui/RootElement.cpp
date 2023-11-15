#include "RootElement.h"
#include "raylib.h"

namespace RLGameGUI
{
    Rectangle& RootElement::GetScreenRect()
    {
        if (ScreenRect.width <= 0 || ScreenRect.height <= 0)
            OnResize();

        return ScreenRect;
    }

    void RootElement::OnResize()
    {
        ScreenRect.width = (float)GetScreenWidth();
        ScreenRect.height = (float)GetScreenHeight();
    }
}