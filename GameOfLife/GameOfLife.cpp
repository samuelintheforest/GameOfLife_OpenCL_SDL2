



//#include <CL/opencl.hpp>
#include <string>
#include <iostream>
#include <thread>
#include "Kernel.hpp"
#include "ImageKernel.hpp"
#include "MultMatrix3x3Kernel.hpp"


/// <summary>
/// main program
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
int main(int argc, char* argv[])
{

    game_of_life::Kernel::SetUpOpenCL();

    game_of_life::ImageKernel img("I am an Imae Kernel, Yayy;");
    game_of_life::MultMatrix3x3Kernel m_matrix("I am a Multi Matrix3x3 Kernel, UFF");


    //get all platforms (drivers)
//    std::vector<cl::Platform> all_platforms;
//    cl::Platform::get(&all_platforms);
//    if (all_platforms.size() == 0) {
//        std::cout << " No platforms found. Check OpenCL installation!\n";
//        exit(1);
//    }
//    cl::Platform default_platform = all_platforms[0];
//    std::cout << "Using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>() << "\n";
//
//    //get default device of the default platform
//    std::vector<cl::Device> all_devices;
//    default_platform.getDevices(CL_DEVICE_TYPE_GPU, &all_devices);
//    if (all_devices.size() == 0) {
//        std::cout << " No devices found. Check OpenCL installation!\n";
//        exit(1);
//    }
//    cl::Device default_device = all_devices[0];
//    std::cout << "Using device: " << default_device.getInfo<CL_DEVICE_NAME>() << "\n";
//
//    cl::Context context({ default_device });
//    cl::Program::Sources sources;
//
//    std::string kernel_code = R"(   
//void kernel simple_add(global const int* A, global const int* B, global int* C)
//{
//    C[get_global_id(0)] = A[get_global_id(0)] * B[get_global_id(0)] * 54;
//}
//)";
//
//    sources.push_back({ kernel_code.c_str(),kernel_code.length() });
//
//    cl::Program program(context, sources);
//    if (program.build({ default_device }, "-cl-std=CL2.0") != CL_SUCCESS) {
//        std::cout << " Error building: " << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device) << "\n";
//        exit(1);
//    }
//
//    // create buffers on the device
//    cl::Buffer buffer_A(context, CL_MEM_READ_WRITE, sizeof(int) * 10);
//    cl::Buffer buffer_B(context, CL_MEM_READ_WRITE, sizeof(int) * 10);
//    cl::Buffer buffer_C(context, CL_MEM_READ_WRITE, sizeof(int) * 10);
// 

//
//    int A[] = { 0, 13213, 2, 3, 4, 5, 6, 7, 8, 9 };
//    int B[] = { 0, 1, 2, 0, 1, 2, 0, 1, 2, 0 };
//
//
//    //create queue to which we will push commands for the device.
//    cl::CommandQueue queue(context, default_device);
//    //write arrays A and B to the device
//    queue.enqueueWriteBuffer(buffer_A, CL_TRUE, 0, sizeof(int) * 10, A);
//    queue.enqueueWriteBuffer(buffer_B, CL_TRUE, 0, sizeof(int) * 10, B);
//
//    cl::Kernel simple_add(program, "simple_add");
//    simple_add.setArg(0, buffer_A);
//    simple_add.setArg(1, buffer_B);
//    simple_add.setArg(2, buffer_C);
//    queue.enqueueNDRangeKernel(simple_add, cl::NullRange, cl::NDRange(10), cl::NullRange);
//    queue.finish();
//
//    int C[10];
//    //read result C from the device to array C
//    queue.enqueueReadBuffer(buffer_C, CL_TRUE, 0, sizeof(int) * 10, C);
//    queue.finish();
//
//    
//
//    std::cout << " result: \n";
//    for (int i = 0; i < 10; i++) {
//        std::cout << C[i] << " ";
//    }

    return 0;
}