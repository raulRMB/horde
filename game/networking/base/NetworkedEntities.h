#ifndef HORDE_CONNECTIONS_H
#define HORDE_CONNECTIONS_H

#include "entt/entt.hpp"
#include "raylib.h"
#include <unordered_map>

class NetworkedEntities {
    std::unordered_map<uint32_t, entt::entity> x;
    std::unordered_map<entt::entity, uint32_t> y;

public:
    uint32_t Add(entt::entity e) {
        uint32_t netId = GetRandomValue(1, 10000);
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

    uint32_t Get(entt::entity e) {
        return y[e];
    }

    entt::entity Get(uint32_t netId) {
        return x[netId];
    }

};


#endif //HORDE_CONNECTIONS_H
