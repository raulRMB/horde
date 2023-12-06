#ifndef SERVER_H
#define SERVER_H

#include <enet/enet.h>
#include <entt/entt.hpp>
#include <vector>
#include "NetMessage.h"
#include "Player.h"
#include "buffers/Events_generated.h"

namespace tZ
{

struct CTransform;

class Server {
public:
    ENetHost* server;
    std::vector<Player*> players;
    Server();
    void Loop();
    void Close();

    void OnInboundMessage(const Net::Header* header, ENetPeer* peer);
    void SendConnectResponse(ENetPeer* peer, uint32_t netId);
    void OnConnect(ENetPeer* peer);
    void SendOutboundMessage(OutboundMessage msg);
    void SendPlayerJoined(uint32_t netId);
    void flush() { enet_host_flush(server); }
    void Sync(entt::entity e, CTransform& t, std::vector<ENetPeer*> c);
    void Sync(entt::entity e, struct CAttributes& ac, std::vector<ENetPeer*> c);
private:
    void Send(flatbuffers::FlatBufferBuilder &builder, Net::Events type, flatbuffers::Offset<> data, std::vector<ENetPeer*>& c);
};

}

#endif