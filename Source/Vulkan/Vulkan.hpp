#pragma once

#pragma region Include
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <cstdint>
#include <string>
#include <ctime>
#include <mutex>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <numeric>

#include "../../External/HalfFloat/include/half.hpp"
//#include <half.hpp>

#define VK_USE_PLATFORM_WIN32_KHR 1
#define VK_PROTOTYPES 1

#include "../../External/Vulkan/include/vulkan/vulkan.h"
//#include <vulkan/vulkan.h>

//#include "../../External/Vulkan/include/vulkan/vk_platform.h"
//#include "../../External/Vulkan/include/vulkan/vk_sdk_platform.h"
//
//#include "../../External/Vulkan/include/vulkan/vk_layer.h"
//#include "../../External/Vulkan/include/vulkan/vk_debug_marker_layer.h"
//
//#include "../../External/Vulkan/include/vulkan/vk_lunarg_debug_marker.h"
//
//#include "../../External/Vulkan/include/vulkan/vk_icd.h"

#pragma endregion

#pragma region
#pragma endregion

namespace BVE
{
#pragma region Typedef
	typedef std::string										string;
	typedef std::int8_t										sint8;
	typedef std::int16_t									sint16;
	typedef std::int32_t									sint32;
	typedef std::int64_t									sint64;
	typedef std::uint8_t									uint8;
	typedef std::uint16_t									uint16;
	typedef std::uint32_t									uint32;
	typedef std::uint64_t									uint64;
	typedef half_float::half								float16;
	typedef std::float_t									float32;
	typedef std::double_t									float64;
	typedef int												unknownType;
	typedef std::string										string;
#pragma endregion
	namespace Vulkan
	{
#pragma region Preprocessor
	// Is debug mode on?
#ifdef _DEBUG
#define __BVE_VULKAN_DEBUG__									1
#else
#define __BVE_VULKAN_DEBUG__									0
#endif

// Create log?
#define __BVE_VULKAN_LOG__											1
#pragma endregion
#pragma region Typedef
#pragma endregion
#pragma region Const
		const string											logFilename = "log.log";
#pragma endregion
#pragma region Predeclaration
		class Exception;
		namespace Exceptions
		{
			class Exception_UnknownException;

			class Exception_OutOfHostMemory;
			class Exception_OutOfDeviceMemory;
			class Exception_InitializationFailed;
			class Exception_LayerNotPresent;
			class Exception_ExtensionNotPresent;
			class Exception_IncompatibleDriver;
		}
#pragma endregion
#pragma region Declaration
		struct LayerProperties:
			public VkLayerProperties
		{
		public:
			string		layerName;
			uint32_t    specVersion;
			uint32_t    implementationVersion;
			string		description;
		public:
			inline LayerProperties()
			{
			}
			inline LayerProperties(VkLayerProperties vk_layerProperties):
				layerName(vk_layerProperties.layerName, vk_layerProperties.layerName + strlen(vk_layerProperties.layerName) + 1),
				specVersion(vk_layerProperties.specVersion),
				implementationVersion(vk_layerProperties.implementationVersion),
				description(vk_layerProperties.description, vk_layerProperties.description + strlen(vk_layerProperties.description) + 1)
			{
			}
		};
		struct InstanceLayersProperties
		{
		public:
			std::vector<LayerProperties> layersProperties;
			std::vector<const char*> layersName;
		public:
			inline InstanceLayersProperties();
		};
		struct DeviceLayersProperties
		{
		public:
			std::vector<LayerProperties> layersProperties;
			std::vector<const char*> layersName;
		public:
			inline DeviceLayersProperties(VkPhysicalDevice vk_physicalDevice);
		};
		struct ExtensionProperties
		{
		public:
			string		extensionName;
			uint32_t	specVersion;
		public:
			inline ExtensionProperties()
			{
			}
			inline ExtensionProperties(VkExtensionProperties vk_extensionProperties):
				extensionName(vk_extensionProperties.extensionName, vk_extensionProperties.extensionName + strlen(vk_extensionProperties.extensionName) + 1),
				specVersion(vk_extensionProperties.specVersion)
			{
			}
		};
		struct InstanceExtensionsProperties
		{
		public:
			std::map<string, std::vector<ExtensionProperties>> layersExtensionsProperties;
			std::vector<const char*> extensionsName;
		public:
			inline InstanceExtensionsProperties(const InstanceLayersProperties& instanceLayersProperties);
		};
		struct DeviceExtensionsProperties
		{
		public:
			std::map<string, std::vector<ExtensionProperties>> layersExtensionsProperties;
			std::vector<const char*> extensionsName;
		public:
			inline DeviceExtensionsProperties(VkPhysicalDevice vk_physicalDevice, const DeviceLayersProperties& deviceLayersProperties);
		};

		class Exception
		{
		protected:
			const string										text;
		public:
			inline												Exception(const string& text_ = "");
			virtual												~Exception() = default;
		public:
			virtual string										Get() const;
		};
		namespace Exceptions
		{
			class Exception_UnknownException:
				public Exception
			{
			protected:
				VkResult										vk_result;
			public:
				inline											Exception_UnknownException(VkResult vk_result_);
				virtual											~Exception_UnknownException() override = default;
			};

			class Exception_OutOfHostMemory:
				public Exception
			{
			public:
				inline											Exception_OutOfHostMemory();
				virtual											~Exception_OutOfHostMemory() override = default;
			};
			class Exception_OutOfDeviceMemory:
				public Exception
			{
			public:
				inline											Exception_OutOfDeviceMemory();
				virtual											~Exception_OutOfDeviceMemory() override = default;
			};
			class Exception_InitializationFailed:
				public Exception
			{
			public:
				inline											Exception_InitializationFailed();
				virtual											~Exception_InitializationFailed() override = default;
			};
			class Exception_DeviceLost:
				public Exception
			{
			public:
				inline											Exception_DeviceLost();
				virtual											~Exception_DeviceLost() override = default;
			};
			class Exception_MemoryMapFailed:
				public Exception
			{
			public:
				inline											Exception_MemoryMapFailed();
				virtual											~Exception_MemoryMapFailed() override = default;
			};
			class Exception_LayerNotPresent:
				public Exception
			{
			public:
				inline											Exception_LayerNotPresent();
				virtual											~Exception_LayerNotPresent() override = default;
			};
			class Exception_ExtensionNotPresent:
				public Exception
			{
			public:
				inline											Exception_ExtensionNotPresent();
				virtual											~Exception_ExtensionNotPresent() override = default;
			};
			class Exception_FeatureNotPresent:
				public Exception
			{
			public:
				inline											Exception_FeatureNotPresent();
				virtual											~Exception_FeatureNotPresent() override = default;
			};
			class Exception_IncompatibleDriver:
				public Exception
			{
			public:
				inline											Exception_IncompatibleDriver();
				virtual											~Exception_IncompatibleDriver() override = default;
			};
			class Exception_TooManyObjects:
				public Exception
			{
			public:
				inline											Exception_TooManyObjects();
				virtual											~Exception_TooManyObjects() override = default;
			};
			class Exception_FormatNotSupported:
				public Exception
			{
			public:
				inline											Exception_FormatNotSupported();
				virtual											~Exception_FormatNotSupported() override = default;
			};
			class Exception_SurfaceLostKHR:
				public Exception
			{
			public:
				inline											Exception_SurfaceLostKHR();
				virtual											~Exception_SurfaceLostKHR() override = default;
			};
			class Exception_NativeWindowInUseKHR:
				public Exception
			{
			public:
				inline											Exception_NativeWindowInUseKHR();
				virtual											~Exception_NativeWindowInUseKHR() override = default;
			};
			class Exception_OutOfDateKHR:
				public Exception
			{
			public:
				inline											Exception_OutOfDateKHR();
				virtual											~Exception_OutOfDateKHR() override = default;
			};
			class Exception_IncompatibleDisplayKHR:
				public Exception
			{
			public:
				inline											Exception_IncompatibleDisplayKHR();
				virtual											~Exception_IncompatibleDisplayKHR() override = default;
			};
			class Exception_ValidationFailedExt:
				public Exception
			{
			public:
				inline											Exception_ValidationFailedExt();
				virtual											~Exception_ValidationFailedExt() override = default;
			};

			inline void Perform(VkResult vk_result);
		}
#pragma endregion
#pragma region Func
		inline std::vector<VkLayerProperties>					EnumerateInstanceLayerProperties();
		inline std::vector<VkExtensionProperties>				EnumerateInstanceExtensionProperties(const char* vk_layerName);
		inline std::vector<VkLayerProperties>					EnumerateDeviceLayerProperties(VkPhysicalDevice vk_physicalDevice);
		inline std::vector<VkExtensionProperties>				EnumerateDeviceExtensionProperties(VkPhysicalDevice vk_physicalDevice, const char* vk_layerName);

		inline VkInstance										CreateInstance(VkInstanceCreateInfo* vk_instanceInfo, const VkAllocationCallbacks* vk_allocator);
		inline void												DestroyInstance(VkInstance vk_instance, const VkAllocationCallbacks* vk_allocator);

		inline std::vector<VkPhysicalDevice>					EnumeratePhysicalDevices(VkInstance);

		inline VkPhysicalDeviceProperties						GetPhysicalDeviceProperties(VkPhysicalDevice vk_physicalDevice);
		inline std::vector<VkQueueFamilyProperties>				GetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice vk_physicalDevice);
		inline VkPhysicalDeviceMemoryProperties					GetPhysicalDeviceMemoryProperties(VkPhysicalDevice vk_physicalDevice);

		inline VkSurfaceKHR										CreateWin32SurfaceKHR(VkInstance vk_instance, HINSTANCE hInstance, HWND hWnd, VkAllocationCallbacks* vk_allocationCallbacks);

		inline std::vector<VkSurfaceFormatKHR>					GetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice vk_physicalDevice, VkSurfaceKHR vk_surface);
		inline VkSurfaceCapabilitiesKHR							GetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice vk_physicalDevice, VkSurfaceKHR vk_surface);
		inline std::vector<VkPresentModeKHR>					GetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice vk_physicalDevice, VkSurfaceKHR vk_surface);

		inline VkDevice											CreateDevice(VkPhysicalDevice vk_physicalDevice, VkDeviceCreateInfo* vk_deviceCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyDevice(VkDevice vk_device, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DeviceWaitIdle(VkDevice vk_device);

		inline VkDeviceMemory									AllocateMemory(VkDevice vk_device, VkMemoryAllocateInfo* vk_memoryAllocateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												BindImageMemory(VkDevice vk_device, VkImage vk_image, VkDeviceMemory vk_deviceMemory, VkDeviceSize vk_deviceSize);
		inline void												BindBufferMemory(VkDevice vk_device, VkBuffer vk_buffer, VkDeviceMemory vk_deviceMemory, VkDeviceSize vk_memoryOffset);
		inline void*											MapMemory(VkDevice vk_device, VkDeviceMemory vk_deviceMemory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags vk_memoryMapFlags);
		inline void												UnmapMemory(VkDevice vk_device, VkDeviceMemory vk_deviceMemory);

		inline VkSwapchainKHR									CreateSwapchainKHR(VkDevice vk_device, VkSwapchainCreateInfoKHR* vk_SwapchainCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline std::vector<VkImage>								GetSwapchainImagesKHR(VkDevice vk_device, VkSwapchainKHR vk_swapchain);
		inline uint32_t											AcquireNextImageKHR(VkDevice vk_device, VkSwapchainKHR vk_swapchain, uint64_t timeout, VkSemaphore vk_semaphore, VkFence vk_fence);

		inline VkQueue											GetDeviceQueue(VkDevice vk_device, uint32_t vk_queueFamilyIndex, uint32_t vk_queueIndex);
		inline void												QueueSubmit(VkQueue vk_queue, std::vector<VkSubmitInfo>& vk_submitInfos, VkFence vk_fence);
		inline void												QueueWaitIdle(VkQueue vk_queue);

		inline VkCommandPool									CreateCommandPool(VkDevice vk_device, VkCommandPoolCreateFlags vk_commandPoolCreateFlags, uint32 vk_commandPoolQueueFamilyIndex, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyCommandPool(VkDevice vk_device, VkCommandPool vk_commandPool, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												ResetCommandPool(VkDevice vk_device, VkCommandPool vk_commandPool, VkCommandPoolResetFlags vk_commandPoolResetFlags);

		inline std::vector<VkCommandBuffer>						AllocateCommandBuffers(VkDevice vk_device, VkCommandPool vk_commandPool, uint32_t vk_commandBuffersCount, VkCommandBufferLevel vk_commandBufferLevel);
		inline void												FreeCommandBuffers(VkDevice vk_device, VkCommandPool vk_commandPool, std::vector<VkCommandBuffer>& vk_commandBuffers);
		inline void												ResetCommandBuffer(VkCommandBuffer vk_commandBuffer, VkCommandBufferResetFlags vk_commandBufferResetFlags);
		inline void												BeginCommandBuffer(VkCommandBuffer vk_commandBuffer, VkCommandBufferBeginInfo *vk_commandBufferBeginInfo);
		inline void												EndCommandBuffer(VkCommandBuffer vk_commandBuffer);

		inline void												CmdExecuteCommands(VkCommandBuffer vk_commandBuffer, std::vector<VkCommandBuffer>& vk_commandBuffers);

		inline VkBuffer											CreateBuffer(VkDevice vk_device, VkBufferCreateInfo* vk_bufferCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline VkMemoryRequirements								GetBufferMemoryRequirements(VkDevice vk_device, VkBuffer vk_buffer);


		inline VkRenderPass										CreateRenderPass(VkDevice vk_device, VkRenderPassCreateInfo* vk_renderPassCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyRenderPass(VkDevice vk_device, VkRenderPass vk_renderPass, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkFramebuffer									CreateFramebuffer(VkDevice vk_device, VkFramebufferCreateInfo* vk_framebufferCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyFramebuffer(VkDevice vk_device, VkFramebuffer vk_framebuffer, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkImage											CreateImage(VkDevice vk_device, VkImageCreateInfo* vk_imageCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline VkMemoryRequirements								GetImageMemoryRequirements(VkDevice vk_device, VkImage vk_image);

		inline VkImageView										CreateImageView(VkDevice vk_device, VkImageViewCreateInfo* vk_imageViewCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkSampler										CreateSampler(VkDevice vk_device, VkSamplerCreateInfo* vk_samplerCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkShaderModule									CreateShaderModule(VkDevice vk_device, VkShaderModuleCreateInfo* vk_shaderModuleCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkPipelineCache									CreatePipelineCache(VkDevice vk_device, VkPipelineCacheCreateInfo* vk_pipelineCacheCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkDescriptorSetLayout							CreateDescriptorSetLayout(VkDevice vk_device, VkDescriptorSetLayoutCreateInfo* vk_descriptorSetLayoutCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkPipelineLayout									CreatePipelineLayout(VkDevice vk_device, VkPipelineLayoutCreateInfo* vk_pipelineLayoutCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);

		inline std::vector<VkPipeline>							CreateGraphicsPipelines(VkDevice vk_device, VkPipelineCache& vk_pipelineCache, std::vector<VkGraphicsPipelineCreateInfo>& vk_graphicsPipelineCreateInfos, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkDescriptorPool									CreateDescriptorPool(VkDevice vk_device, VkDescriptorPoolCreateInfo* vk_descriptorPoolCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);

		inline std::vector<VkDescriptorSet>						AllocateDescriptorSets(VkDevice vk_device, VkDescriptorPool vk_descriptorPool, uint32_t count, const VkDescriptorSetLayout* vk_descriptorSetLayout);

		inline uint32_t											GetCorrectMemoryType(const VkPhysicalDeviceMemoryProperties& vk_physicalDeviceMemoryProperties, uint32_t vk_memoryTypeBits, VkFlags flags);
#pragma endregion
#pragma region Log
		namespace Log
		{
			extern std::mutex									writeMutex;
			extern std::clock_t									timestampClock;
			inline void											Clear();
			inline void											Write(const string& text);
		}
#pragma endregion
	}
}

#pragma region
#pragma endregion

#pragma region Vulkan
#pragma region Exception
inline															BVE::Vulkan::Exception::Exception(const string& text_):
	text(text_)
{
}
#pragma endregion
#pragma region Exceptions
#pragma region Exception_UnknownException
inline															BVE::Vulkan::Exceptions::Exception_UnknownException::Exception_UnknownException(VkResult vk_result_):
	Exception(),
	vk_result(vk_result_)
{
}
#pragma endregion

#pragma region Exception_OutOfHostMemory
inline															BVE::Vulkan::Exceptions::Exception_OutOfHostMemory::Exception_OutOfHostMemory():
	Exception()
{
}
#pragma endregion
#pragma region
inline															BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory::Exception_OutOfDeviceMemory():
	Exception()
{
}
#pragma endregion
#pragma region Exception_InitializationFailed
inline															BVE::Vulkan::Exceptions::Exception_InitializationFailed::Exception_InitializationFailed():
	Exception()
{
}
#pragma endregion
#pragma region Exception_DeviceLost
inline															BVE::Vulkan::Exceptions::Exception_DeviceLost::Exception_DeviceLost():
	Exception()
{
}
#pragma endregion
#pragma region Exception_MemoryMapFailed
inline															BVE::Vulkan::Exceptions::Exception_MemoryMapFailed::Exception_MemoryMapFailed():
	Exception()
{
}
#pragma endregion
#pragma region Exception_LayerNotPresent
inline															BVE::Vulkan::Exceptions::Exception_LayerNotPresent::Exception_LayerNotPresent():
	Exception()
{
}
#pragma endregion
#pragma region Exception_ExtensionNotPresent
inline															BVE::Vulkan::Exceptions::Exception_ExtensionNotPresent::Exception_ExtensionNotPresent():
	Exception()
{
}
#pragma endregion
#pragma region Exception_FeatureNotPresent
inline															BVE::Vulkan::Exceptions::Exception_FeatureNotPresent::Exception_FeatureNotPresent():
	Exception()
{
}
#pragma endregion
#pragma region Exception_IncompatibleDriver
inline															BVE::Vulkan::Exceptions::Exception_IncompatibleDriver::Exception_IncompatibleDriver():
	Exception()
{
}
#pragma endregion
#pragma region Exception_TooManyObjects
inline															BVE::Vulkan::Exceptions::Exception_TooManyObjects::Exception_TooManyObjects():
	Exception()
{
}
#pragma endregion
#pragma region Exception_FormatNotSupported
inline															BVE::Vulkan::Exceptions::Exception_FormatNotSupported::Exception_FormatNotSupported():
	Exception()
{
}
#pragma endregion
#pragma region Exception_SurfaceLostKHR
inline															BVE::Vulkan::Exceptions::Exception_SurfaceLostKHR::Exception_SurfaceLostKHR():
	Exception()
{
}
#pragma endregion
#pragma region Exception_NativeWindowInUseKHR
inline															BVE::Vulkan::Exceptions::Exception_NativeWindowInUseKHR::Exception_NativeWindowInUseKHR():
	Exception()
{
}
#pragma endregion
#pragma region Exception_OutOfDateKHR
inline															BVE::Vulkan::Exceptions::Exception_OutOfDateKHR::Exception_OutOfDateKHR():
	Exception()
{
}
#pragma endregion
#pragma region Exception_IncompatibleDisplayKHR
inline															BVE::Vulkan::Exceptions::Exception_IncompatibleDisplayKHR::Exception_IncompatibleDisplayKHR():
	Exception()
{
}
#pragma endregion
#pragma region Exception_ValidationFailedExt
inline															BVE::Vulkan::Exceptions::Exception_ValidationFailedExt::Exception_ValidationFailedExt():
	Exception()
{
}
#pragma endregion

inline void														BVE::Vulkan::Exceptions::Perform(VkResult vk_result)
{
	switch(vk_result)
	{
		/*
		VK_SUCCESS = 0,
		VK_NOT_READY = 1,
		VK_TIMEOUT = 2,
		VK_EVENT_SET = 3,
		VK_EVENT_RESET = 4,
		VK_INCOMPLETE = 5,
		VK_ERROR_OUT_OF_HOST_MEMORY = -1,
		VK_ERROR_OUT_OF_DEVICE_MEMORY = -2,
		VK_ERROR_INITIALIZATION_FAILED = -3,
		VK_ERROR_DEVICE_LOST = -4,
		VK_ERROR_MEMORY_MAP_FAILED = -5,
		VK_ERROR_LAYER_NOT_PRESENT = -6,
		VK_ERROR_EXTENSION_NOT_PRESENT = -7,
		VK_ERROR_FEATURE_NOT_PRESENT = -8,
		VK_ERROR_INCOMPATIBLE_DRIVER = -9,
		VK_ERROR_TOO_MANY_OBJECTS = -10,
		VK_ERROR_FORMAT_NOT_SUPPORTED = -11,
		VK_ERROR_SURFACE_LOST_KHR = -1000000000,
		VK_ERROR_NATIVE_WINDOW_IN_USE_KHR = -1000000001,
		VK_SUBOPTIMAL_KHR = 1000001003,
		VK_ERROR_OUT_OF_DATE_KHR = -1000001004,
		VK_ERROR_INCOMPATIBLE_DISPLAY_KHR = -1000003001,
		VK_ERROR_VALIDATION_FAILED_EXT = -1000011001,
		VK_RESULT_BEGIN_RANGE = VK_ERROR_FORMAT_NOT_SUPPORTED,
		VK_RESULT_END_RANGE = VK_INCOMPLETE,
		VK_RESULT_RANGE_SIZE = (VK_INCOMPLETE - VK_ERROR_FORMAT_NOT_SUPPORTED + 1),
		VK_RESULT_MAX_ENUM = 0x7FFFFFFF
		*/
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		case VK_ERROR_INITIALIZATION_FAILED:
			throw BVE::Vulkan::Exceptions::Exception_InitializationFailed();
		case VK_ERROR_DEVICE_LOST:
			throw BVE::Vulkan::Exceptions::Exception_DeviceLost();
		case VK_ERROR_MEMORY_MAP_FAILED:
			throw BVE::Vulkan::Exceptions::Exception_MemoryMapFailed();
		case VK_ERROR_LAYER_NOT_PRESENT:
			throw BVE::Vulkan::Exceptions::Exception_LayerNotPresent();
		case VK_ERROR_EXTENSION_NOT_PRESENT:
			throw BVE::Vulkan::Exceptions::Exception_ExtensionNotPresent();
		case VK_ERROR_FEATURE_NOT_PRESENT:
			throw BVE::Vulkan::Exceptions::Exception_FeatureNotPresent();
		case VK_ERROR_INCOMPATIBLE_DRIVER:
			throw BVE::Vulkan::Exceptions::Exception_IncompatibleDriver();
		case VK_ERROR_TOO_MANY_OBJECTS:
			throw BVE::Vulkan::Exceptions::Exception_TooManyObjects();
		case VK_ERROR_FORMAT_NOT_SUPPORTED:
			throw BVE::Vulkan::Exceptions::Exception_FormatNotSupported();
		case VK_ERROR_SURFACE_LOST_KHR:
			throw BVE::Vulkan::Exceptions::Exception_SurfaceLostKHR();
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
			throw BVE::Vulkan::Exceptions::Exception_NativeWindowInUseKHR();
		case VK_ERROR_OUT_OF_DATE_KHR:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDateKHR();
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
			throw BVE::Vulkan::Exceptions::Exception_IncompatibleDisplayKHR();
		case VK_ERROR_VALIDATION_FAILED_EXT:
			throw BVE::Vulkan::Exceptions::Exception_ValidationFailedExt();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}
}
#pragma endregion

inline BVE::Vulkan::InstanceLayersProperties::InstanceLayersProperties()
{
	auto vk_instanceLayersProperties = EnumerateInstanceLayerProperties();

	layersProperties.resize(vk_instanceLayersProperties.size());
	for(size_t i = 0; i < vk_instanceLayersProperties.size(); ++i)
	{
		layersProperties[i] = LayerProperties(vk_instanceLayersProperties[i]);
		layersName.push_back(layersProperties[i].layerName.c_str());
	}
}
inline BVE::Vulkan::DeviceLayersProperties::DeviceLayersProperties(VkPhysicalDevice vk_physicalDevice)
{
	auto vk_deviceLayersProperties = EnumerateDeviceLayerProperties(vk_physicalDevice);

	layersProperties.resize(vk_deviceLayersProperties.size());
	for(size_t i = 0; i < vk_deviceLayersProperties.size(); ++i)
	{
		layersProperties[i] = LayerProperties(vk_deviceLayersProperties[i]);
		layersName.push_back(layersProperties[i].layerName.c_str());
	}
}
inline BVE::Vulkan::InstanceExtensionsProperties::InstanceExtensionsProperties(const InstanceLayersProperties& instanceLayersProperties)
{
	for(auto &layerProperties : instanceLayersProperties.layersProperties)
	{
		auto &layersExtensionProperties = layersExtensionsProperties[layerProperties.layerName];

		auto vk_instanceExtensionsProperties = EnumerateInstanceExtensionProperties(layerProperties.layerName.c_str());

		layersExtensionProperties.resize(vk_instanceExtensionsProperties.size());
		for(size_t i = 0; i < vk_instanceExtensionsProperties.size(); ++i)
		{
			layersExtensionProperties[i] = ExtensionProperties(vk_instanceExtensionsProperties[i]);
			extensionsName.push_back(layersExtensionProperties[i].extensionName.c_str());
		}
	}

	auto &layersExtensionProperties = layersExtensionsProperties[""];

	auto vk_instanceExtensionsProperties = EnumerateInstanceExtensionProperties("");

	layersExtensionProperties.resize(vk_instanceExtensionsProperties.size());
	for(size_t i = 0; i < vk_instanceExtensionsProperties.size(); ++i)
	{
		layersExtensionProperties[i] = ExtensionProperties(vk_instanceExtensionsProperties[i]);
		extensionsName.push_back(layersExtensionProperties[i].extensionName.c_str());
	}
}
inline BVE::Vulkan::DeviceExtensionsProperties::DeviceExtensionsProperties(VkPhysicalDevice vk_physicalDevice, const DeviceLayersProperties& deviceLayersProperties)
{
	for(auto &layerProperties : deviceLayersProperties.layersProperties)
	{
		auto &layersExtensionProperties = layersExtensionsProperties[layerProperties.layerName];

		auto vk_deviceExtensionsProperties = EnumerateDeviceExtensionProperties(vk_physicalDevice, layerProperties.layerName.c_str());

		layersExtensionProperties.resize(vk_deviceExtensionsProperties.size());
		for(size_t i = 0; i < vk_deviceExtensionsProperties.size(); ++i)
		{
			layersExtensionProperties[i] = ExtensionProperties(vk_deviceExtensionsProperties[i]);
			extensionsName.push_back(layersExtensionProperties[i].extensionName.c_str());
		}
	}

	auto &layersExtensionProperties = layersExtensionsProperties[""];

	auto vk_deviceExtensionsProperties = EnumerateDeviceExtensionProperties(vk_physicalDevice, "");

	layersExtensionProperties.resize(vk_deviceExtensionsProperties.size());
	for(size_t i = 0; i < vk_deviceExtensionsProperties.size(); ++i)
	{
		layersExtensionProperties[i] = ExtensionProperties(vk_deviceExtensionsProperties[i]);
		extensionsName.push_back(layersExtensionProperties[i].extensionName.c_str());
	}
}

#pragma region Func
inline std::vector<VkLayerProperties>							BVE::Vulkan::EnumerateInstanceLayerProperties()
{
	uint32_t vk_instancelayersPropertiesCount;
	BVE::Vulkan::Exceptions::Perform(vkEnumerateInstanceLayerProperties(&vk_instancelayersPropertiesCount, nullptr));

	std::vector<VkLayerProperties> vk_instancelayersProperties(vk_instancelayersPropertiesCount);
	BVE::Vulkan::Exceptions::Perform(vkEnumerateInstanceLayerProperties(&vk_instancelayersPropertiesCount, vk_instancelayersProperties.data()));

	return vk_instancelayersProperties;
}
inline std::vector<VkLayerProperties>							BVE::Vulkan::EnumerateDeviceLayerProperties(VkPhysicalDevice vk_physicalDevice)
{
	uint32_t vk_layersPropertiesCount;
	vkEnumerateDeviceLayerProperties(vk_physicalDevice, &vk_layersPropertiesCount, nullptr);

	std::vector<VkLayerProperties> vk_layersProperties(vk_layersPropertiesCount);
	vkEnumerateDeviceLayerProperties(vk_physicalDevice, &vk_layersPropertiesCount, vk_layersProperties.data());

	return std::move(vk_layersProperties);
}
inline std::vector<VkExtensionProperties>						BVE::Vulkan::EnumerateInstanceExtensionProperties(const char* vk_layerName)
{
	uint32_t vk_instanceExtensionPropertiesCount;
	vkEnumerateInstanceExtensionProperties(vk_layerName, &vk_instanceExtensionPropertiesCount, nullptr);

	std::vector<VkExtensionProperties> vk_extensionProperties(vk_instanceExtensionPropertiesCount);
	vkEnumerateInstanceExtensionProperties(vk_layerName, &vk_instanceExtensionPropertiesCount, vk_extensionProperties.data());

	return std::move(vk_extensionProperties);
}
inline std::vector<VkExtensionProperties>						BVE::Vulkan::EnumerateDeviceExtensionProperties(VkPhysicalDevice vk_physicalDevice, const char* vk_layerName)
{
	uint32_t vk_deviceExtensionPropertiesCount;
	vkEnumerateDeviceExtensionProperties(vk_physicalDevice, vk_layerName, &vk_deviceExtensionPropertiesCount, nullptr);

	std::vector<VkExtensionProperties> vk_extensionProperties(vk_deviceExtensionPropertiesCount);
	vkEnumerateDeviceExtensionProperties(vk_physicalDevice, vk_layerName, &vk_deviceExtensionPropertiesCount, vk_extensionProperties.data());

	return std::move(vk_extensionProperties);
}

inline VkInstance												BVE::Vulkan::CreateInstance(VkInstanceCreateInfo* vk_instanceInfo, const VkAllocationCallbacks* vk_allocator)
{
	VkInstance vk_instance;

	auto vk_result = vkCreateInstance(vk_instanceInfo, vk_allocator, &vk_instance);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw new BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw new BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		case VK_ERROR_INITIALIZATION_FAILED:
			throw new BVE::Vulkan::Exceptions::Exception_InitializationFailed();
		case VK_ERROR_LAYER_NOT_PRESENT:
			throw new BVE::Vulkan::Exceptions::Exception_LayerNotPresent();
		case VK_ERROR_EXTENSION_NOT_PRESENT:
			throw new BVE::Vulkan::Exceptions::Exception_ExtensionNotPresent();
		case VK_ERROR_INCOMPATIBLE_DRIVER:
			throw new BVE::Vulkan::Exceptions::Exception_IncompatibleDriver();
		default:
			throw new BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_instance;
}
inline void														BVE::Vulkan::DestroyInstance(VkInstance vk_instance, const VkAllocationCallbacks* vk_allocator)
{
	vkDestroyInstance(vk_instance, vk_allocator);
}

inline std::vector<VkPhysicalDevice>							BVE::Vulkan::EnumeratePhysicalDevices(VkInstance vk_instance)
{
	uint32_t vk_physicalDevicesCount = 0;
	{
		auto vk_result = vkEnumeratePhysicalDevices(vk_instance, &vk_physicalDevicesCount, nullptr);
		switch(vk_result)
		{
			case VK_SUCCESS:
				break;
			case VK_ERROR_OUT_OF_HOST_MEMORY:
				throw new BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
			case VK_ERROR_OUT_OF_DEVICE_MEMORY:
				throw new BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
			case VK_ERROR_INITIALIZATION_FAILED:
				throw new BVE::Vulkan::Exceptions::Exception_InitializationFailed();
			default:
				throw new BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
		}
	}

	std::vector<VkPhysicalDevice> vk_physicalDevices(vk_physicalDevicesCount);

	auto vk_result = vkEnumeratePhysicalDevices(vk_instance, &vk_physicalDevicesCount, vk_physicalDevices.data());
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw new BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw new BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		case VK_ERROR_INITIALIZATION_FAILED:
			throw new BVE::Vulkan::Exceptions::Exception_InitializationFailed();
		default:
			throw new BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return std::move(vk_physicalDevices);
}

inline VkPhysicalDeviceProperties								BVE::Vulkan::GetPhysicalDeviceProperties(VkPhysicalDevice vk_physicalDevice)
{
	VkPhysicalDeviceProperties vk_physicalDeviceProperties;

	vkGetPhysicalDeviceProperties(vk_physicalDevice, &vk_physicalDeviceProperties);

	return vk_physicalDeviceProperties;
}
inline std::vector<VkQueueFamilyProperties>						BVE::Vulkan::GetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice vk_physicalDevice)
{
	uint32_t vk_queueFamilyPropertiesCount;
	vkGetPhysicalDeviceQueueFamilyProperties(vk_physicalDevice, &vk_queueFamilyPropertiesCount, nullptr);

	std::vector<VkQueueFamilyProperties> vk_queueFamilyProperties(vk_queueFamilyPropertiesCount);
	vkGetPhysicalDeviceQueueFamilyProperties(vk_physicalDevice, &vk_queueFamilyPropertiesCount, vk_queueFamilyProperties.data());

	return std::move(vk_queueFamilyProperties);
}
inline VkPhysicalDeviceMemoryProperties							BVE::Vulkan::GetPhysicalDeviceMemoryProperties(VkPhysicalDevice vk_physicalDevice)
{
	VkPhysicalDeviceMemoryProperties vk_physicalDeviceMemoryProperties;
	
	vkGetPhysicalDeviceMemoryProperties(vk_physicalDevice, &vk_physicalDeviceMemoryProperties);

	return vk_physicalDeviceMemoryProperties;
}

inline VkSurfaceKHR												BVE::Vulkan::CreateWin32SurfaceKHR(VkInstance vk_instance, HINSTANCE hInstance, HWND hWnd, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkWin32SurfaceCreateInfoKHR vk_win32SurfaceCreateInfoKHR;
	{
		vk_win32SurfaceCreateInfoKHR.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
		vk_win32SurfaceCreateInfoKHR.pNext = nullptr;
		vk_win32SurfaceCreateInfoKHR.flags = 0;
		vk_win32SurfaceCreateInfoKHR.hinstance = hInstance;
		vk_win32SurfaceCreateInfoKHR.hwnd = hWnd;
	}

	VkSurfaceKHR vk_surfaceKHR;

	BVE::Vulkan::Exceptions::Perform(vkCreateWin32SurfaceKHR(vk_instance, &vk_win32SurfaceCreateInfoKHR, vk_allocationCallbacks, &vk_surfaceKHR));

	return vk_surfaceKHR;
}

inline std::vector<VkSurfaceFormatKHR>							BVE::Vulkan::GetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice vk_physicalDevice, VkSurfaceKHR vk_surface)
{
	uint32_t vk_surfaceFormatsCount;
	BVE::Vulkan::Exceptions::Perform(vkGetPhysicalDeviceSurfaceFormatsKHR(vk_physicalDevice, vk_surface, &vk_surfaceFormatsCount, nullptr));

	std::vector<VkSurfaceFormatKHR> vk_surfaceFormats(vk_surfaceFormatsCount);
	BVE::Vulkan::Exceptions::Perform(vkGetPhysicalDeviceSurfaceFormatsKHR(vk_physicalDevice, vk_surface, &vk_surfaceFormatsCount, vk_surfaceFormats.data()));

	return std::move(vk_surfaceFormats);
}
inline VkSurfaceCapabilitiesKHR									BVE::Vulkan::GetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice vk_physicalDevice, VkSurfaceKHR vk_surface)
{
	VkSurfaceCapabilitiesKHR vk_surfaceCapabilities;

	BVE::Vulkan::Exceptions::Perform(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vk_physicalDevice, vk_surface, &vk_surfaceCapabilities));

	return vk_surfaceCapabilities;
}
inline std::vector<VkPresentModeKHR>							BVE::Vulkan::GetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice vk_physicalDevice, VkSurfaceKHR vk_surface)
{
	uint32_t vk_physicalDeviceSurfacePresentModesCount;
	BVE::Vulkan::Exceptions::Perform(vkGetPhysicalDeviceSurfacePresentModesKHR(vk_physicalDevice, vk_surface, &vk_physicalDeviceSurfacePresentModesCount, nullptr));

	std::vector<VkPresentModeKHR> vk_physicalDeviceSurfacePresentModes(vk_physicalDeviceSurfacePresentModesCount);
	BVE::Vulkan::Exceptions::Perform(vkGetPhysicalDeviceSurfacePresentModesKHR(vk_physicalDevice, vk_surface, &vk_physicalDeviceSurfacePresentModesCount, vk_physicalDeviceSurfacePresentModes.data()));

	return std::move(vk_physicalDeviceSurfacePresentModes);
}

inline VkDevice													BVE::Vulkan::CreateDevice(VkPhysicalDevice vk_physicalDevice, VkDeviceCreateInfo* vk_deviceCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkDevice vk_device;

	auto vk_result = vkCreateDevice(vk_physicalDevice, vk_deviceCreateInfo, vk_allocationCallbacks, &vk_device);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw new BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw new BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		case VK_ERROR_INITIALIZATION_FAILED:
			throw new BVE::Vulkan::Exceptions::Exception_InitializationFailed();
		case VK_ERROR_DEVICE_LOST:
			throw new BVE::Vulkan::Exceptions::Exception_DeviceLost();
		case VK_ERROR_LAYER_NOT_PRESENT:
			throw new BVE::Vulkan::Exceptions::Exception_LayerNotPresent();
		case VK_ERROR_EXTENSION_NOT_PRESENT:
			throw new BVE::Vulkan::Exceptions::Exception_ExtensionNotPresent();
		case VK_ERROR_FEATURE_NOT_PRESENT:
			throw new BVE::Vulkan::Exceptions::Exception_FeatureNotPresent();
		case VK_ERROR_TOO_MANY_OBJECTS:
			throw new BVE::Vulkan::Exceptions::Exception_TooManyObjects();
		default:
			throw new BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_device;
}
inline void														BVE::Vulkan::DestroyDevice(VkDevice vk_device, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyDevice(vk_device, vk_allocationCallbacks);
}
inline void														BVE::Vulkan::DeviceWaitIdle(VkDevice vk_device)
{
	auto vk_result = vkDeviceWaitIdle(vk_device);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		case VK_ERROR_DEVICE_LOST:
			throw BVE::Vulkan::Exceptions::Exception_DeviceLost();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}
}

inline VkDeviceMemory											BVE::Vulkan::AllocateMemory(VkDevice vk_device, VkMemoryAllocateInfo* vk_memoryAllocateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkDeviceMemory vk_deviceMemory;

	BVE::Vulkan::Exceptions::Perform(vkAllocateMemory(vk_device, vk_memoryAllocateInfo, vk_allocationCallbacks, &vk_deviceMemory));

	return vk_deviceMemory;
}
inline void														BVE::Vulkan::BindImageMemory(VkDevice vk_device, VkImage vk_image, VkDeviceMemory vk_deviceMemory, VkDeviceSize vk_deviceSize)
{
	BVE::Vulkan::Exceptions::Perform(vkBindImageMemory(vk_device, vk_image, vk_deviceMemory, vk_deviceSize));
}
inline void														BVE::Vulkan::BindBufferMemory(VkDevice vk_device, VkBuffer vk_buffer, VkDeviceMemory vk_deviceMemory, VkDeviceSize vk_memoryOffset)
{
	BVE::Vulkan::Exceptions::Perform(vkBindBufferMemory(vk_device, vk_buffer, vk_deviceMemory, vk_memoryOffset));
}
inline void*													BVE::Vulkan::MapMemory(VkDevice vk_device, VkDeviceMemory vk_deviceMemory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags vk_memoryMapFlags)
{
	void* data;

	auto vk_result = vkMapMemory(vk_device, vk_deviceMemory, offset, size, vk_memoryMapFlags, &data);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		case VK_ERROR_MEMORY_MAP_FAILED:
			throw BVE::Vulkan::Exceptions::Exception_MemoryMapFailed();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return data;
}
inline void														BVE::Vulkan::UnmapMemory(VkDevice vk_device, VkDeviceMemory vk_deviceMemory)
{
	vkUnmapMemory(vk_device, vk_deviceMemory);
}

inline VkSwapchainKHR											BVE::Vulkan::CreateSwapchainKHR(VkDevice vk_device, VkSwapchainCreateInfoKHR* vk_SwapchainCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkSwapchainKHR vk_swapchain;

	BVE::Vulkan::Exceptions::Perform(vkCreateSwapchainKHR(vk_device, vk_SwapchainCreateInfo, vk_allocationCallbacks, &vk_swapchain));

	return vk_swapchain;
}
inline std::vector<VkImage>										BVE::Vulkan::GetSwapchainImagesKHR(VkDevice vk_device, VkSwapchainKHR vk_swapchain)
{
	uint32_t vk_imagesCount;
	BVE::Vulkan::Exceptions::Perform(vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &vk_imagesCount, nullptr));

	std::vector<VkImage> vk_images(vk_imagesCount);
	BVE::Vulkan::Exceptions::Perform(vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &vk_imagesCount, vk_images.data()));

	return std::move(vk_images);
}
inline uint32_t													BVE::Vulkan::AcquireNextImageKHR(VkDevice vk_device, VkSwapchainKHR vk_swapchain, uint64_t timeout, VkSemaphore vk_semaphore, VkFence vk_fence)
{
	uint32_t vk_currentImage;

	Exceptions::Perform(vkAcquireNextImageKHR(vk_device, vk_swapchain, UINT64_MAX, 0, (VkFence)0, &vk_currentImage));

	return vk_currentImage;
}

inline VkQueue													BVE::Vulkan::GetDeviceQueue(VkDevice vk_device, uint32_t vk_queueFamilyIndex, uint32_t vk_queueIndex)
{
	VkQueue vk_queue = 0;

	vkGetDeviceQueue(
		vk_device,
		vk_queueFamilyIndex, // less than vk_deviceCreateInfo.queueCreateInfoCount
		vk_queueIndex, // less than vk_deviceQueueCreateInfos[i].queueCount
		&vk_queue
	);

	return vk_queue;
}
inline void														BVE::Vulkan::QueueSubmit(VkQueue vk_queue, std::vector<VkSubmitInfo>& vk_submitInfos, VkFence vk_fence)
{
	BVE::Vulkan::Exceptions::Perform(vkQueueSubmit(vk_queue, vk_submitInfos.size(), vk_submitInfos.data(), vk_fence));
}
inline void														BVE::Vulkan::QueueWaitIdle(VkQueue vk_queue)
{
	auto vk_result = vkQueueWaitIdle(vk_queue);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		case VK_ERROR_DEVICE_LOST:
			throw BVE::Vulkan::Exceptions::Exception_DeviceLost();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}
}

inline VkCommandPool											BVE::Vulkan::CreateCommandPool(VkDevice vk_device, VkCommandPoolCreateFlags vk_commandPoolCreateFlags, uint32 vk_commandPoolQueueFamilyIndex, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkCommandPool vk_commandPool;

	VkCommandPoolCreateInfo vk_commandPoolCreateInfo;
	{
		vk_commandPoolCreateInfo.sType				= VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		vk_commandPoolCreateInfo.pNext				= nullptr;
		vk_commandPoolCreateInfo.flags				= vk_commandPoolCreateFlags;
		vk_commandPoolCreateInfo.queueFamilyIndex	= vk_commandPoolQueueFamilyIndex;
	}

	auto vk_result = vkCreateCommandPool(vk_device, &vk_commandPoolCreateInfo, vk_allocationCallbacks, &vk_commandPool);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_commandPool;
}
inline void														BVE::Vulkan::DestroyCommandPool(VkDevice vk_device, VkCommandPool vk_commandPool, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyCommandPool(vk_device, vk_commandPool, vk_allocationCallbacks);
}
inline void														BVE::Vulkan::ResetCommandPool(VkDevice vk_device, VkCommandPool vk_commandPool, VkCommandPoolResetFlags vk_commandPoolResetFlags)
{
	auto vk_result = vkResetCommandPool(vk_device, vk_commandPool, vk_commandPoolResetFlags);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}
}

inline std::vector<VkCommandBuffer>								BVE::Vulkan::AllocateCommandBuffers(VkDevice vk_device, VkCommandPool vk_commandPool, uint32_t vk_commandBuffersCount, VkCommandBufferLevel vk_commandBufferLevel)
{
	std::vector<VkCommandBuffer> vk_commandBuffers(vk_commandBuffersCount);

	VkCommandBufferAllocateInfo vk_CommandBufferAllocateInfo;
	{
		vk_CommandBufferAllocateInfo.sType				= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		vk_CommandBufferAllocateInfo.pNext				= nullptr;
		vk_CommandBufferAllocateInfo.commandPool		= vk_commandPool;
		vk_CommandBufferAllocateInfo.level				= vk_commandBufferLevel; // only VK_COMMAND_BUFFER_LEVEL_PRIMARY, VK_COMMAND_BUFFER_LEVEL_SECONDARY
		vk_CommandBufferAllocateInfo.commandBufferCount = vk_commandBuffersCount;
	}

	auto vk_result = vkAllocateCommandBuffers(vk_device, &vk_CommandBufferAllocateInfo, vk_commandBuffers.data());
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return std::move(vk_commandBuffers);
}
inline void														BVE::Vulkan::FreeCommandBuffers(VkDevice vk_device, VkCommandPool vk_commandPool, std::vector<VkCommandBuffer>& vk_commandBuffers)
{
	vkFreeCommandBuffers(vk_device, vk_commandPool, vk_commandBuffers.size(), vk_commandBuffers.data());
	vk_commandBuffers.clear();
}
inline void														BVE::Vulkan::ResetCommandBuffer(VkCommandBuffer vk_commandBuffer, VkCommandBufferResetFlags vk_commandBufferResetFlags)
{
	auto vk_result = vkResetCommandBuffer(vk_commandBuffer, vk_commandBufferResetFlags);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}
}
inline void														BVE::Vulkan::BeginCommandBuffer(VkCommandBuffer vk_commandBuffer, VkCommandBufferBeginInfo *vk_commandBufferBeginInfo)
{
	auto vk_result = vkBeginCommandBuffer(vk_commandBuffer, vk_commandBufferBeginInfo);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}
}
inline void														BVE::Vulkan::EndCommandBuffer(VkCommandBuffer vk_commandBuffer)
{
	auto vk_result = vkEndCommandBuffer(vk_commandBuffer);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}
}

inline VkBuffer													BVE::Vulkan::CreateBuffer(VkDevice vk_device, VkBufferCreateInfo* vk_bufferCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkBuffer vk_buffer;

	auto vk_result = vkCreateBuffer(vk_device, vk_bufferCreateInfo, vk_allocationCallbacks, &vk_buffer);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_buffer;
}
inline VkMemoryRequirements										BVE::Vulkan::GetBufferMemoryRequirements(VkDevice vk_device, VkBuffer vk_buffer)
{
	VkMemoryRequirements vk_memoryRequirements;

	vkGetBufferMemoryRequirements(vk_device, vk_buffer, &vk_memoryRequirements);

	return vk_memoryRequirements;
}

inline void														BVE::Vulkan::CmdExecuteCommands(VkCommandBuffer vk_commandBuffer, std::vector<VkCommandBuffer>& vk_commandBuffers)
{
	vkCmdExecuteCommands(vk_commandBuffer, vk_commandBuffers.size(), vk_commandBuffers.data());
}

VkRenderPass													BVE::Vulkan::CreateRenderPass(VkDevice vk_device, VkRenderPassCreateInfo* vk_renderPassCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkRenderPass vk_renderPass;

	auto vk_result = vkCreateRenderPass(vk_device, vk_renderPassCreateInfo, vk_allocationCallbacks, &vk_renderPass);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_renderPass;
}
inline void														BVE::Vulkan::DestroyRenderPass(VkDevice vk_device, VkRenderPass vk_renderPass, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyRenderPass(vk_device, vk_renderPass, vk_allocationCallbacks);
}

inline VkFramebuffer											BVE::Vulkan::CreateFramebuffer(VkDevice vk_device, VkFramebufferCreateInfo* vk_framebufferCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkFramebuffer vk_framebuffer;

	auto vk_result = vkCreateFramebuffer(vk_device, vk_framebufferCreateInfo, vk_allocationCallbacks, &vk_framebuffer);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_framebuffer;
}
inline void														BVE::Vulkan::DestroyFramebuffer(VkDevice vk_device, VkFramebuffer vk_framebuffer, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyFramebuffer(vk_device, vk_framebuffer, vk_allocationCallbacks);
}

inline VkImage													BVE::Vulkan::CreateImage(VkDevice vk_device, VkImageCreateInfo* vk_imageCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkImage vk_image;

	auto vk_result = vkCreateImage(vk_device, vk_imageCreateInfo, vk_allocationCallbacks, &vk_image);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_image;
}
inline VkMemoryRequirements										BVE::Vulkan::GetImageMemoryRequirements(VkDevice vk_device, VkImage vk_image)
{
	VkMemoryRequirements vk_memoryRequirements;

	vkGetImageMemoryRequirements(vk_device, vk_image, &vk_memoryRequirements);

	return vk_memoryRequirements;
}

inline VkImageView												BVE::Vulkan::CreateImageView(VkDevice vk_device, VkImageViewCreateInfo* vk_imageViewCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkImageView vk_imageView;

	auto vk_result = vkCreateImageView(vk_device, vk_imageViewCreateInfo, vk_allocationCallbacks, &vk_imageView);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_imageView;
}

inline VkSampler												BVE::Vulkan::CreateSampler(VkDevice vk_device, VkSamplerCreateInfo* vk_samplerCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkSampler vk_sampler;

	auto vk_result = vkCreateSampler(vk_device, vk_samplerCreateInfo, vk_allocationCallbacks, &vk_sampler);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		case VK_ERROR_TOO_MANY_OBJECTS:
			throw BVE::Vulkan::Exceptions::Exception_TooManyObjects();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_sampler;
}

inline VkShaderModule											BVE::Vulkan::CreateShaderModule(VkDevice vk_device, VkShaderModuleCreateInfo* vk_shaderModuleCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkShaderModule vk_ShaderModule;

	auto vk_result = vkCreateShaderModule(vk_device, vk_shaderModuleCreateInfo, vk_allocationCallbacks, &vk_ShaderModule);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_ShaderModule;
}

inline VkPipelineCache											BVE::Vulkan::CreatePipelineCache(VkDevice vk_device, VkPipelineCacheCreateInfo* vk_pipelineCacheCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkPipelineCache vk_pipelineCache;

	auto vk_result = vkCreatePipelineCache(vk_device, vk_pipelineCacheCreateInfo, vk_allocationCallbacks, &vk_pipelineCache);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_pipelineCache;
}

inline VkDescriptorSetLayout									BVE::Vulkan::CreateDescriptorSetLayout(VkDevice vk_device, VkDescriptorSetLayoutCreateInfo* vk_descriptorSetLayoutCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkDescriptorSetLayout vk_descriptorSetLayout;

	auto vk_result = vkCreateDescriptorSetLayout(vk_device, vk_descriptorSetLayoutCreateInfo, vk_allocationCallbacks, &vk_descriptorSetLayout);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_descriptorSetLayout;
}

inline VkPipelineLayout											BVE::Vulkan::CreatePipelineLayout(VkDevice vk_device, VkPipelineLayoutCreateInfo* vk_pipelineLayoutCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkPipelineLayout vk_pipelineLayout;

	auto vk_result = vkCreatePipelineLayout(vk_device, vk_pipelineLayoutCreateInfo, vk_allocationCallbacks, &vk_pipelineLayout);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_pipelineLayout;
}

inline std::vector<VkPipeline>									BVE::Vulkan::CreateGraphicsPipelines(VkDevice vk_device, VkPipelineCache& vk_pipelineCache, std::vector<VkGraphicsPipelineCreateInfo>& vk_graphicsPipelineCreateInfos, VkAllocationCallbacks* vk_allocationCallbacks)
{
	std::vector<VkPipeline> vk_pipelines(vk_graphicsPipelineCreateInfos.size());

	auto vk_result = vkCreateGraphicsPipelines(vk_device, vk_pipelineCache, vk_graphicsPipelineCreateInfos.size(), vk_graphicsPipelineCreateInfos.data(), vk_allocationCallbacks, vk_pipelines.data());
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return std::move(vk_pipelines);
}

inline VkDescriptorPool											BVE::Vulkan::CreateDescriptorPool(VkDevice vk_device, VkDescriptorPoolCreateInfo* vk_descriptorPoolCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkDescriptorPool vk_descriptorPool= VK_NULL_HANDLE;

	auto vk_result = vkCreateDescriptorPool(vk_device, vk_descriptorPoolCreateInfo, vk_allocationCallbacks, &vk_descriptorPool);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_descriptorPool;
}

inline std::vector<VkDescriptorSet>								BVE::Vulkan::AllocateDescriptorSets(VkDevice vk_device, VkDescriptorPool vk_descriptorPool, uint32_t count, const VkDescriptorSetLayout* vk_descriptorSetLayout)
{
	VkDescriptorSetAllocateInfo vk_descriptorSetAllocateInfo;
	{
		vk_descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		vk_descriptorSetAllocateInfo.pNext = nullptr;
		vk_descriptorSetAllocateInfo.descriptorPool = vk_descriptorPool;
		vk_descriptorSetAllocateInfo.descriptorSetCount = count;
		vk_descriptorSetAllocateInfo.pSetLayouts = vk_descriptorSetLayout;
	}

	std::vector<VkDescriptorSet> vk_descriptorSets(count);

	auto vk_result = vkAllocateDescriptorSets(vk_device, &vk_descriptorSetAllocateInfo, vk_descriptorSets.data());
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw BVE::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw BVE::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return std::move(vk_descriptorSets);
}

inline uint32_t													BVE::Vulkan::GetCorrectMemoryType(const VkPhysicalDeviceMemoryProperties& vk_physicalDeviceMemoryProperties, uint32_t vk_memoryTypeBits, VkFlags flags)
{
	// returns VkMemoryAllocateInfo.memoryTypeIndex

	//for(size_t i = 0; i < VK_MAX_MEMORY_TYPES; ++i)
	for(size_t i = 0; i < vk_physicalDeviceMemoryProperties.memoryTypeCount; ++i)
	{
		auto bit = ((uint32_t)1 << i);
		if((vk_memoryTypeBits & bit) != 0) // check i-th bit
		{
			if((vk_physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & flags) == flags)
			{
				return i;
			}
		}
	}
	return UINT_MAX; // TODO: throw sheit here
}
#pragma endregion
#pragma region Log
inline void														BVE::Vulkan::Log::Clear()
{
#if __BVE_VULKAN_LOG__
	writeMutex.lock();

	FILE* file = nullptr;

	auto error = fopen_s(&file, logFilename.c_str(), "w");
	if(error != 0)
	{
		// throw Exception();
	}

#if __BVE_VULKAN_DEBUG__
	//if(!file) throw Exception("Can't write log.");
#endif

	auto timestamp = std::to_string(std::clock() - timestampClock);
	string text = "[Log] Cleared";

	fwrite(timestamp.c_str(), sizeof(char), timestamp.size(), file);
	fwrite(": ", sizeof(char), 2, file);
	fwrite(text.c_str(), sizeof(char), text.size(), file);
	fwrite("\n", sizeof(char), 1, file);

	fclose(file);

	timestampClock = std::clock();

	writeMutex.unlock();
#endif
}
inline void														BVE::Vulkan::Log::Write(const string& text)
{
#if __BVE_VULKAN_LOG__
	writeMutex.lock();

	FILE* file = nullptr;

	auto error = fopen_s(&file, logFilename.c_str(), "a");
	if(error != 0)
	{
		throw Exception();
	}

#if __BVE_VULKAN_DEBUG__
	if(!file) throw Exception("Can't write log.");
#endif

	auto timestamp = std::to_string(std::clock() - timestampClock);

	fwrite(timestamp.c_str(), sizeof(char), timestamp.size(), file);
	fwrite(": ", sizeof(char), 2, file);
	fwrite(text.c_str(), sizeof(char), text.size(), file);
	fwrite("\n", sizeof(char), 1, file);
	fclose(file);

	writeMutex.unlock();
#endif
}
#pragma endregion
#pragma endregion

#pragma region
#pragma endregion
