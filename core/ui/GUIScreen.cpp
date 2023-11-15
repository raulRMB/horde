#include "GUIScreen.h"
#include "raylib.h"

namespace RLGameGUI
{
	bool GUIScreen::IsActive() const
	{
		return Active;
	}

	void GUIScreen::Activate()
	{
		Active = true;
		DoResize();
		OnActivate();
	}

	void GUIScreen::Deactivate()
	{
		Active = false;
		OnDeactivate();
	}

	void GUIScreen::DoResize()
    {
        Root.Resize();
        for (auto child : Elements)
            child->Resize();
	}

	void GUIScreen::Update()
	{
		PostRenderCallbacks.clear();

		if (IsWindowResized())
			DoResize();

		// do input
		Vector2 mouse = GetMousePosition();

		// let everyone think
		for (auto child : Elements)
			child->Update(mouse);
	}

	void GUIScreen::Render()
	{
		OnRender();
        for (auto child : Elements)
            child->Render();

		for (auto& callback : PostRenderCallbacks)
			callback();
	}

    GUIElement::Ptr GUIScreen::AddElement(GUIElement::Ptr element)
    {
		element->Parent = &Root;
		Elements.emplace_back(element);
		OnElementAdd(element);

		return element;
    }
}