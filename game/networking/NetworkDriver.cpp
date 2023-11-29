#include "NetworkDriver.h"
#include "components/Follow.h"

NetworkDriver::NetworkDriver(){}

NetworkDriver& NetworkDriver::Instance()
{
    static NetworkDriver instance;
    return instance;
}

void NetworkDriver::SetIsServer(bool isServer) {
    Instance().isServer = isServer;
}

void NetworkDriver::Init() {
    if(NetworkDriver::IsServer()) {
        server = new Server();
        std::thread networkThread([]()
        {
            while(1)
            {
                NetworkDriver::GetServer()->Loop();
            }
        });
        networkThread.detach();
    } else {
        client = new Client();
        std::thread networkThread([]()
        {
            while(1)
            {
                NetworkDriver::GetClient()->Loop();
            }
        });
        networkThread.detach();
    }
}

void NetworkDriver::ProcessQueues() {
    while(!GetInboundQueue().empty()) {
        enet_uint8 *data = GetInboundQueue().front();
        ENetMsg type = *(ENetMsg *) data;
        if(IsServer()) {
            Instance().server->OnInboundMessage(type, data);
        } else {
            Instance().client->OnInboundMessage(type, data);
        }
        GetInboundQueue().pop();
    }
    while(!GetOutboundQueue().empty()) {
        OutboundMessage som = GetOutboundQueue().front();
        if(IsServer()) {
            Instance().server->SendOutboundMessage(som);
        } else {
            Instance().client->SendOutboundMessage(som.Packet);
        }
        GetOutboundQueue().pop();
    }
}

NetworkedEntities& NetworkDriver::GetNetworkedEntities() {
    return Instance().networkedEntities;
}

Server* NetworkDriver::GetServer() {
    return Instance().server;
}

Client* NetworkDriver::GetClient() {
    return Instance().client;
}

bool NetworkDriver::IsServer() {
    return Instance().isServer;
}

bool NetworkDriver::IsOfflineMode() {
    return Instance().offlineMode;
}