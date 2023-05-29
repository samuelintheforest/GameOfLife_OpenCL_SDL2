#pragma once

#include <string>

namespace game_of_life {

std::string vector_add_kernel = R"(

kernel void memset(   global uint *dst )
{          
    dst[get_global_id(0)] = get_global_id(0);
}

)";

}