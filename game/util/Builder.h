#ifndef HORDE_BUILDER_H
#define HORDE_BUILDER_H

#include <entt/entt.hpp>
#include "components/Transform.h"

namespace tZ {
    class Builder {
        public:
            static entt::entity Player(CTransform& t);
    };
}

#endif //HORDE_BUILDER_H
