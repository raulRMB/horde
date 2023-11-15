#pragma once

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include "GUIElement.h"
#include "RootElement.h"

namespace RLGameGUI
{
	class GUIScreen
	{
	public:
		std::string Name;

		bool IsActive() const;

		void Activate();
		void Deactivate();

		void Update();
		void Render();

		typedef std::shared_ptr<GUIScreen> Ptr;
		inline static Ptr Create() { return std::make_shared<GUIScreen>(); }

		GUIElement::Ptr AddElement(GUIElement::Ptr element);

		typedef std::function<void()> ScreenEventCallback;

		inline void AddPostRenderCallback(ScreenEventCallback callback) { PostRenderCallbacks.emplace_back(callback); }

	protected:
		bool Active = false;
		RootElement Root;
        std::vector<GUIElement::Ptr> Elements;

		void DoResize();

		virtual void OnActivate() {}
		virtual void OnDeactivate() {}
		virtual void OnUpdate() {}
		virtual void OnRender() {}

		virtual void OnElementAdd(GUIElement::Ptr element) {}

		std::vector<ScreenEventCallback> PostRenderCallbacks;
	};
}