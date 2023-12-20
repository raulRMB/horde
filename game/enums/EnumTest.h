//
// Created by Raul Romero on 2023-12-19.
//

#ifndef HORDE_ENUMTEST_H
#define HORDE_ENUMTEST_H

#include "util/tX_Enum.h"

namespace tX
{
    enum ETest : u8
    {
        Test1 = 0,
        Test2,
        Test3,
        Test4
    };
    Enum<ETest> TestEnum;
}


#endif //HORDE_ENUMTEST_H
