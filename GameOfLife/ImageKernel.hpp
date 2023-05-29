#pragma once
#include "Kernel.hpp"
#include <string>

namespace game_of_life
{
	class ImageKernel : public Kernel
	{
	public:
		ImageKernel(const std::string& source);
		~ImageKernel();
	private:
		void CompileKernels();
	};
}
