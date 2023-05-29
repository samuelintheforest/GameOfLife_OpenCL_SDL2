#pragma once
#include "Kernel.hpp"

namespace game_of_life
{
    class MultMatrix3x3Kernel : public Kernel
    {
    public:
        MultMatrix3x3Kernel(const std::string& source);
        ~MultMatrix3x3Kernel();

    private:
        void CompileKernels();
    };
}
