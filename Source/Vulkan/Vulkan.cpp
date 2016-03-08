#include "Vulkan.hpp"

#pragma region Vulkan

#pragma region Exception
BVE::string														BVE::Vulkan::Exception::Get() const
{
	return text;
}
#pragma endregion

#pragma region Func
#pragma endregion
#pragma region Log
std::mutex														BVE::Vulkan::Log::writeMutex;
std::clock_t													BVE::Vulkan::Log::timestampClock;
#pragma endregion
#pragma endregion
























