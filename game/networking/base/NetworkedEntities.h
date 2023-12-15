#ifndef HORDE_CONNECTIONS_H
#define HORDE_CONNECTIONS_H

#include <entt/entt.hpp>
#include <enet/enet.h>
#include <unordered_map>
#include <cstdlib>
#include <random>

class NetworkedEntities {
    std::unordered_map<uint32_t, entt::entity> x;
    std::unordered_map<entt::entity, uint32_t> y;

    std::unordered_map<ENetPeer*, uint32_t> ownedEntities;
    std::unordered_map<uint32_t, ENetPeer*> ownedEntitiesRev;

public:

    uint32_t Add(entt::entity e, ENetPeer* owner) {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> randint(0, UINT32_MAX);
        uint32_t netId = randint(rng);

        auto it = y.find(e);
        if (it == y.end()) {
            x.insert(std::pair<uint32_t, entt::entity>(netId, e));
            y.insert(std::pair<entt::entity, uint32_t>(e, netId));
//            ownedEntities.insert(std::pair<ENetPeer*, uint32_t>(owner, netId));
//            ownedEntitiesRev.insert(std::pair<uint32_t, ENetPeer*>(netId, owner));
            return netId;
        }
        return 0;
    }

    uint32_t Add(entt::entity e) {
        std::random_device dev;
        std::mt19937 rng(dev());
        std::uniform_int_distribution<std::mt19937::result_type> randint(0, UINT32_MAX);
        uint32_t netId = randint(rng);

        auto it = y.find(e);
        if (it == y.end()) {
            x.insert(std::pair<uint32_t, entt::entity>(netId, e));
            y.insert(std::pair<entt::entity, uint32_t>(e, netId));
            return netId;
        }
        return 0;
    }

    uint32_t Add(entt::entity e, uint32_t netId) {
        auto it = y.find(e);
        if (it == y.end()) {
            x.insert(std::pair<uint32_t, entt::entity>(netId, e));
            y.insert(std::pair<entt::entity, uint32_t>(e, netId));
            return netId;
        }
        return 0;
    }

    void Remove(uint32_t netId) {
        entt::entity tmp = x[netId];
        x.erase(netId);
        y.erase(tmp);
    }

    void Remove(entt::entity e) {
        uint32_t tmp = y[e];
        y.erase(e);
        x.erase(tmp);
    }

    bool Exists(entt::entity e) {
        auto it = y.find(e);
        return it == y.end();
    }

    bool Exists(uint32_t netId) {
        auto it = x.find(netId);
        return it == x.end();
    }

    uint32_t Get(entt::entity e) {
        return y[e];
    }

    entt::entity Get(uint32_t netId) {
        return x[netId];
    }

    void SetOwner(uint32_t netId, ENetPeer* peer) {
        ownedEntities.insert(std::pair<ENetPeer*, uint32_t>(peer, netId));
        ownedEntitiesRev.insert(std::pair<uint32_t, ENetPeer*>(netId, peer));
    }

    ENetPeer* GetOwner(uint32_t netId) {
        return ownedEntitiesRev[netId];
    }

    uint32_t GetOwner(ENetPeer* peer) {
        return ownedEntities[peer];
    }

};


#endif //HORDE_CONNECTIONS_H
