#include "raylib.h"
#include "../core/ui/RLGameGui.h"
#include "../core/ui/StandardElements.h"
#include "MainScene.h"

void MainScene::Start()
{
    InitUI();
}

void MainScene::InitUI()
{
    int fontSize = 26;
    Texture2D panelBG = LoadTexture("assets/ui/concept.png");
    Font textFont = LoadFontEx("assets/ui/fonts/BebasNeue Book.otf", fontSize, NULL, 0);
    RLGameGUI::GUIScreen::Ptr rootScreen = RLGameGUI::GUIScreen::Create();
	RLGameGUI::GUIPanel::Ptr panel = RLGameGUI::GUIPanel::Create();
    panel->Name = "panel1";
	panel->RelativeBounds = RLGameGUI::RelativeRect(
        RLGameGUI::RelativeValue(1.0f, true), 
        RLGameGUI::RelativeValue(1.0f, false), 
        RLGameGUI::RelativeValue(0.75f, false), 
        RLGameGUI::RelativeValue(0.75f, false), 
        RLGameGUI::AlignmentTypes::Maximum, 
        RLGameGUI::AlignmentTypes::Maximum, 
        Vector2{ 10,10 });
	panel->Background = panelBG;
	panel->Padding = RLGameGUI::RelativePoint(16, 16);
	panel->Fillmode = RLGameGUI::PanelFillModes::NPatch;
	panel->NPatchGutters = Vector2{ 16, 16 };
    panel->Tint = WHITE;
	rootScreen->AddElement(panel);

    RLGameGUI::GUILabel::Ptr label = RLGameGUI::GUILabel::Create("Holy Shit FONTS",textFont,fontSize);
	label->RelativeBounds = RLGameGUI::RelativeRect{ 0, 10, 500, 40 };
	label->Tint = RAYWHITE;
	rootScreen->AddElement(label);
    RLGameGUI::Manager::PushScreen(rootScreen);
}

void MainScene::HandleInput()
{
}

void MainScene::Update(float deltaTime)
{
}

void MainScene::Draw()
{
}

void MainScene::DrawUI()
{
    RLGameGUI::Manager::Render();
}

void MainScene::Clean()
{
}

void MainScene::Save()
{
}

void MainScene::Load()
{
}
