#ifndef SERVER_H
#define SERVER_H

#include <enet/enet.h>
#include <entt/entt.hpp>
#include <vector>
#include <raylib.h>
#include "NetMessage.h"
#include "Player.h"
#include "buffers/Events_generated.h"

class Server {
public:
    ENetHost* server;
    std::vector<Player*> players;
    Server();
    void Loop();
    void Close();

    void OnInboundMessage(const Net::Header* header, ENetPeer* peer);
    void ConnectResponse(ENetPeer* peer, uint32_t netId);
    void OnConnect(ENetPeer* peer);
    void SendOutboundMessage(OutboundMessage msg);
    void SendPlayerJoined(uint32_t netId);
    void Send(flatbuffers::FlatBufferBuilder &builder, Net::Events type, flatbuffers::Offset<> data, std::vector<ENetPeer*>& c);
    std::vector<int32_t> GetOtherPlayers(ENetPeer* peer);

    flatbuffers::Offset<Net::Vector3> CreateVector3(flatbuffers::FlatBufferBuilder &builder, Vector3 v);
    flatbuffers::Offset<Net::Vector4> CreateVector4(flatbuffers::FlatBufferBuilder &builder, Vector4 v);
    flatbuffers::Offset<Net::Transform> CreateTransform(flatbuffers::FlatBufferBuilder &builder, Transform& t);
    std::vector<ENetPeer*>& CreatePeerVector(ENetPeer* peer);
    void flush() { enet_host_flush(server); }
    void Sync(entt::entity e, Transform& t, std::vector<ENetPeer*> c);
};

#endif