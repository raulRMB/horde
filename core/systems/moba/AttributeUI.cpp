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
    for(const entt::entity& entity : Game::GetRegistry().view<CAttributes, CTransform>())
    {
        CTransform& t = registry.get<CTransform>(entity);
        raylib::Vector2 healthBarPos = GetWorldToScreen(
                (raylib::Vector3){ t.Position.x, t.Position.y + 12.0f, t.Position.z },
                ToRaylibCamera(Game::Instance().GetActiveCamera()));
        CAttributes& ac = registry.get<CAttributes>(entity);
        FAttribute& health = *Util::GetAttribute(ac, "health");
        if (health.id != "empty") {
            int sizeX = 80;
            int sizeXh = 40;
            raylib::DrawRectangle(healthBarPos.x - sizeXh, healthBarPos.y, sizeX, 10, raylib::GRAY);
            raylib::DrawRectangle(healthBarPos.x - sizeXh, healthBarPos.y, sizeX * health.get()/health.max, 10, raylib::GREEN);
            raylib::DrawTextEx(font, "Username", raylib::Vector2{healthBarPos.x - sizeXh, healthBarPos.y - 13}, 13, 1, raylib::WHITE);
        }
    }
}

}
