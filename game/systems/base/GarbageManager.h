#ifndef S_GARBAGEMANAGER_H
#define S_GARBAGEMANAGER_H

#include "systems/System.h"

namespace tZ
{

class SGarbageManager : public UpdateSystem
{
    std::vector<entt::entity> ToDestroy;
public:
    SGarbageManager() = default;
    ~SGarbageManager() override = default;

    void Add(entt::entity e) { ToDestroy.push_back(e); }

    void Update(f32 deltaSeconds) override;
};

}


#endif //S_GARBAGEMANAGER_H
