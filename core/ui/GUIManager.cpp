#include "GUIManager.h"

#include <stack>

namespace RLGameGUI
{
	namespace Manager
	{
		std::stack<GUIScreen::Ptr>	ScreenStack;

		void Update()
		{
			auto top = TopScreen();
			if (top != nullptr)
				top->Update();
		}

		void Render()
		{
			auto top = TopScreen();
			if (top == nullptr)
				return;

			top->Render();
		}
		
		void PushScreen(GUIScreen::Ptr screen)
		{
			auto top = TopScreen();
			if (top != nullptr)
				top->Deactivate();

			ScreenStack.push(screen);
			screen->Activate();
		}

		GUIScreen::Ptr PopScreen()
		{
			auto top = TopScreen();
			if (top != nullptr)
				top->Deactivate();

			ScreenStack.pop();

			top = TopScreen();
			if (top != nullptr)
				top->Activate();

			return TopScreen();
		}

		GUIScreen::Ptr TopScreen()
		{
			if (ScreenStack.empty())
				return nullptr;

			return ScreenStack.top();
		}
	}
}