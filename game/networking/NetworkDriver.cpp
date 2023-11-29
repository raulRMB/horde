#include "NetworkDriver.h"
#include "Player.h"

NetworkDriver::NetworkDriver(){

}

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
                                      double netFreq = 120.0;
                                      auto periodMicroseconds = static_cast<long long>(1e6 / netFreq);
                                      while(1)
                                      {
                                          auto startTime = std::chrono::high_resolution_clock::now();
                                          NetworkDriver::GetServer()->Loop();
                                          auto endTime = std::chrono::high_resolution_clock::now();
                                          auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
                                          auto sleepTime = periodMicroseconds - elapsedTime;
                                          if (sleepTime > 0) {
                                              std::this_thread::sleep_for(std::chrono::microseconds(sleepTime));
                                          }
                                      }
                                  });
        networkThread.detach();
    } else {
        client = new Client();
        std::thread networkThread([]()
                                  {
                                      double netFreq = 120.0;
                                      auto periodMicroseconds = static_cast<long long>(1e6 / netFreq);
                                      while(1)
                                      {
                                          auto startTime = std::chrono::high_resolution_clock::now();
                                          NetworkDriver::GetClient()->Loop();
                                          auto endTime = std::chrono::high_resolution_clock::now();
                                          auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();
                                          auto sleepTime = periodMicroseconds - elapsedTime;
                                          if (sleepTime > 0) {
                                              std::this_thread::sleep_for(std::chrono::microseconds(sleepTime));
                                          }
                                      }
                                  });
        networkThread.detach();
    }
}

void NetworkDriver::ProcessQueue() {
    while(!GetNetworkingQueue().empty()) {
        enet_uint8 *data;
        data = GetNetworkingQueue().front();
        ENetMsg type = *(ENetMsg *) data;

        NetMessageTransform msg = *(NetMessageTransform *) data;
        InitialConnection x = *(InitialConnection *) data;

        if(IsServer()) {
            switch (type)
            {
                case ENetMsg::MoveTo:
//                    if(System::Get<SNavigation>().IsValidPoint(msg.pos)) {
//                        auto e = GetNetworkedEntities().Get(msg.NetworkId);
//                        CFollow &followComponent = GetRegistry().get<CFollow>(e);
//                        followComponent.FollowState = EFollowState::Dirty;
//                        followComponent.Index = 1;
//                        TraceLog(LOG_INFO, "x: %f y %f", msg.pos.x, msg.pos.y);
//                        followComponent.Goal = msg.pos;
//                    }
                    break;
                case ENetMsg::InitialConnection:
                    OnConnect(x.peer);
                    break;
                default:
                    TraceLog(LOG_INFO, "DEFAULT");
                    break;
            }
        } else {
            switch (type) {
                case ENetMsg::ConnectionResponse:
                    NetConnectionResponse msg = *(NetConnectionResponse *) data;
                    Game::SpawnPlayer(msg.NetworkId);
                    TraceLog(LOG_INFO, "entt: %d", msg.NetworkId);
                    break;
            }
        }
        GetNetworkingQueue().pop();
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

void NetworkDriver::OnConnect(ENetPeer* peer) {
    Player* player = new Player();
    uint32_t netId = Instance().networkedEntities.Add(player->GetEntity());
    NetworkDriver::GetServer()->ConnectResponse(peer, netId);
}