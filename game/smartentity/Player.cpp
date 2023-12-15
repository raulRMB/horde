#include "Player.h"
#include "components/Follow.h"
#include "util/Util.h"
#include "systems/moba/Navigation.h"
#include "components/Model.h"
#include "networking/base/NetworkDriver.h"
#include "components/Network.h"
#include "components/Physics.h"
#include "components/Attribute.h"
#include "components/Shapes.h"
#include "components/RayCollision.h"
#include "components/Animation.h"
#include "components/CharacterAnimation.h"
#include "components/Mortal.h"
#include "systems/SEnemy.h"

namespace tZ
{
static raylib::Font font;

Player::Player()
{
}

Player::~Player() = default;

void Player::Start()
{
    System::Get<SEnemy>().SetPlayer(GetEntity());
}

void Player::HandleInput(entt::registry* Registry)
{
    if(raylib::IsMouseButtonDown(raylib::MOUSE_BUTTON_RIGHT)) {
        v2 mousePos = Util::GetMouseWorldPosition2D();
        if(System::Get<SNavigation>().IsValidPoint(mousePos))
        {
            if(!NetworkDriver::IsOfflineMode()) {
                const u32 NetId = NetworkDriver::GetNetworkedEntities().Get(GetEntity());
                NetworkDriver::GetClient()->SendMoveTo(mousePos, NetId);
            }
        }
    }
    if(raylib::IsMouseButtonPressed(raylib::MOUSE_BUTTON_RIGHT))
    {
        auto e = Registry->create();
        CMoveCircle mc = CMoveCircle {};
        mc.Position = Util::GetMouseWorldPosition();
        mc.Radius = 2.2;

        Registry->emplace<CMoveCircle>(e, mc);
        v2 mousePos = Util::GetMouseWorldPosition2D();
        if(System::Get<SNavigation>().IsValidPoint(mousePos))
        {
            if(!NetworkDriver::IsOfflineMode()) {
                auto NetId = NetworkDriver::GetNetworkedEntities().Get(GetEntity());
                NetworkDriver::GetClient()->SendMoveTo(mousePos, NetId);
            }
        }
    }
    if(raylib::IsKeyPressed(raylib::KEY_Q))
    {
        if(!Game::IsServer()) {
            FRayCollision Collision = Util::GetMouseCollision();
            auto vec = v3(Collision.point.x, 0.0f, Collision.point.z);
            auto netId = NetworkDriver::GetNetworkedEntities().Get(GetEntity());
            NetworkDriver::GetClient()->TriggerAbility(netId, 0, vec);
//            CTransform clickPoint = CTransform{vec};
//            CTransform t = GetComponent<CTransform>();
//            Projectile(GetEntity(), clickPoint.Position, t.Position);
        }
    }
    if(raylib::IsKeyPressed(raylib::KEY_W))
    {
    }
    if(raylib::IsKeyPressed(raylib::KEY_E))
    {
    }
    if(raylib::IsKeyPressed(raylib::KEY_R))
    {
    }
}

void Player::DrawUI() {}

void Player::Kill()
{
    SmartEntity::Kill();
}

void Player::SetTransform(CTransform &t) {
    CTransform& x = GetComponent<CTransform>();
    CNetwork& n = GetComponent<CNetwork>();
    n.TargetTransform = t;
    x.Position = t.Position;
    x.Scale = t.Scale;
    x.Rotation = t.Rotation;
}

}