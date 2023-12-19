set(SOURCES
    imgui.h
    imgui.cpp
    imgui_draw.cpp
    imgui_widgets.cpp
    imgui_impl_raylib.h
    imgui_tables.cpp
    rlImGui.cpp
    rlImGui.h
    rlImGuiColors.h
    imgui_demo.cpp
)

add_library(imgui STATIC ${SOURCES})