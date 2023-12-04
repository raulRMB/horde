#include "NetworkDriver.h"

#include <thread>

#include "components/Follow.h"

namespace tZ
{

NetworkDriver::NetworkDriver(){}

NetworkDriver& NetworkDriver::Instance()
{
    static NetworkDriver instance;
    return instance;
}

void NetworkDriver::SetIsServer(bool isServer) {
    Instance().isServer = isServer;
}

void NetworkDriver::Init(long long periodMicroseconds) {
    if(NetworkDriver::IsServer()) {
        server = new Server();
        std::thread networkThread([periodMicroseconds]()
        {
            
            while(1)
            {
//                auto startTime = std::chrono::high_resolution_clock::now();
                NetworkDriver::GetServer()->Loop();
//                auto endTime = std::chrono::high_resolution_clock::now();
//                auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
//                auto sleepTime = periodMicroseconds - elapsedTime;
//                if (sleepTime > 0) {
//                    std::this_thread::sleep_for(std::chrono::microseconds(sleepTime));
//                }
            }
        });
        networkThread.detach();
    } else {
        client = new Client();
        std::thread networkThread([periodMicroseconds]()
        {
            while(1)
            {
//                auto startTime = std::chrono::high_resolution_clock::now();
                NetworkDriver::GetClient()->Loop();
//                auto endTime = std::chrono::high_resolution_clock::now();
//                auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
//                auto sleepTime = periodMicroseconds - elapsedTime;
//                if (sleepTime > 0) {
//                    std::this_thread::sleep_for(std::chrono::microseconds(sleepTime));
//                }
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
        delete data;
        GetInboundQueue().pop();
    }
    while(!GetOutboundQueue().empty()) {
        NetOutboundMessage som = GetOutboundQueue().front();
        if(IsServer()) {
            Instance().server->SendOutboundMessage(som);
        } else {
            Instance().client->SendOutboundMessage(som.Packet);
        }
        GetOutboundQueue().pop();
    }
    if(IsServer()) {
        Instance().server->flush();
    } else {
        Instance().client->flush();
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
}