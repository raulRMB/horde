#include "AttributeUI.h"

#include "entt/entt.hpp"
namespace raylib
{
#include "raylib.h"
}
#include "components/Attribute.h"
#include "util/Util.h"
#include "components/Transform.h"

static raylib::Font font;
namespace tZ
{

SAttributeUI::SAttributeUI() {
    font = raylib::LoadFont("../assets/ui/fonts/Roboto-Bold.ttf");
}

void SAttributeUI::Draw2D()
{
    entt::registry& registry = Game::GetRegistry();
    for(const entt::entity& entity : Game::GetRegistry().view<CAttributeSet, CTransform>())
    {
        CTransform& t = registry.get<CTransform>(entity);
        raylib::Vector2 healthBarPos = GetWorldToScreen(
                (raylib::Vector3){ t.Position.x, t.Position.y + 14.0f, t.Position.z },
                ToRaylibCamera(Game::Instance().GetActiveCamera()));
        CAttributeSet& ac = registry.get<CAttributeSet>(entity);
        FAttribute& health = *Util::GetAttribute(ac, "health");
        if (health.id != "empty") {
            int sizeX = 80;
            int sizeXh = 40;
            raylib::DrawRectangleLines(healthBarPos.x - sizeXh - 1, healthBarPos.y - 1, sizeX + 2, 12, raylib::DARKGRAY);
            raylib::DrawRectangle(healthBarPos.x - sizeXh, healthBarPos.y, sizeX, 10, raylib::GRAY);
            raylib::DrawRectangle(healthBarPos.x - sizeXh, healthBarPos.y, sizeX * health.get()/health.max, 10, raylib::GREEN);
            raylib::DrawTextEx(font, "Username", raylib::Vector2{healthBarPos.x - sizeXh - 1, healthBarPos.y - 16 + 1}, 13, 1, raylib::DARKGRAY);
            raylib::DrawTextEx(font, "Username", raylib::Vector2{healthBarPos.x - sizeXh + 1, healthBarPos.y - 16 - 1}, 13, 1, raylib::DARKGRAY);
            raylib::DrawTextEx(font, "Username", raylib::Vector2{healthBarPos.x - sizeXh, healthBarPos.y - 16}, 13, 1, raylib::WHITE);
        }
    }
}

}
