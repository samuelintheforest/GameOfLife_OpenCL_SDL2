#include "Kernel.hpp"

// ################################################
// ######      Init the static variables     ######
// ################################################
int game_of_life::Kernel::setupFunctionCounter = 0;
std::vector<cl::Platform> game_of_life::Kernel::s_AllPlatforms;
cl::Platform game_of_life::Kernel::s_DefaultPlatform;
std::vector<cl::Device> game_of_life::Kernel::s_AllDevices;
cl::Device game_of_life::Kernel::s_DefaultDevice;


// ################################################
// ######            Setting up OpenCL       ######
// ################################################
void game_of_life::Kernel::SetUpOpenCL()
{
    if (setupFunctionCounter == 0)
    {
        setupFunctionCounter++;

        // 1. Platform setup
        cl::Platform::get(&game_of_life::Kernel::s_AllPlatforms);
        if (game_of_life::Kernel::s_AllPlatforms.size() == 0) {
            std::cout << " No platforms found. Check OpenCL installation!\n";
            exit(1);
        }

        Kernel::s_DefaultPlatform = Kernel::s_AllPlatforms[0];
        std::cout << "Using platform: " << Kernel::s_DefaultPlatform.getInfo<CL_PLATFORM_NAME>() << "\n";

        // 2. Device setup
        Kernel::s_DefaultPlatform.getDevices(CL_DEVICE_TYPE_GPU, &Kernel::s_AllDevices);
        if (Kernel::s_AllDevices.size() == 0) {
            std::cout << " No devices found. Check OpenCL installation!\n";
            exit(1);
        }

        Kernel::s_DefaultDevice = Kernel::s_AllDevices[0];
        std::cout << "Using device: " << Kernel::s_DefaultDevice.getInfo<CL_DEVICE_NAME>() << "\n";
    }
}

game_of_life::Kernel::Kernel(const std::string& src)
{
    if (setupFunctionCounter == 0)
    {
        std::cout << "Error: First call SetUpOpenCL()\n";
        exit(-1);

    }

    // Create it's context
    m_KernelContext = { Kernel::s_DefaultDevice };
    m_ProgramSrc = src;
    std::cout << this << "\n" << m_ProgramSrc << "\n";
    
}

game_of_life::Kernel::~Kernel()
{

}

