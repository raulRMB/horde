#include <enet/enet.h>
#include <cstring>
#include "raylib.h"
#include "Client.h"
#include "Game.h"
#include "NetworkDriver.h"
#include "NetMessage.h"
#include "networking/buffers/Events_generated.h"
#include "networking/buffers/FlatBufferUtil.h"

Client::Client() {
    if (enet_initialize() != 0) {
        TraceLog(LOG_INFO, "Failed to initialize ENet");
    }
    client = enet_host_create(nullptr, 1, 2, 0, 0);
    if (client == nullptr) {
        TraceLog(LOG_INFO, "Failed to create server");
        enet_deinitialize();
    }
    ENetAddress address;
    enet_address_set_host(&address, "localhost");
    address.port = 7777;
    peer = enet_host_connect(client, &address, 2, 0);
    if (peer == nullptr) {
        TraceLog(LOG_INFO, "No available peers for initiating an ENet connection");
        enet_host_destroy(client);
        enet_deinitialize();
    }
    TraceLog(LOG_INFO, "Client Running");
}

void Client::SendMoveTo(Vector2 pos, u_int32_t NetworkId) {
    flatbuffers::FlatBufferBuilder builder;
    auto vec = Net::CreateVector2(builder, pos.x, pos.y);
    Net::OnMoveToBuilder mtbuilder(builder);
    mtbuilder.add_netId(NetworkId);
    mtbuilder.add_pos(vec);
    auto moveto = mtbuilder.Finish();
    Send(builder, Net::Events_OnMoveTo, moveto.Union());
}

void Client::Send(flatbuffers::FlatBufferBuilder &builder, Net::Events type, flatbuffers::Offset<> data) {
    auto header = CreateHeader(builder, type, data);
    builder.Finish(header);
    ENetPacket* packet = enet_packet_create(builder.GetBufferPointer(), builder.GetSize(), ENET_PACKET_FLAG_RELIABLE);
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
    auto d = cbuilder.Finish();
    Send(builder, Net::Events::Events_OnConnection, d.Union());
}

void Client::Loop() {
    if(client == nullptr) {
        return;
    }
    ENetEvent event;
    if(enet_host_service(client, &event, 2000) > 0)
    {
        if(event.type == ENET_EVENT_TYPE_CONNECT) {
            TraceLog(LOG_INFO, "CONNECT!");
            SendInitialConnection();
        } else if(event.type == ENET_EVENT_TYPE_RECEIVE) {
            IncomingMessage msg;
            enet_uint8* dataCopy = new enet_uint8[event.packet->dataLength];
            std::memcpy(dataCopy, event.packet->data, event.packet->dataLength);
            msg.data = dataCopy;
            msg.peer = event.peer;
            NetworkDriver::GetInboundQueue().push(msg);
            enet_packet_destroy(event.packet);
        } else if(event.type == ENET_EVENT_TYPE_DISCONNECT) {
            TraceLog(LOG_INFO, "DISCONNECT!");
        }
    }
}

int gotPackets = 0;
void Client::OnInboundMessage(const Net::Header* header) {
    gotPackets++;
    switch (header->Event_type()) {
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
            Transform& t = Game::GetRegistry().get<Transform>(NetworkDriver::GetNetworkedEntities().Get(res->netId()));
            t = FlatBufferUtil::NetTransformToTransform(res->transform());
            break;
        }
        case Net::Events_OnPlayerJoined: {
            auto res = header->Event_as_OnPlayerJoined();
            auto t = Transform{};
            Game::SpawnPlayer(res->netId(), t, false);
            break;
        }
    }
}

void Client::SendOutboundMessage(ENetPacket* packet) {
    enet_peer_send(peer, 0, packet);
}

void Client::Close() {
    enet_host_destroy(client);
    enet_deinitialize();
}