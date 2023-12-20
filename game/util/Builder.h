#ifndef HORDE_BUILDER_H
#define HORDE_BUILDER_H

#include <entt/entt.hpp>
#include "components/CTransform.h"

namespace tX {
    class Builder {
        public:
            static entt::entity Player(CTransform& t);
            static entt::entity Spawner();
            static entt::entity Map();
            static entt::entity Enemy(CTransform& t);
    };
}

#endif //HORDE_BUILDER_H
