#ifndef HORDE_CMETADATA_H
#define HORDE_CMETADATA_H

#include "components/tX_Component.h"
#include "util/defines.h"
#include <set>
#include <string>

namespace tX
{

struct CMetaData : CComponent
{
    std::set<std::string> Tags;
};

}

#endif //HORDE_CMETADATA_H
