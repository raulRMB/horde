#include "GUIElement.h"

namespace RLGameGUI
{
	void GUIElement::Update(Vector2 mousePostion)
	{
        if (RelativeBounds.IsDirty())
            Resize();

		if (Disabled)
			mousePostion.x = -1;

		if (mousePostion.x < 0 || !CheckCollisionPointRec(mousePostion, GetScreenRect()))
		{
			if (Hovered)
				OnHoverEnd();
			Hovered = false;

			if (Clicked)
				OnClickCancel();
			Clicked = false;
		}
		else
		{
			if (!Hovered)
			{
				Hovered = true;
				OnHoverStart();
			}

			if (IsMouseButtonDown(0))
			{
				if (!Clicked)
				{
					Clicked = true;
					OnClickStart();
				}
			}
			else if (Clicked)
			{
				Clicked = false;
				OnClickEnd();
				if (ElementClicked != nullptr)
					ElementClicked(this);
			}
		}

		OnUpdate();
        if (RelativeBounds.IsDirty())
            Resize();

		for (auto child : Children)
			child->Update(mousePostion);
	}

	void GUIElement::Resize()
	{
		OnPreResize();

		Vector2 padding = { 0,0 };
		if (Parent != nullptr)
		{
			ScreenRect = RelativeBounds.Resolve(Parent->GetContentRect());
			padding = Padding.ResolveSize(Parent->GetContentRect());
		}

		ContentRect = ScreenRect;
		ContentRect.x += padding.x;
		ContentRect.y += padding.y;
		ContentRect.width -= padding.x * 2;
		ContentRect.height -= padding.y * 2;

        OnResize();
        for (auto child : Children)
            child->Resize();
	}

    GUIElement::Ptr GUIElement::AddChild(GUIElement::Ptr child)
    {
		child->Parent = this;
		Children.emplace_back(child);
		return child;
    }

    void GUIElement::Render()
	{
		if (Hidden)
			return;

		if (Renders)
			OnRender();

		for (auto child : Children)
			child->Render();
	}

	Rectangle& GUIElement::GetScreenRect()
	{
        if (RelativeBounds.IsDirty())
            Resize();

		return ScreenRect;
	}

	Rectangle& GUIElement::GetContentRect()
	{
        if (RelativeBounds.IsDirty())
            Resize();

        return ContentRect;
	}

	float RelativeValue::ResolvePos(const Rectangle& parrentScreenRect)
	{
		Clean();

		float origin = AxisType == AxisTypes::Horizontal ? parrentScreenRect.x : parrentScreenRect.y;
		float size = AxisType == AxisTypes::Horizontal ? parrentScreenRect.width : parrentScreenRect.height;

		float pixelValue = SizeValue;
		if (SizeType == RelativeSizeTypes::Percent)
			pixelValue *= size;

		pixelValue += origin;

		return pixelValue;
	}

	float RelativeValue::ResolveSize(const Rectangle& parrentScreenRect)
    {
		Clean();

        float size = AxisType == AxisTypes::Horizontal ? parrentScreenRect.width : parrentScreenRect.height;

        float pixelValue = SizeValue;
        if (SizeType == RelativeSizeTypes::Percent)
            pixelValue *= size;

        return pixelValue;
    }

	Vector2 RelativePoint::ResolvePos(const Rectangle& parent)
	{
		Clean();
		return Vector2{ X.ResolvePos(parent), Y.ResolvePos(parent) };
	}

	Vector2 RelativePoint::ResolveSize(const Rectangle& parent)
	{
		Clean();
		return Vector2{ X.ResolveSize(parent), Y.ResolveSize(parent) };
	}

	float GetAllginedValue(float value, AlignmentTypes Alignment, float size, float offset)
	{
		if (Alignment == AlignmentTypes::Maximum)
		{
			value -= size;
			value -= offset;
		}
		else if (Alignment == AlignmentTypes::Center)
		{
			value -= size * 0.5f;
			value += offset;
		}
		else
			value += offset;

		return value;
	}

    Rectangle RelativeRect::Resolve(const Rectangle& parent)
    {
		Vector2 pixelSize = Size.ResolveSize(parent);
		Vector2 pixelOrigin = Origin.ResolvePos(parent);

		pixelOrigin.x = GetAllginedValue(pixelOrigin.x, HorizontalAlignment, pixelSize.x, Offset.x);
		pixelOrigin.y = GetAllginedValue(pixelOrigin.y, VerticalAlignment, pixelSize.y, Offset.y);

		return Rectangle{ pixelOrigin.x, pixelOrigin.y, pixelSize.x, pixelSize.y };
    }
}