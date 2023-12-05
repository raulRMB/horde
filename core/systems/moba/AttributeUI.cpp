#include "AttributeUI.h"

#include "entt/entt.hpp"
#include "raylib.h"
#include "components/Attribute.h"
#include "util/Util.h"

SAttributeUI::SAttributeUI() {
    font = LoadFont("../assets/ui/fonts/Roboto-Bold.ttf");
}

void SAttributeUI::Draw2D()
{
    entt::registry& registry = Game::GetRegistry();
    for(const entt::entity& entity : Game::GetRegistry().view<CAttributes>())
    {
        Transform& t = registry.get<Transform>(entity);
        Vector2 healthBarPos = GetWorldToScreen(
                (Vector3){ t.translation.x, t.translation.y + 10.0f, t.translation.z },
                Game::Instance().GetActiveCamera());
        CAttributes& ac = registry.get<CAttributes>(entity);
        FAttribute& health = *Util::GetAttribute(ac, "health");
        if (health.id != "empty") {
            DrawRectangle(healthBarPos.x, healthBarPos.y, 80, 10, GRAY);
            DrawRectangle(healthBarPos.x, healthBarPos.y, 80 * health.get()/health.max, 10, GREEN);
            DrawTextEx(font, "Username", Vector2{healthBarPos.x, healthBarPos.y - 13}, 13, 1, WHITE);
        }
    }
}
