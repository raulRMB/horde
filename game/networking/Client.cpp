#include <cstring>
#include <enet/enet.h>
#include "Client.h"
#include "Game.h"
#include "networking/base/NetworkDriver.h"
#include "networking/base/NetMessage.h"
#include "abilities/Projectile.h"
#include "networking/buffers/Events_generated.h"
#include "networking/buffers/FlatBufferUtil.h"
#include "components/CAttribute.h"
#include "components/CCharacterAnimation.h"
#include "util/Util.h"
#include "components/CTransform.h"
#include "components/CNetwork.h"
#include "util/Builder.h"

namespace raylib
{
#include "raylib.h"
}

namespace tX
{

void Client::Connect() {
    if(Game::IsClient()) {
        if (enet_initialize() != 0) {
            raylib::TraceLog(raylib::LOG_ERROR, "Failed to initialize ENet");
        }
        client = enet_host_create(nullptr, 1, 2, 0, 0);
        client->maximumPacketSize = 64 * 1024 * 1024;
        client->maximumWaitingData = 64 * 1024 * 1024;
        enet_peer_timeout(client->peers, ENET_PEER_TIMEOUT_LIMIT, ENET_PEER_TIMEOUT_MINIMUM, ENET_PEER_TIMEOUT_MAXIMUM);
        if (client == nullptr) {
            raylib::TraceLog(raylib::LOG_ERROR, "Failed to create server");
            enet_deinitialize();
        }
        ENetAddress address;
        enet_address_set_host(&address, "localhost");
        address.port = 7777;
        peer = enet_host_connect(client, &address, 2, 0);
        if (peer == nullptr) {
            raylib::TraceLog(raylib::LOG_ERROR, "No available peers for initiating an ENet connection");
            enet_host_destroy(client);
            enet_deinitialize();
        }

        ENetEvent event;
        if(enet_host_service(client, &event, 5000) > 0) {
            if (event.type == ENET_EVENT_TYPE_CONNECT) {
                connected = true;
                SendInitialConnection();
                raylib::TraceLog(raylib::LOG_INFO, "Client Running");
            }
        }
    }
}

Client::Client() {
    Connect();
}

void Client::SendMoveTo(v2 pos, uint32_t NetworkId) {
    flatbuffers::FlatBufferBuilder builder;
    auto vec = Net::CreateVector2(builder, pos.x, pos.y);
    Net::OnMoveToBuilder mtbuilder(builder);
    mtbuilder.add_netId(NetworkId);
    mtbuilder.add_pos(vec);
    auto moveto = mtbuilder.Finish();
    Send(builder, Net::Events_OnMoveTo, moveto.Union());
}

void Client::TriggerAbility(u32 networkId, int abilityId, v3 targeting)
{
    flatbuffers::FlatBufferBuilder builder;
    auto v = FlatBufferUtil::CreateVector3(builder, targeting);
    flatbuffers::Offset<Net::TriggerAbility> trigger = Net::CreateTriggerAbility(builder, networkId, abilityId, v);
    Send(builder, Net::Events_TriggerAbility, trigger.Union());
}

void Client::Send(flatbuffers::FlatBufferBuilder &builder, Net::Events type, flatbuffers::Offset<> data) {
    auto header = CreateHeader(builder, type, data);
    builder.Finish(header);
    size_t size = builder.GetSize();
    ENetPacket* packet = enet_packet_create(builder.GetBufferPointer(), size, ENET_PACKET_FLAG_RELIABLE);
    OutboundMessage msg = OutboundMessage{};
    msg.Packet = packet;
    msg.Connections.push_back(peer);
    NetworkDriver::GetOutboundQueue().push(msg);
}

void Client::SendInitialConnection() {
    flatbuffers::FlatBufferBuilder builder;
    auto sec = builder.CreateString("YOOO");
    Net::OnConnectionBuilder cbuilder(builder);
    cbuilder.add_secret(sec);
    if(NetworkDriver::GetNetworkedEntities().Exists(Game::GetPlayer())) {
        cbuilder.add_netId(NetworkDriver::GetNetworkedEntities().Get(Game::GetPlayer()));
    } else {
        cbuilder.add_netId(-1);
    }
    auto d = cbuilder.Finish();
    Send(builder, Net::Events::Events_OnConnection, d.Union());
}

void Client::Loop() {
    if (client == nullptr || !connected) {
        return;
    }
    ENetEvent event;
    if (enet_host_service(client, &event, 0) > 0) {
        if (event.type == ENET_EVENT_TYPE_CONNECT) {
            connected = true;
            raylib::TraceLog(raylib::LOG_INFO, "CONNECT!");
            SendInitialConnection();
        } else if (event.type == ENET_EVENT_TYPE_RECEIVE) {
            IncomingMessage msg;
            enet_uint8 *dataCopy = new enet_uint8[event.packet->dataLength];
            std::memcpy(dataCopy, event.packet->data, event.packet->dataLength);
            msg.data = dataCopy;
            msg.peer = event.peer;
            NetworkDriver::GetInboundQueue().push(msg);
            enet_packet_destroy(event.packet);
        } else if (event.type == ENET_EVENT_TYPE_DISCONNECT) {
            connected = false;
            raylib::TraceLog(raylib::LOG_INFO, "DISCONNECTED!");
            //Connect();
        }
    }
}

int gotPackets = 0;
void Client::OnInboundMessage(const Net::Header* header) {
    gotPackets++;

    Net::Events event = Net::Events::Events_NONE;
    try {
        event = header->Event_type();
    } catch(int s) {}
    switch(event) {
        case Net::Events_OnConnectionResponse: {
            auto res = header->Event_as_OnConnectionResponse();
            auto t = FlatBufferUtil::NetTransformToTransform(res->self()->location());
            Game::SpawnPlayer(res->self()->netId(), t, true);
            std::vector stdVector(res->others()->begin(), res->others()->end());
            for(auto otherPlayer : stdVector) {
                auto loc = FlatBufferUtil::NetTransformToTransform(otherPlayer->location());
                Game::SpawnPlayer(otherPlayer->netId(), loc, false);
            }
            break;
        }
        case Net::Events_SyncTransform: {
            auto res = header->Event_as_SyncTransform();
            if(NetworkDriver::GetNetworkedEntities().Exists(res->netId())) {
                CNetwork* t = Game::GetRegistry().try_get<CNetwork>(
                        NetworkDriver::GetNetworkedEntities().Get(res->netId()));
                //printf("Sync %u to pos %f, %f, %f\n", res->netId(), res->transform()->translation()->x(), res->transform()->translation()->y(), res->transform()->translation()->z());
                if(t != nullptr) {
                    t->TargetTransform = FlatBufferUtil::NetTransformToTransform(res->transform());
                }
            }
            break;
        }
        case Net::Events_BatchSyncTransform: {
            auto res = header->Event_as_BatchSyncTransform();
            std::vector entities(res->entities()->begin(), res->entities()->end());
            for(auto entity : entities) {
                if(NetworkDriver::GetNetworkedEntities().Exists(entity->netId())) {
                    CNetwork* t = Game::GetRegistry().try_get<CNetwork>(
                            NetworkDriver::GetNetworkedEntities().Get(entity->netId()));
                    if(t != nullptr) {
                        t->TargetTransform = FlatBufferUtil::NetTransformToTransform(entity->transform());
                    }
                }
            }
            break;
        }
        case Net::Events_SyncAttributeComponent: {
            const Net::SyncAttributeComponent* res = header->Event_as_SyncAttributeComponent();
            auto netid = res->netId();
            if(NetworkDriver::GetNetworkedEntities().Exists(netid)) {
                CAttributeSet& ac = Game::GetRegistry().get<CAttributeSet>(NetworkDriver::GetNetworkedEntities().Get(netid));
                std::vector stdVector(res->attributes()->begin(), res->attributes()->end());
                for(auto a : stdVector) {
                    FAttribute* attr = Util::GetAttribute(ac, a->name()->str());
                    if(attr != nullptr) {
                        attr->base = a->value();
                        attr->max = a->max();
                    }
                }
            }
            break;
        }
        case Net::Events_OnPlayerJoined: {
            const Net::OnPlayerJoined* res = header->Event_as_OnPlayerJoined();
            CTransform t = FlatBufferUtil::NetTransformToTransform(res->transform());
            Game::SpawnPlayer(res->netId(), t, false);
            break;
        }
        case Net::Events_SyncCharacterAnimState :
        {
            const Net::SyncCharacterAnimState* res = header->Event_as_SyncCharacterAnimState();
            if(NetworkDriver::GetNetworkedEntities().Exists(res->netId())) {
                CCharacterAnimation &ca = Game::GetRegistry().get<CCharacterAnimation>(
                        NetworkDriver::GetNetworkedEntities().Get(res->netId()));
                ca.AnimState = static_cast<ECharacterAnimState>(res->state());
            }
            break;
        }
        case Net::Events_SpawnEntity:
        {
            const Net::SpawnEntity* res = header->Event_as_SpawnEntity();
            if(res->entityType() == 1) {
                CTransform t = FlatBufferUtil::NetTransformToTransform(res->location());
                entt::entity e = Builder::Enemy(t);
                NetworkDriver::GetNetworkedEntities().Add(e, res->netid());
            }
            break;
        }
        case Net::Events_SpawnProjectile:
        {
            const Net::SpawnProjectile* res = header->Event_as_SpawnProjectile();
            const v2 pos(res->position()->x(), res->position()->y());
            const v2 dir(res->direction()->x(), res->direction()->y());
            const float speed(res->speed());
            const float lifetime(res->lifetime());
            SpawnProjectile(NetworkDriver::GetNetworkedEntities().Get(res->netid()), pos, dir, speed, lifetime);
            break;
        }
    }
}

void Client::SendOutboundMessage(ENetPacket* packet) {
    if(connected) {
        enet_peer_send(peer, 0, packet);
    }
}

void Client::Close() {
    enet_host_destroy(client);
    enet_deinitialize();
}

}