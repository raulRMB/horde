#ifndef HORDE_NETWORK_H
#define HORDE_NETWORK_H

#include <set>
#include <string>

struct CNetwork {
    std::set<std::string> tags;
    Transform prevTransform = Transform{};
};

#endif
