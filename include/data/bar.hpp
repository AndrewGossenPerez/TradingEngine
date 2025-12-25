
#pragma once 
#include "core/types.hpp"

namespace trd{

    struct Bar{
        timestamp epoch; // GMT + 12:00
        price open,high,low,close;
        quantity volume;
    };

    void printBar(const Bar& bar);

} 