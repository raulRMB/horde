#pragma once

#include "GUIScreen.h"
#include <functional>

namespace RLGameGUI
{
	namespace Manager
	{
		void Update();
		void Render();
		
		void PushScreen(GUIScreen::Ptr screen);
		GUIScreen::Ptr PopScreen();
		GUIScreen::Ptr TopScreen();

		using TextCallback = std::function<void(std::string& text)>;

		// void SetTextFocus(TextCallback callback);
	}
}