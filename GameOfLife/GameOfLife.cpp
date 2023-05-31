



//#include <CL/opencl.hpp>
#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include "Kernel.hpp"
#include "ImageKernel.hpp"
#include "MultMatrix3x3Kernel.hpp"
#include "KernelSources.h"
#include "DrawBoard.hpp"


//typedef struct _cl_image_format {
//    cl_channel_order image_channel_order;
//    cl_channel_type image_channel_data_type;
//} cl_image_format;

/// <summary>
/// main program
/// </summary>
/// <param name="argc"></param>
/// <param name="argv"></param>
/// <returns></returns>
/// 
/// 
int main(int argc, char* argv[])
{

    game_of_life::Kernel::SetUpOpenCL();
    DrawBoard::SetupDrawing();
    DrawBoard::SetPenColor(255, 255, 255, 255);
    DrawBoard::SetBackColor(0, 0, 0, 255);

    game_of_life::ImageKernel img("I am an Imae Kernel, Yayy;");
    //game_of_life::MultMatrix3x3Kernel m_matrix("I am a Multi Matrix3x3 Kernel, UFF");

    //cl_float array[10] = { 1.0f, 20.2f, 32.323f, 543.f, 1.f, 0.02f, 32.1f, 5.4f, 4.f, 54634.43f };
    //cl::Buffer container;
    //try {
    //    container = { img.m_KernelContext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(cl_float) * 10, &array[0] };
    //    std::cout << container.getInfo<CL_MEM_SIZE>() << std::endl;
    //}
    //catch (cl::Error e) {
    //    std::cout << e.what() << ": Error code " << e.err() << std::endl;
    //} 

    //cl_char ff;
    //
    const int img_w = 1920;
    const int img_h = 1080;
    const int img_d = 2;
    constexpr int length = img_w * img_h * img_d;
    ////
    uint8_t* img_array = (uint8_t*)malloc(sizeof(uint8_t) * length);
    ////
    //for (int j = 0; j < 200; j++)
    //{
    //    auto start = std::chrono::high_resolution_clock::now();
    //    for (int i = 0; i < length; i++)
    //    {
    //        img_array[i] = 0;
    //    }
    //    std::cout << "Ellapsed(ms)=" << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << std::endl;

    //}
    
    //cl_image_format gof_img_format = { CL_DEPTH, CL_UNSIGNED_INT8 };
    cl_image_desc gof_img_desc;
    gof_img_desc.image_type = CL_MEM_OBJECT_IMAGE3D;
    gof_img_desc.image_width = img_w;
    gof_img_desc.image_height = img_h;
    gof_img_desc.image_depth = img_d;
    gof_img_desc.image_row_pitch = 0;
    gof_img_desc.image_slice_pitch = 0;
    gof_img_desc.num_mip_levels = 0;
    gof_img_desc.num_samples = 0;
    gof_img_desc.mem_object = NULL;


     
    // Fill up the memory region with 0

    auto start = std::chrono::high_resolution_clock::now();
    for (int idx = 0; idx < length; idx++)
    {
        img_array[idx] = 0;
    }
    std::cout << "Ellapsed(ms)=" << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << std::endl;
    
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(0, img_w * img_h - 1);
    std::uniform_int_distribution<> distrib_c(0, 255);

    for (int soe = 0; soe < 1000000; soe++)
    {
        int i = distrib(gen);
        img_array[i] = 1;
    }

    
    int z_c = 0;
    int o_c = 0;

    for (int idx = 0; idx < length / 2; idx++)
    {
        if (img_array[idx] == 0)
        {
            z_c++;
        }
        else if (img_array[idx] == 1)
        {
            o_c++;
        }

    }
    int example_gen = 1000;

    std::cout << "> Simulation stat : begining (" << example_gen << ")\n";
    std::cout << "N 0-count: " << z_c << ", N 1-count: " << o_c << std::endl;
    
    cl::ImageFormat gof_image_format(CL_R, CL_UNSIGNED_INT8);
    cl::Image3D space;
    cl::Sampler img_sampler;
    cl::Program::Sources sources;
    cl::Program program;
    cl::CommandQueue queue;
    cl::Kernel gof_kernel_cpy;
    cl::Kernel gof_kernel_clear;
    cl::Kernel gof_kernel_solve;
    try
    {
        
        space = { img.m_KernelContext, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR,
            gof_image_format, gof_img_desc.image_width, gof_img_desc.image_height, gof_img_desc.image_depth, 0, 0, &img_array[0]};

        img_sampler = { img.m_KernelContext, CL_FALSE, CL_ADDRESS_CLAMP, CL_FILTER_NEAREST };

        sources.push_back({ game_of_life::gof_kernel.c_str(), game_of_life::gof_kernel.length()});



        program = { img.m_KernelContext, sources };
        program.build({ game_of_life::Kernel::s_DefaultDevice }, "-cl-std=CL2.0");


        std::cout << "Number of kernels: " << program.getInfo<CL_PROGRAM_NUM_KERNELS>() << std::endl;

        queue = { img.m_KernelContext, game_of_life::Kernel::s_DefaultDevice };
        gof_kernel_cpy = { program, "gof_copy" };
        gof_kernel_cpy.setArg(0, space);
        gof_kernel_cpy.setArg(1, img_sampler);

        gof_kernel_clear = { program, "gof_clear" };
        gof_kernel_clear.setArg(0, space);
        gof_kernel_clear.setArg(1, img_sampler);

        gof_kernel_solve = { program, "gof_solve" };
        gof_kernel_solve.setArg(0, space);
        gof_kernel_solve.setArg(1, img_sampler);
        
    }
    catch (const cl::BuildError& be)
    {
        for (auto& val : be.getBuildLog())
        {
            std::cout << "Device: " << val.first.getInfo<CL_DEVICE_NAME>() << ", Problem: " << val.second << ", Error coe: " << be.err() << ", What: " << be.what() << std::endl;
        }
    }
    catch (const cl::Error& e)
    {
        std::cout << e.what() << ": Error code " << e.err() << std::endl;
    } 
    
    bool quit = false;

    try
    {

       

        auto start = std::chrono::high_resolution_clock::now();
        while (!quit)
        {
            DrawBoard::StartDrawing();
            quit = DrawBoard::PollEvents();

            //auto start = std::chrono::high_resolution_clock::now();
            queue.enqueueNDRangeKernel(gof_kernel_solve, cl::NullRange, cl::NDRange(img_w, img_h, img_d));
            queue.finish();
            queue.enqueueNDRangeKernel(gof_kernel_cpy, cl::NullRange, cl::NDRange(img_w, img_h, img_d));
            queue.finish();
            queue.enqueueNDRangeKernel(gof_kernel_clear, cl::NullRange, cl::NDRange(img_w, img_h, img_d));
            queue.finish();

            queue.enqueueReadImage(space, CL_FALSE, { 0, 0, 0 }, { img_w, img_h, img_d }, 0, 0, &img_array[0]);
            queue.finish();

            z_c = 0;
            o_c = 0;

            for (int idx = 0; idx < (length / 2); idx++)
            {
                if (img_array[idx] == 0)
                {
                    z_c++;
                }
                else if (img_array[idx] == 1)
                {
                    o_c++;
                    DrawBoard::SetPenColor(distrib_c(gen), distrib_c(gen), distrib_c(gen), 255);
                    DrawBoard::FillRect(idx % img_w, idx / img_w, 1, 1);
                }
            }
            DrawBoard::StopDrawing();
            //std::cout << "N 0-count: " << z_c << ", N 1-count: " << o_c << std::endl;
        }



        queue.enqueueReadImage(space, CL_FALSE, { 0, 0, 0 }, { img_w, img_h, img_d }, 0, 0, & img_array[0]);
        queue.finish();

        z_c = 0;
        o_c = 0;

        for (int idx = 0; idx < length / 2; idx++)
        {
            if (img_array[idx] == 0)
            {
                z_c++;
            }
            else if (img_array[idx] == 1)
            {
                o_c++;
            }
        }

        std::cout << "> Simulation stats end: \n";
        std::cout << "N 0-count: " << z_c << ", N 1-count: " << o_c << std::endl;
        std::cout << "Ellapsed(ms)=" << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() << std::endl;

    }
    catch (const cl::Error& e)
    {
        std::cout << e.what() << ": Error code " << e.err() << std::endl;
    }


    

//  get all platforms (drivers)
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
//    
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


    DrawBoard::ReleaseDrawing();
    free(img_array);

    std::cout << "Press anything to continue!" << std::endl;
    int buff;
    buff = std::getchar();
    
    return 0;
}


