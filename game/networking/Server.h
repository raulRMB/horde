#ifndef SERVER_H
#define SERVER_H

#include <enet/enet.h>
#include <entt/entt.hpp>
#include <vector>
#include "util/defines.h"
#include "networking/base/NetMessage.h"
#include "buffers/Events_generated.h"

namespace tX
{

struct CTransform;

class Server {
public:
    ENetHost* server;
    std::vector<entt::entity> players;
    flatbuffers::FlatBufferBuilder builder;
    Server();
    void Loop();
    void Close();

    void OnInboundMessage(const Net::Header* header, ENetPeer* peer);
    void SendConnectResponse(ENetPeer* peer, uint32_t netId);
    void OnConnect(ENetPeer* peer);
    void SendOutboundMessage(OutboundMessage msg);
    void SendPlayerJoined(u32 netId, CTransform& t);
    void SendSpawnProjectile(u32 netId, v2 pos, v2 dir, float speed, float lifetime);
    void flush() { enet_host_flush(server); }
    void Sync(entt::entity e, CTransform& t, std::vector<ENetPeer*> c);
    void Sync(entt::entity e, struct CAttributeSet& ac, std::vector<ENetPeer*> c);
    void Sync(entt::entity e, struct CCharacterAnimation& ca, std::vector<ENetPeer*> c);
    void SendSync(std::vector<ENetPeer*> c);
    void SendSpawnEntity(uint32_t netId, uint32_t entityType, CTransform& t);
private:
    void Send(flatbuffers::FlatBufferBuilder &builder, Net::Events type, flatbuffers::Offset<> data, std::vector<ENetPeer*>& c,
              ENetPacketFlag flag);
};

}

#endif