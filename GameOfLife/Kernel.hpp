#pragma once

#include <iostream>
#include <string>
#define CL_HPP_ENABLE_EXCEPTIONS
#define CL_HPP_TARGET_OPENCL_VERSION 210
#include <CL/opencl.hpp>

namespace game_of_life
{
	class Kernel
	{
	public:
		static void SetUpOpenCL();
		// Constructors and Destructors
		Kernel(const std::string& src);
		~Kernel();

	protected:
		static int setupFunctionCounter;
		static std::vector<cl::Platform> s_AllPlatforms;
		static cl::Platform s_DefaultPlatform;
		static std::vector<cl::Device> s_AllDevices;
		static cl::Device s_DefaultDevice;
		cl::Context m_KernelContext;
		virtual void CompileKernels() = 0;
		std::string m_ProgramSrc;
	};

}