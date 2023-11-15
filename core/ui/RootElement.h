#pragma once

#include "GUIElement.h"

namespace RLGameGUI
{
    class RootElement : public GUIElement
    {
    public:
        RootElement() {Name = "RootElement"; }

    protected:
        Rectangle& GetScreenRect() override;
        Rectangle& GetContentRect() override { return GetScreenRect(); }

        void OnResize() override;
    };
}