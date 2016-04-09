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

#include <half_float/half.hpp>
//#include <half.hpp>

#define VK_USE_PLATFORM_WIN32_KHR 1
#define VK_PROTOTYPES 1

#include <vulkan/vulkan.h>
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

namespace KID
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
#pragma region Macro
	namespace Macro_Default
	{
#define __KID_BAN_COPY__(Type) \
	public: \
		Type(const Type&) = delete; \
		Type(Type&&) = delete; \
	public: \
		Type& operator = (const Type&) = delete; \
		Type& operator = (Type&&) = delete;
	}
	namespace Macro_Dependency
	{
#define __KID_DEPENDENCY_IMPLEMENTATION__(Dependency) \
	public: \
		class Exception_UndeletedDependentObjects: public Exception {}; \
	protected: \
		std::list<Dependency##Dependent*> dependents; \
	public: \
		inline void AttachDependent(Dependency##Dependent* dependent_) \
		{ \
			dependents.push_back(dependent_); \
		} \
		inline void DetachDependent(Dependency##Dependent* dependent_) \
		{ \
			dependents.remove(dependent_); \
		}
#define __KID_DEPENDENCY_CHECK__ \
		if(dependents.size() > 0) \
		{ \
			throw new Exception_UndeletedDependentObjects(); \
		}
#define __KID_DEPENDENT_DECLARTION__(Dependency)  class Dependency##Dependent;
#define __KID_DEPENDENT_IMPLEMENTATION__(Dependency, dependency) \
		class Dependency##Dependent: \
			public Info \
		{ \
		protected: \
			Dependency*const dependency; \
		public: \
			inline Dependency##Dependent(Dependency* dependency##_): \
			dependency(dependency##_) \
			{ \
				__KID_VULKAN_LOG_BEGIN__(Dependency##Dependent()); \
				dependency->AttachDependent(this); \
				__KID_VULKAN_LOG_END__; \
			} \
			inline ~##Dependency##Dependent() \
			{ \
				__KID_VULKAN_LOG_BEGIN__(~Dependency##Dependent()); \
				dependency->DetachDependent(this); \
				__KID_VULKAN_LOG_END__; \
			} \
		public: \
			inline Dependency* Get##Dependency() const \
			{ \
				return dependency; \
			} \
		public: \
			inline string GetInfoHeader() const \
			{ \
				return "[KID][Vulkan]["#Dependency"Dependent]"; \
			} \
			inline string GetInfo() const \
			{ \
				auto info = Info::GetInfo(); \
				return (info.size() > 0 ? (info + "\n") : "") + #Dependency + " = " + std::to_string((uint64)dependency); \
			} \
		};
	}
	namespace Macro_Readonly
	{
#define __KID_CONST_REFERENCE_READONLY_FIELD__(Type, name, Accessor) \
	protected: \
		Type name; \
	public: \
		inline const Type& Accessor() const \
		{ \
			return name; \
		}
#define __KID_CONST_REFERENCE_READONLY_CONST_FIELD__(Type, name, Accessor) \
	protected: \
		const Type name; \
	public: \
		inline const Type& Accessor() const \
		{ \
			return name; \
		}
	}
#pragma endregion
	namespace Vulkan
	{
#pragma region Preprocessor
	// Is debug mode on?
#ifdef _DEBUG
#define __KID_VULKAN_DEBUG__									1
#else
#define __KID_VULKAN_DEBUG__									0
#endif

// Create log?
#define __KID_VULKAN_LOG__											1
#pragma endregion
#pragma region Macro
		namespace Macro_Info
		{
#define __KID_VULKAN_IMPLEMENT_INFO_HEADER__(Type) \
		public: \
			inline string GetInfoHeader() const \
			{ \
				return "[KID][Vulkan]["#Type"]"; \
			}
		}
		namespace Macro_Log
		{
#define __KID_VULKAN_LOG_BEGIN__(T) \
			Log::Timestamp(); \
			Log::Write(GetInfoHeader() + " ("); \
			Log::Tab();  \
			Log::Write(GetInfo()); \
			Log::Untab(); \
			Log::Write(")"); \
			Log::Write(#T" {"); \
			Log::Tab();
#define __KID_VULKAN_LOG_END__ \
			Log::Untab(); \
			Log::Write("}");
		}
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

		class Info;

		class LayerProperty;
		class InstanceLayersProperties;
		class DeviceLayersProperties;
		class ExtensionProperties;
		class InstanceExtensionsProperties;
		class DeviceExtensionsProperties;

		class Instance; __KID_DEPENDENT_DECLARTION__(Instance);

		class PhysicalDevice; __KID_DEPENDENT_DECLARTION__(PhysicalDevice);

		class Device; __KID_DEPENDENT_DECLARTION__(Device);

		class Surface; __KID_DEPENDENT_DECLARTION__(Surface);

		class Swapchain; __KID_DEPENDENT_DECLARTION__(Swapchain);

		class DeviceMemory; __KID_DEPENDENT_DECLARTION__(DeviceMemory);

		class Queue; __KID_DEPENDENT_DECLARTION__(Queue);

		class Image; __KID_DEPENDENT_DECLARTION__(Image);
		class SwapchainImage; typedef std::vector<SwapchainImage*> SwapchainImages;
		class ImageView; __KID_DEPENDENT_DECLARTION__(ImageView);

		class Sampler; __KID_DEPENDENT_DECLARTION__(Sampler);

		class RenderPass; __KID_DEPENDENT_DECLARTION__(RenderPass);

		class Framebuffer; __KID_DEPENDENT_DECLARTION__(Framebuffer);

		class CommandPool; __KID_DEPENDENT_DECLARTION__(CommandPool);
		class CommandBuffer; __KID_DEPENDENT_DECLARTION__(CommandBuffer);

		class DescriptorPool; __KID_DEPENDENT_DECLARTION__(DescriptorPool);
		class DescriptorSetLayout; __KID_DEPENDENT_DECLARTION__(DescriptorSetLayout);
		class DescriptorSet; __KID_DEPENDENT_DECLARTION__(DescriptorSet);

		class ShaderModule; __KID_DEPENDENT_DECLARTION__(ShaderModule);

		class PipelineLayout; __KID_DEPENDENT_DECLARTION__(PipelineLayout);
		class PipelineCache; __KID_DEPENDENT_DECLARTION__(PipelineCache);
		class Pipeline; __KID_DEPENDENT_DECLARTION__(Pipeline);

		class Buffer; __KID_DEPENDENT_DECLARTION__(Buffer);

		namespace Log
		{
			inline void											Clear();
			inline void											Write(const string& text);
			inline void											Timestamp();
			inline void											Tab();
			inline void											Untab();
		}
#pragma endregion
#pragma region Declaration
		class LayerProperty:
			public VkLayerProperties
		{
		public:
			string		layerName;
			uint32_t    specVersion;
			uint32_t    implementationVersion;
			string		description;
		public:
			inline LayerProperty()
			{
			}
			inline LayerProperty(VkLayerProperties vk_layerProperties):
				layerName(vk_layerProperties.layerName, vk_layerProperties.layerName + strlen(vk_layerProperties.layerName) + 1),
				specVersion(vk_layerProperties.specVersion),
				implementationVersion(vk_layerProperties.implementationVersion),
				description(vk_layerProperties.description, vk_layerProperties.description + strlen(vk_layerProperties.description) + 1)
			{
			}
		};
		class InstanceLayersProperties
		{
		public:
			std::vector<LayerProperty> layersProperties;
			std::vector<const char*> layersName;
		public:
			inline InstanceLayersProperties();
		};
		class DeviceLayersProperties
		{
		public:
			std::vector<LayerProperty> layersProperties;
			std::vector<const char*> layersName;
		public:
			inline DeviceLayersProperties(VkPhysicalDevice vk_physicalDevice);
			inline DeviceLayersProperties(PhysicalDevice* physicalDevice);
		};
		class ExtensionProperties
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
		class InstanceExtensionsProperties
		{
		public:
			std::map<string, std::vector<ExtensionProperties>> layersExtensionsProperties;
			std::vector<const char*> extensionsName;
		public:
			inline InstanceExtensionsProperties(const InstanceLayersProperties& instanceLayersProperties);
		};
		class DeviceExtensionsProperties
		{
		public:
			std::map<string, std::vector<ExtensionProperties>> layersExtensionsProperties;
			std::vector<const char*> extensionsName;
		public:
			inline DeviceExtensionsProperties(VkPhysicalDevice vk_physicalDevice, const DeviceLayersProperties& deviceLayersProperties);
			inline DeviceExtensionsProperties(PhysicalDevice* physicalDevice, DeviceLayersProperties* deviceLayersProperties);
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

		class Info
		{
		public:
			inline string GetInfoHeader() const;
			inline string GetInfo() const;
		};

		class Instance:
			public Info
		{
#pragma region Typedefs
		public:
			typedef const char* LayerName;
			typedef std::vector<LayerName> LayersName;

			typedef const char* ExtensionName;
			typedef std::vector<ExtensionName> ExtensionsName;
#pragma endregion
#pragma region Basic
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkInstance, vk_instance, Vk_GetInstance);
		public:
			Instance(const LayersName& layersName, const ExtensionsName& extensionsName);
			Instance(const Instance&) = delete;
			Instance(Instance&&) = delete;
			~Instance();
		public:
			Instance& operator = (const Instance&) = delete;
			Instance& operator = (Instance&&) = delete;
#pragma endregion
#pragma region Info
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(Instance);
#pragma endregion
#pragma region Debug
#if __KID_VULKAN_DEBUG__
		protected:
			PFN_vkCreateDebugReportCallbackEXT vk_vkCreateDebugReportCallbackEXT;
			PFN_vkDestroyDebugReportCallbackEXT vk_vkDestroyDebugReportCallbackEXT;
			VkDebugReportCallbackEXT vk_debugReportCallbackEXT;
		protected:
			static VkBool32 __stdcall Vk_DebugCallback(
				VkDebugReportFlagsEXT                       flags,
				VkDebugReportObjectTypeEXT                  objectType,
				uint64_t                                    object,
				size_t                                      location,
				int32_t                                     messageCode,
				const char*                                 pLayerPrefix,
				const char*                                 pMessage,
				void*                                       pUserData);
#endif
#pragma endregion
#pragma region Physical Devices
			__KID_CONST_REFERENCE_READONLY_FIELD__(std::vector<PhysicalDevice*>, physicalDevices, GetPhysicalDevices);
#pragma endregion
#pragma region Dependent
			__KID_DEPENDENCY_IMPLEMENTATION__(Instance);
#pragma endregion
		};
		__KID_DEPENDENT_IMPLEMENTATION__(Instance, instance);

		class PhysicalDevice:
			public InstanceDependent
		{
#pragma region Basic
			friend Instance;
		public:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkPhysicalDevice, vk_physicalDevice, Vk_GetPhysicalDevice);
		protected:
			PhysicalDevice(Instance* instance_, VkPhysicalDevice vk_physicalDevice_);
		public:
			PhysicalDevice(const PhysicalDevice&) = default;
			PhysicalDevice(PhysicalDevice&&) = delete;
			~PhysicalDevice();
		public:
			PhysicalDevice& operator = (const PhysicalDevice&) = default;
			PhysicalDevice& operator = (PhysicalDevice&&) = delete;
#pragma endregion
#pragma region Info
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(PhysicalDevice);
#pragma endregion
#pragma region Properties
		public:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkPhysicalDeviceProperties, vk_physicalDeviceProperties, Vk_GetPhysicalDeviceProperties);
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkPhysicalDeviceMemoryProperties, vk_physicalDeviceMemoryProperties, Vk_GetPhysicalDeviceMemoryProperties);
#pragma endregion
#pragma region Dependent
			__KID_DEPENDENCY_IMPLEMENTATION__(PhysicalDevice);
#pragma endregion
		};
		__KID_DEPENDENT_IMPLEMENTATION__(PhysicalDevice, physicalDevice);

		class Device:
			public PhysicalDeviceDependent
		{
#pragma region Typedefs
		public:
			typedef const char* LayerName;
			typedef std::vector<LayerName> LayersName;

			typedef const char* ExtensionName;
			typedef std::vector<ExtensionName> ExtensionsName;
#pragma endregion
#pragma region Basic
		public:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkDevice, vk_device, Vk_GetDevice);
		public:
			Device(PhysicalDevice* physicalDevice_, const LayersName& layersName, const ExtensionsName& extensionsName);
			~Device();
		public:
			inline void WaitIdle();
#pragma endregion
#pragma region Info
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(Device);
#pragma endregion
#pragma region Dependent
			__KID_DEPENDENCY_IMPLEMENTATION__(Device);
#pragma endregion
		};
		__KID_DEPENDENT_IMPLEMENTATION__(Device, device);

		class Surface:
			public PhysicalDeviceDependent
		{
#pragma region Basic
		public:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkSurfaceKHR, vk_surface, Vk_GetSurface);
		public:
			__KID_CONST_REFERENCE_READONLY_FIELD__(HINSTANCE, win_hInstance, Win_GetHInstance);
			__KID_CONST_REFERENCE_READONLY_FIELD__(HWND, win_hWnd, Win_GetHWnd);
		public:
			Surface(PhysicalDevice* physicalDevice_, HINSTANCE hInstance_, HWND hWnd_);
			~Surface();
#pragma endregion
#pragma region Info
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(Surface);
#pragma endregion
#pragma region Properties
		public:
			typedef std::vector<VkPresentModeKHR> PhysicalDeviceSurfacePresentModes;
			typedef std::vector<VkSurfaceFormatKHR> Formats;
		public:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkSurfaceCapabilitiesKHR, vk_surfaceCapabilities, Vk_GetSurfaceCapabilities);
			__KID_CONST_REFERENCE_READONLY_FIELD__(PhysicalDeviceSurfacePresentModes, vk_physicalDeviceSurfacePresentModes, Vk_GetPhysicalDeviceSurfacePresentModes);
			__KID_CONST_REFERENCE_READONLY_FIELD__(Formats, vk_surfaceFormats, Vk_GetSurfaceFormats);
#pragma endregion
#pragma region Dependent
			__KID_DEPENDENCY_IMPLEMENTATION__(Surface);
#pragma endregion
		};
		__KID_DEPENDENT_IMPLEMENTATION__(Surface, surface);

		class Swapchain:
			public DeviceDependent,
			public SurfaceDependent
		{
#pragma region Basic
		public:
			typedef decltype(VkSwapchainCreateInfoKHR::imageFormat) Format;
			typedef decltype(VkSwapchainCreateInfoKHR::imageColorSpace) ColorSpace;
			typedef VkExtent2D Size;
		public:
			class Exception_NotSupported:
				public Exception
			{
			};
		protected:
			__KID_CONST_REFERENCE_READONLY_CONST_FIELD__(uint32_t, vk_queueFamilyIndex, Vk_GetQueueFamilyIndex);
			__KID_CONST_REFERENCE_READONLY_CONST_FIELD__(Format, vk_format, Vk_GetFormat);
			__KID_CONST_REFERENCE_READONLY_CONST_FIELD__(ColorSpace, vk_colorSpace, Vk_GetColorSpace);
			__KID_CONST_REFERENCE_READONLY_CONST_FIELD__(Size, vk_size, Vk_GetSize);
			__KID_CONST_REFERENCE_READONLY_FIELD__(SwapchainImages, images, GetImages);
		protected:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkSwapchainKHR, vk_swapchain, Vk_GetSwapchain);
		public:
			Swapchain(Device* device_, Surface* surface_, uint32_t queueFamilyIndex_, Format vk_format_, ColorSpace vk_colorSpace_, Size vk_size_);
			~Swapchain();
		public:
			inline uint32_t GetNextImage() const;
#pragma endregion
#pragma region Info
		public:
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(Swapchain);
			inline string GetInfo() const;
#pragma endregion
#pragma region Dependent
			__KID_DEPENDENCY_IMPLEMENTATION__(Swapchain);
#pragma endregion
		};
		__KID_DEPENDENT_IMPLEMENTATION__(Swapchain, swapchain);

		class DeviceMemory:
			public DeviceDependent
		{
			__KID_BAN_COPY__(DeviceMemory);
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(DeviceMemory);
			__KID_DEPENDENCY_IMPLEMENTATION__(DeviceMemory);
		public:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkDeviceMemory, vk_deviceMemory, Vk_GetDeviceMemory);
		public:
			DeviceMemory(Image* image, VkMemoryPropertyFlags vk_memoryPropertyFlags = 0);
			DeviceMemory(Buffer* buffer, VkMemoryPropertyFlags vk_memoryPropertyFlags = VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			~DeviceMemory();
		public:
			inline void* Map(uint32_t offset, uint32_t size);
			inline void Unmap();
		};
		__KID_DEPENDENT_IMPLEMENTATION__(DeviceMemory, deviceMemory);
		class DeviceMemoryUser
		{
		protected:
			DeviceMemory* deviceMemory = nullptr;
		public:
			inline DeviceMemoryUser() = default;
			inline DeviceMemoryUser(DeviceMemory* deviceMemory_):
				deviceMemory(deviceMemory_)
			{
			}
			inline ~DeviceMemoryUser() = default;
		public:
			inline void SetDeviceMemory(DeviceMemory* deviceMemory_)
			{
				deviceMemory = deviceMemory_;
			}
			inline DeviceMemory* GetDeviceMemory() const
			{
				return deviceMemory;
			}
		};

		class Queue:
			public DeviceDependent
		{
#pragma region Basic
		protected:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkQueue, vk_queue, Vk_GetQueue);
			__KID_CONST_REFERENCE_READONLY_CONST_FIELD__(uint32_t, vk_queueFamilyIndex, Vk_GetQueueFamilyIndex);
			__KID_CONST_REFERENCE_READONLY_CONST_FIELD__(uint32_t, vk_queueIndex, Vk_GetQueueIndex);
		public:
			Queue(Device* device_, uint32_t queueFamilyIndex_, uint32_t queueIndex_);
			~Queue();
#pragma endregion
#pragma region Info
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(Queue);
#pragma endregion
#pragma region Dependent
			__KID_DEPENDENCY_IMPLEMENTATION__(Queue);
#pragma endregion
#pragma region Methods
		public:
			inline void WaitIdle();
		public:
			inline void Submit(std::vector<CommandBuffer*> commandBuffers, VkPipelineStageFlags vk_pipelineStageFlags);
		public:
			inline void Present(const std::vector<Swapchain*>& swapchains, const std::vector<uint32_t>& imageIndices);
#pragma endregion
		};
		__KID_DEPENDENT_IMPLEMENTATION__(Queue, queue);

		class Image:
			public DeviceDependent,
			public DeviceMemoryUser
		{
#pragma region Typedef
		public:
			typedef VkExtent3D Size;
			typedef VkImageType Type;
			typedef VkFormat Format;
			typedef VkImageTiling Tiling;
			typedef VkImageUsageFlags Usage;
			typedef VkImageUsageFlagBits UsageBits;
			typedef VkImageLayout Layout;
#pragma endregion
#pragma region Basic
		protected:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkImage, vk_image, Vk_GetImage);
			__KID_CONST_REFERENCE_READONLY_FIELD__(Size, size, Vk_GetSize);
			__KID_CONST_REFERENCE_READONLY_CONST_FIELD__(Type, type, Vk_GetType);
			__KID_CONST_REFERENCE_READONLY_CONST_FIELD__(Format, format, Vk_GetFormat);
			__KID_CONST_REFERENCE_READONLY_CONST_FIELD__(Usage, usage, Vk_GetUsage);
		public:
			Image(Device* device_, Size size_, Type type_, Format format_, Tiling tiling_, UsageBits usage_, Layout layout_ = Layout::VK_IMAGE_LAYOUT_UNDEFINED);
			~Image();
		protected:
			Image(Device* device_, VkImage vk_image_, Size size_, Type type_, Format format_, Usage usage_);
#pragma endregion
#pragma region Info
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(Image);
#pragma endregion
#pragma region Dependent
			__KID_DEPENDENCY_IMPLEMENTATION__(Image);
#pragma endregion
#pragma region Methods
			inline void SetDeviceMemory(DeviceMemory* deviceMemory_);
#pragma endregion
		};
		__KID_DEPENDENT_IMPLEMENTATION__(Image, image);
		class SwapchainImage:
			public SwapchainDependent,
			public Image
		{
#pragma region Basic
		public:
			SwapchainImage(Swapchain* swapchain_, VkImage vk_image_, Size size_, Type type_, Format format_, Usage usage_);
			~SwapchainImage();
#pragma endregion
#pragma region Info
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(SwapchainImage);
			inline string GetInfo() const;
#pragma endregion
		};
		class ImageView:
			public ImageDependent
		{
#pragma region Typedef
		public:
			typedef VkImageViewType Type;
			typedef VkImageAspectFlags Aspect;
			typedef VkImageAspectFlagBits AspectBits;
#pragma endregion
#pragma region Basic
		protected:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkImageView, vk_imageView, Vk_GetImageView);
			__KID_CONST_REFERENCE_READONLY_CONST_FIELD__(Type, vk_imageViewType, Vk_GetImageViewType);
			__KID_CONST_REFERENCE_READONLY_CONST_FIELD__(Aspect, vk_imageAspect, Vk_GetImageAspect);
		public:
			ImageView(Image* image_, Type type_, Aspect aspect_);
			~ImageView();
#pragma endregion
#pragma region Info
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(ImageView);
#pragma endregion
#pragma region Dependent
			__KID_DEPENDENCY_IMPLEMENTATION__(ImageView);
#pragma endregion
		};
		__KID_DEPENDENT_IMPLEMENTATION__(ImageView, imageView);

		class Sampler:
			public DeviceDependent
		{
		public:
			typedef VkFilter Filter;
			typedef VkSamplerMipmapMode MipmapMode;
			typedef VkSamplerAddressMode AddressMode;
		public:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkSampler, vk_sampler, Vk_GetSampler);
			__KID_CONST_REFERENCE_READONLY_CONST_FIELD__(Filter, vk_minFilter, Vk_GetMinFilter);
			__KID_CONST_REFERENCE_READONLY_CONST_FIELD__(Filter, vk_magFilter, Vk_GetMagFilter);
			__KID_CONST_REFERENCE_READONLY_CONST_FIELD__(MipmapMode, vk_mipmapMode, Vk_GetMipmapMode);
			__KID_CONST_REFERENCE_READONLY_CONST_FIELD__(AddressMode, vk_addressModeU, Vk_GetAddressModeU);
			__KID_CONST_REFERENCE_READONLY_CONST_FIELD__(AddressMode, vk_addressModeV, Vk_GetAddressModeV);
			__KID_CONST_REFERENCE_READONLY_CONST_FIELD__(AddressMode, vk_addressModeW, Vk_GetAddressModeW);
		public:
			Sampler(Device* device_,
				Filter vk_minFilter_, Filter vk_magFilter_,
				MipmapMode vk_mipmapMode_,
				AddressMode vk_addressModeU_, AddressMode vk_addressModeV_, AddressMode vk_addressModeW_
			);
			~Sampler();
		public:
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(Sampler);
			__KID_DEPENDENCY_IMPLEMENTATION__(Sampler);
		};
		__KID_DEPENDENT_IMPLEMENTATION__(Sampler, sampler);

		class RenderPass:
			public DeviceDependent
		{
#pragma region Typedef
		public:
			typedef VkRect2D Rect;
			typedef VkFormat Format;
#pragma endregion
#pragma region Basic
		public:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkRenderPass, vk_renderPass, Vk_GetRenderPass);
		public:
			RenderPass(Device* device_, const std::vector<Format>& colorFormats_, const std::vector<Format>& depthFormats_);
			~RenderPass();
#pragma endregion
#pragma region Info
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(RenderPass);
#pragma endregion
#pragma region Dependent
			__KID_DEPENDENCY_IMPLEMENTATION__(RenderPass);
#pragma endregion
#pragma region Methods
			inline void Begin(CommandBuffer* commandBuffer, Framebuffer* framebuffer, const Rect& renderArea, const std::vector<VkClearValue>& clearValues, VkSubpassContents vk_subpassContents);
			inline void End(CommandBuffer* commandBuffer);
#pragma endregion
		};
		__KID_DEPENDENT_IMPLEMENTATION__(RenderPass, renderPass);

		class Framebuffer:
			public RenderPassDependent
		{
#pragma region Basic
		public:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkFramebuffer, vk_framebuffer, Vk_GetFramebuffer);
		public:
			Framebuffer(RenderPass* renderPass_, const std::vector<ImageView*>& colorAttachments_, ImageView* depthAttachment_);
			~Framebuffer();
#pragma endregion
#pragma region Info
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(Framebuffer);
#pragma endregion
#pragma region Dependent
			__KID_DEPENDENCY_IMPLEMENTATION__(Framebuffer);
#pragma endregion
		};
		__KID_DEPENDENT_IMPLEMENTATION__(Framebuffer, framebuffer);

		class CommandPool:
			public DeviceDependent
		{
#pragma region Basic
		public:
			__KID_CONST_REFERENCE_READONLY_CONST_FIELD__(uint32_t, vk_queueFamilyIndex, Vk_GetQueueFamilyIndex);
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkCommandPool, vk_commandPool, Vk_GetCommandPool);
		public:
			CommandPool(Device* device_, uint32_t vk_queueFamilyIndex_);
			~CommandPool();
		public:
			inline void Reset();
#pragma endregion
#pragma region Info
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(CommandPool);
#pragma endregion
#pragma region Dependent
			__KID_DEPENDENCY_IMPLEMENTATION__(CommandPool);
#pragma endregion
		};
		__KID_DEPENDENT_IMPLEMENTATION__(CommandPool, commandPool);
		class CommandBuffer:
			public CommandPoolDependent
		{
#pragma region Typedef
		public:
			typedef VkCommandBufferLevel Level;
#pragma endregion
#pragma region Basic
		public:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkCommandBuffer, vk_commandBuffer, Vk_GetCommandBuffer);
			__KID_CONST_REFERENCE_READONLY_CONST_FIELD__(Level, commandBufferLevel, Vk_GetCommandBufferLevel);
		public:
			CommandBuffer(CommandPool* commandPool_, Level commandBufferLevel_);
			~CommandBuffer();
		public:
			inline void Reset();
			inline void Begin(RenderPass* renderPass_, uint32_t subpass_, Framebuffer* framebuffer, bool occlusionQuery_, VkQueryControlFlags vk_queryControlFlags = 0);
			inline void End();
#pragma endregion
#pragma region Info
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(CommandBuffer);
#pragma endregion
#pragma region Dependent
			__KID_DEPENDENCY_IMPLEMENTATION__(CommandBuffer);
#pragma endregion
		};
		__KID_DEPENDENT_IMPLEMENTATION__(CommandBuffer, commandBuffer);

		class DescriptorPool:
			public DeviceDependent
		{
		public:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkDescriptorPool, vk_descriptorPool, Vk_GetDescriptorPool);
		public:
			DescriptorPool(Device* device_, const std::vector<VkDescriptorPoolSize>& descriptorPoolSizes);
			~DescriptorPool();
		public:
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(DescriptorPool);
			__KID_DEPENDENCY_IMPLEMENTATION__(DescriptorPool);
		};
		__KID_DEPENDENT_IMPLEMENTATION__(DescriptorPool, descriptorPool);
		class DescriptorSetLayout:
			public DeviceDependent
		{
		public:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkDescriptorSetLayout, vk_descriptorSetLayout, Vk_GetDescriptorSetLayout);
		public:
			DescriptorSetLayout(Device* device_, const std::vector<VkDescriptorSetLayoutBinding>& vk_descriptorSetLayoutBindings);
			~DescriptorSetLayout();
		public:
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(DescriptorSetLayout);
			__KID_DEPENDENCY_IMPLEMENTATION__(DescriptorSetLayout);
		};
		__KID_DEPENDENT_IMPLEMENTATION__(DescriptorSetLayout, descriptorSetLayout);
		class DescriptorSet:
			public DescriptorPoolDependent,
			public DescriptorSetLayoutDependent
		{
		public:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkDescriptorSet, vk_descriptorSet, Vk_GetDescriptorSet);
		public:
			DescriptorSet(DescriptorPool* descriptorPool_, DescriptorSetLayout* descriptorSetLayout_);
			~DescriptorSet();
		public:
			__KID_DEPENDENCY_IMPLEMENTATION__(DescriptorSet);
		public:
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(DescriptorSet);
			inline string GetInfo() const;
		};
		__KID_DEPENDENT_IMPLEMENTATION__(DescriptorSet, descriptorSet);

		class ShaderModule:
			public DeviceDependent
		{
		public:
			typedef std::vector<uint32_t> SourceCode;
		public:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkShaderModule, vk_shaderModule, Vk_GetShaderModule);
		public:
			ShaderModule(Device* device_, const SourceCode& sourceCode);
			~ShaderModule();
		public:
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(ShaderModule);
			__KID_DEPENDENCY_IMPLEMENTATION__(ShaderModule);
		};
		__KID_DEPENDENT_IMPLEMENTATION__(ShaderModule, shaderModule);

		class PipelineLayout:
			public DeviceDependent
		{
		public:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkPipelineLayout, vk_pipelineLayout, Vk_GetPipelineLayout);
		public:
			PipelineLayout(Device* device_, const std::vector<DescriptorSetLayout*>& descriptorSetLayouts);
			~PipelineLayout();
		public:
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(PipelineLayout);
			__KID_DEPENDENCY_IMPLEMENTATION__(PipelineLayout);
		};
		__KID_DEPENDENT_IMPLEMENTATION__(PipelineLayout, pipelineLayout);
		class PipelineCache:
			public DeviceDependent
		{
		public:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkPipelineCache, vk_pipelineCache, Vk_GetPipelineCache);
		public:
			PipelineCache(Device* device_);
			~PipelineCache();
		public:
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(PipelineCache);
			__KID_DEPENDENCY_IMPLEMENTATION__(PipelineCache);
		};
		__KID_DEPENDENT_IMPLEMENTATION__(PipelineCache, pipelineCache);
		class Pipeline:
			public DeviceDependent,
			public RenderPassDependent,
			public PipelineLayoutDependent,
			public PipelineCacheDependent
		{
		public:
			typedef std::vector<VkViewport> Viewports;
			typedef std::vector<VkRect2D> Scissors;

			typedef VkVertexInputBindingDescription VertexBindings;
			typedef std::vector<VertexBindings> VerticesBindings;
			typedef VkVertexInputAttributeDescription VertexAttributes;
			typedef std::vector<VertexAttributes> VerticesAttributes;

			typedef VkPrimitiveTopology Topology;

			typedef VkPolygonMode Fill;
			typedef VkCullModeFlags Cull; typedef VkCullModeFlagBits Culls;
			typedef VkFrontFace Front;

			typedef VkCompareOp Comp;

		public:
			struct UniformBufferBindingDescription
			{
				Buffer* buffer;
				uint32_t binding;
				uint32_t offset;
				uint32_t range;
			};
			struct ShaderStageInfo
			{
				ShaderModule* shader;
				std::vector<UniformBufferBindingDescription> uniformBufferBindingDescriptions;
			};
			struct ShaderModules
			{
				ShaderStageInfo vertex;
				ShaderStageInfo tessellationControl;
				ShaderStageInfo tessellationEvaluation;
				ShaderStageInfo geometry;
				ShaderStageInfo fragment;
			};
		public:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkDescriptorSetLayout, vk_descriptorSetLayout, Vk_GetDescriptorSetLayout);
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkPipelineLayout, vk_pipelineLayout, Vk_GetPipelineLayout);
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkPipelineCache, vk_pipelineCache, Vk_GetPipelineCache);
			__KID_CONST_REFERENCE_READONLY_FIELD__(Viewports, vk_viewports, Vk_GetViewports);
			__KID_CONST_REFERENCE_READONLY_FIELD__(Scissors, vk_scissors, Vk_GetScissors);
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkPipeline, vk_pipeline, Vk_GetPipeline);
		public:
			Pipeline(
				Device* device_, RenderPass* renderPass_, PipelineLayout* pipelineLayout_, PipelineCache* pipelineCache_,
				const ShaderModules& shaderModules_,
				const VerticesBindings& verticesBindings,
				const VerticesAttributes& verticesAttributes,
				Topology topology, VkBool32 primitiveRestart,
				const Viewports& viewports_, const Scissors& scissors_,
				Fill fill, Cull cull, Front front
			);
			~Pipeline();
		public:
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(Pipeline);
			inline string GetInfo() const;
		public:
			__KID_DEPENDENCY_IMPLEMENTATION__(Pipeline);
		};
		__KID_DEPENDENT_IMPLEMENTATION__(Pipeline, pipeline);

		class Buffer:
			public DeviceDependent,
			public DeviceMemoryUser
		{
		public:
			typedef VkBufferUsageFlags Usage;
			typedef VkBufferUsageFlagBits Usages;
		public:
			__KID_CONST_REFERENCE_READONLY_FIELD__(VkBuffer, vk_buffer, Vk_GetBuffer);
			__KID_CONST_REFERENCE_READONLY_FIELD__(Usage, vk_usage, Vk_GetUsage);
			__KID_CONST_REFERENCE_READONLY_FIELD__(uint32_t, size, GetSize);
		public:
			Buffer(Device* device_, uint32_t size_, Usage usage_);
			~Buffer();
		public:
			__KID_VULKAN_IMPLEMENT_INFO_HEADER__(Buffer);
			__KID_DEPENDENCY_IMPLEMENTATION__(Buffer);
		public:
			inline void SetDeviceMemory(DeviceMemory* deviceMemory_);
		};
		__KID_DEPENDENT_IMPLEMENTATION__(Buffer, buffer);

#pragma endregion
#pragma region Func
		inline VkClearValue										ClearColorf(float r, float g, float b, float a);
		inline VkClearValue										ClearColori(int32_t r, int32_t g, int32_t b, int32_t a);
		inline VkClearValue										ClearColorui(uint32_t r, uint32_t g, uint32_t b, uint32_t a);
		inline VkClearValue										ClearDepthStencil(float depth, uint32_t stencil);

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
		inline void												DestroySurfaceKHR(VkInstance vk_instance, VkSurfaceKHR vk_surface, VkAllocationCallbacks* vk_allocationCallbacks);
		inline bool												GetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice vk_physicalDevice, uint32_t vk_queueFamilyIndex, VkSurfaceKHR vk_surface);

		inline std::vector<VkSurfaceFormatKHR>					GetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice vk_physicalDevice, VkSurfaceKHR vk_surface);
		inline VkSurfaceCapabilitiesKHR							GetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice vk_physicalDevice, VkSurfaceKHR vk_surface);
		inline std::vector<VkPresentModeKHR>					GetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice vk_physicalDevice, VkSurfaceKHR vk_surface);

		inline VkDevice											CreateDevice(VkPhysicalDevice vk_physicalDevice, VkDeviceCreateInfo* vk_deviceCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyDevice(VkDevice vk_device, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DeviceWaitIdle(VkDevice vk_device);

		inline VkDeviceMemory									AllocateMemory(VkDevice vk_device, VkMemoryAllocateInfo* vk_memoryAllocateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												FreeMemory(VkDevice vk_device, VkDeviceMemory vk_deviceMemory, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												BindImageMemory(VkDevice vk_device, VkImage vk_image, VkDeviceMemory vk_deviceMemory, VkDeviceSize vk_deviceSize);
		inline void												BindBufferMemory(VkDevice vk_device, VkBuffer vk_buffer, VkDeviceMemory vk_deviceMemory, VkDeviceSize vk_memoryOffset);
		inline void*											MapMemory(VkDevice vk_device, VkDeviceMemory vk_deviceMemory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags vk_memoryMapFlags);
		inline void												UnmapMemory(VkDevice vk_device, VkDeviceMemory vk_deviceMemory);

		inline VkSwapchainKHR									CreateSwapchainKHR(VkDevice vk_device, VkSwapchainCreateInfoKHR* vk_SwapchainCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline std::vector<VkImage>								GetSwapchainImagesKHR(VkDevice vk_device, VkSwapchainKHR vk_swapchain);
		inline uint32_t											AcquireNextImageKHR(VkDevice vk_device, VkSwapchainKHR vk_swapchain, uint64_t timeout, VkSemaphore vk_semaphore, VkFence vk_fence);
		inline void												DestroySwapchainKHR(VkDevice vk_device, VkSwapchainKHR vk_swapchain, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkQueue											GetDeviceQueue(VkDevice vk_device, uint32_t vk_queueFamilyIndex, uint32_t vk_queueIndex);
		inline void												QueueSubmit(VkQueue vk_queue, std::vector<VkSubmitInfo>& vk_submitInfos, VkFence vk_fence);
		inline void												QueueWaitIdle(VkQueue vk_queue);
		inline void												QueuePresentKHR(VkQueue vk_queue, VkPresentInfoKHR* vk_presentInfoKHR);

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
		inline void												DestroyBuffer(VkDevice vk_device, VkBuffer vk_buffer, VkAllocationCallbacks* vk_allocationCallbacks);
		inline VkMemoryRequirements								GetBufferMemoryRequirements(VkDevice vk_device, VkBuffer vk_buffer);


		inline VkRenderPass										CreateRenderPass(VkDevice vk_device, VkRenderPassCreateInfo* vk_renderPassCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyRenderPass(VkDevice vk_device, VkRenderPass vk_renderPass, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkFramebuffer									CreateFramebuffer(VkDevice vk_device, VkFramebufferCreateInfo* vk_framebufferCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyFramebuffer(VkDevice vk_device, VkFramebuffer vk_framebuffer, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkImage											CreateImage(VkDevice vk_device, VkImageCreateInfo* vk_imageCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline VkMemoryRequirements								GetImageMemoryRequirements(VkDevice vk_device, VkImage vk_image);
		inline void												DestroyImage(VkDevice vk_device, VkImage vk_image, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkImageView										CreateImageView(VkDevice vk_device, VkImageViewCreateInfo* vk_imageViewCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyImageView(VkDevice vk_device, VkImageView vk_imageView, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkSampler										CreateSampler(VkDevice vk_device, VkSamplerCreateInfo* vk_samplerCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroySampler(VkDevice vk_device, VkSampler vk_sampler, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkShaderModule									CreateShaderModule(VkDevice vk_device, VkShaderModuleCreateInfo* vk_shaderModuleCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyShaderModule(VkDevice vk_device, VkShaderModule vk_shaderModule, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkPipelineCache									CreatePipelineCache(VkDevice vk_device, VkPipelineCacheCreateInfo* vk_pipelineCacheCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyPipelineCache(VkDevice vk_device, VkPipelineCache vk_pipelineCache, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkDescriptorSetLayout							CreateDescriptorSetLayout(VkDevice vk_device, VkDescriptorSetLayoutCreateInfo* vk_descriptorSetLayoutCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyDescriptorSetLayout(VkDevice vk_device, VkDescriptorSetLayout vk_descriptorSetLayout, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkPipelineLayout									CreatePipelineLayout(VkDevice vk_device, VkPipelineLayoutCreateInfo* vk_pipelineLayoutCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyPipelineLayout(VkDevice vk_device, VkPipelineLayout vk_pipelineLayout, VkAllocationCallbacks* vk_allocationCallbacks);

		inline std::vector<VkPipeline>							CreateGraphicsPipelines(VkDevice vk_device, VkPipelineCache& vk_pipelineCache, std::vector<VkGraphicsPipelineCreateInfo>& vk_graphicsPipelineCreateInfos, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyPipeline(VkDevice vk_device, VkPipeline vk_pipeline, VkAllocationCallbacks* vk_allocationCallbacks);

		inline VkDescriptorPool									CreateDescriptorPool(VkDevice vk_device, VkDescriptorPoolCreateInfo* vk_descriptorPoolCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks);
		inline void												DestroyDescriptorPool(VkDevice vk_device, VkDescriptorPool vk_descriptorPool, VkAllocationCallbacks* vk_allocationCallbacks);

		inline std::vector<VkDescriptorSet>						AllocateDescriptorSets(VkDevice vk_device, VkDescriptorPool vk_descriptorPool, uint32_t count, const VkDescriptorSetLayout* vk_descriptorSetLayout);
		inline void												FreeDescriptorSets(VkDevice vk_device, VkDescriptorPool vk_descriptorPool, const std::vector<VkDescriptorSet>& vk_descriptorSets);

		inline uint32_t											GetCorrectMemoryType(const VkPhysicalDeviceMemoryProperties& vk_physicalDeviceMemoryProperties, uint32_t vk_memoryTypeBits, VkFlags flags);
		
		void													ChangeImageLayout(Queue* queue, CommandPool* commandPool, Image* image, VkAccessFlags vk_srcAccessFlags, VkAccessFlags vk_dstAccessFlags, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags vk_imageAspectFlags);
#pragma endregion
#pragma region Log
		namespace Log
		{
			extern size_t										tab;
			extern std::mutex									writeMutex;
			extern std::clock_t									timestampClock;
		}
#pragma endregion
	}
}

#pragma region
#pragma endregion

#pragma region Vulkan
#pragma region Definition
#pragma region Exception
inline															KID::Vulkan::Exception::Exception(const string& text_):
	text(text_)
{
}
#pragma endregion
#pragma region Exceptions
#pragma region Exception_UnknownException
inline															KID::Vulkan::Exceptions::Exception_UnknownException::Exception_UnknownException(VkResult vk_result_):
	Exception(),
	vk_result(vk_result_)
{
}
#pragma endregion

#pragma region Exception_OutOfHostMemory
inline															KID::Vulkan::Exceptions::Exception_OutOfHostMemory::Exception_OutOfHostMemory():
	Exception()
{
}
#pragma endregion
#pragma region
inline															KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory::Exception_OutOfDeviceMemory():
	Exception()
{
}
#pragma endregion
#pragma region Exception_InitializationFailed
inline															KID::Vulkan::Exceptions::Exception_InitializationFailed::Exception_InitializationFailed():
	Exception()
{
}
#pragma endregion
#pragma region Exception_DeviceLost
inline															KID::Vulkan::Exceptions::Exception_DeviceLost::Exception_DeviceLost():
	Exception()
{
}
#pragma endregion
#pragma region Exception_MemoryMapFailed
inline															KID::Vulkan::Exceptions::Exception_MemoryMapFailed::Exception_MemoryMapFailed():
	Exception()
{
}
#pragma endregion
#pragma region Exception_LayerNotPresent
inline															KID::Vulkan::Exceptions::Exception_LayerNotPresent::Exception_LayerNotPresent():
	Exception()
{
}
#pragma endregion
#pragma region Exception_ExtensionNotPresent
inline															KID::Vulkan::Exceptions::Exception_ExtensionNotPresent::Exception_ExtensionNotPresent():
	Exception()
{
}
#pragma endregion
#pragma region Exception_FeatureNotPresent
inline															KID::Vulkan::Exceptions::Exception_FeatureNotPresent::Exception_FeatureNotPresent():
	Exception()
{
}
#pragma endregion
#pragma region Exception_IncompatibleDriver
inline															KID::Vulkan::Exceptions::Exception_IncompatibleDriver::Exception_IncompatibleDriver():
	Exception()
{
}
#pragma endregion
#pragma region Exception_TooManyObjects
inline															KID::Vulkan::Exceptions::Exception_TooManyObjects::Exception_TooManyObjects():
	Exception()
{
}
#pragma endregion
#pragma region Exception_FormatNotSupported
inline															KID::Vulkan::Exceptions::Exception_FormatNotSupported::Exception_FormatNotSupported():
	Exception()
{
}
#pragma endregion
#pragma region Exception_SurfaceLostKHR
inline															KID::Vulkan::Exceptions::Exception_SurfaceLostKHR::Exception_SurfaceLostKHR():
	Exception()
{
}
#pragma endregion
#pragma region Exception_NativeWindowInUseKHR
inline															KID::Vulkan::Exceptions::Exception_NativeWindowInUseKHR::Exception_NativeWindowInUseKHR():
	Exception()
{
}
#pragma endregion
#pragma region Exception_OutOfDateKHR
inline															KID::Vulkan::Exceptions::Exception_OutOfDateKHR::Exception_OutOfDateKHR():
	Exception()
{
}
#pragma endregion
#pragma region Exception_IncompatibleDisplayKHR
inline															KID::Vulkan::Exceptions::Exception_IncompatibleDisplayKHR::Exception_IncompatibleDisplayKHR():
	Exception()
{
}
#pragma endregion
#pragma region Exception_ValidationFailedExt
inline															KID::Vulkan::Exceptions::Exception_ValidationFailedExt::Exception_ValidationFailedExt():
	Exception()
{
}
#pragma endregion

inline void														KID::Vulkan::Exceptions::Perform(VkResult vk_result)
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
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		case VK_ERROR_INITIALIZATION_FAILED:
			throw KID::Vulkan::Exceptions::Exception_InitializationFailed();
		case VK_ERROR_DEVICE_LOST:
			throw KID::Vulkan::Exceptions::Exception_DeviceLost();
		case VK_ERROR_MEMORY_MAP_FAILED:
			throw KID::Vulkan::Exceptions::Exception_MemoryMapFailed();
		case VK_ERROR_LAYER_NOT_PRESENT:
			throw KID::Vulkan::Exceptions::Exception_LayerNotPresent();
		case VK_ERROR_EXTENSION_NOT_PRESENT:
			throw KID::Vulkan::Exceptions::Exception_ExtensionNotPresent();
		case VK_ERROR_FEATURE_NOT_PRESENT:
			throw KID::Vulkan::Exceptions::Exception_FeatureNotPresent();
		case VK_ERROR_INCOMPATIBLE_DRIVER:
			throw KID::Vulkan::Exceptions::Exception_IncompatibleDriver();
		case VK_ERROR_TOO_MANY_OBJECTS:
			throw KID::Vulkan::Exceptions::Exception_TooManyObjects();
		case VK_ERROR_FORMAT_NOT_SUPPORTED:
			throw KID::Vulkan::Exceptions::Exception_FormatNotSupported();
		case VK_ERROR_SURFACE_LOST_KHR:
			throw KID::Vulkan::Exceptions::Exception_SurfaceLostKHR();
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:
			throw KID::Vulkan::Exceptions::Exception_NativeWindowInUseKHR();
		case VK_ERROR_OUT_OF_DATE_KHR:
			throw KID::Vulkan::Exceptions::Exception_OutOfDateKHR();
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:
			throw KID::Vulkan::Exceptions::Exception_IncompatibleDisplayKHR();
		case VK_ERROR_VALIDATION_FAILED_EXT:
			throw KID::Vulkan::Exceptions::Exception_ValidationFailedExt();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}
}
#pragma endregion

#pragma region Info
inline KID::string KID::Vulkan::Info::GetInfoHeader() const
{
	return "[KID][Vulkan][Info]";
}
inline KID::string KID::Vulkan::Info::GetInfo() const
{
	return "this = " + std::to_string((uint64)this);
}
#pragma endregion

#pragma region InstanceLayersProperties
inline KID::Vulkan::InstanceLayersProperties::InstanceLayersProperties()
{
	auto vk_instanceLayersProperties = EnumerateInstanceLayerProperties();

	layersProperties.resize(vk_instanceLayersProperties.size());
	for(size_t i = 0; i < vk_instanceLayersProperties.size(); ++i)
	{
		layersProperties[i] = LayerProperty(vk_instanceLayersProperties[i]);
		layersName.push_back(layersProperties[i].layerName.c_str());
	}
}
#pragma endregion
#pragma region DeviceLayersProperties
inline KID::Vulkan::DeviceLayersProperties::DeviceLayersProperties(VkPhysicalDevice vk_physicalDevice)
{
	auto vk_deviceLayersProperties = EnumerateDeviceLayerProperties(vk_physicalDevice);

	layersProperties.resize(vk_deviceLayersProperties.size());
	for(size_t i = 0; i < vk_deviceLayersProperties.size(); ++i)
	{
		layersProperties[i] = LayerProperty(vk_deviceLayersProperties[i]);
		layersName.push_back(layersProperties[i].layerName.c_str());
	}
}
inline KID::Vulkan::DeviceLayersProperties::DeviceLayersProperties(PhysicalDevice* physicalDevice)
{
	auto vk_deviceLayersProperties = EnumerateDeviceLayerProperties(physicalDevice->Vk_GetPhysicalDevice());

	layersProperties.resize(vk_deviceLayersProperties.size());
	for(size_t i = 0; i < vk_deviceLayersProperties.size(); ++i)
	{
		layersProperties[i] = LayerProperty(vk_deviceLayersProperties[i]);
		layersName.push_back(layersProperties[i].layerName.c_str());
	}
}
#pragma endregion
#pragma region InstanceExtensionsProperties
inline KID::Vulkan::InstanceExtensionsProperties::InstanceExtensionsProperties(const InstanceLayersProperties& instanceLayersProperties)
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
#pragma endregion
#pragma region DeviceExtensionsProperties
inline KID::Vulkan::DeviceExtensionsProperties::DeviceExtensionsProperties(VkPhysicalDevice vk_physicalDevice, const DeviceLayersProperties& deviceLayersProperties)
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
inline KID::Vulkan::DeviceExtensionsProperties::DeviceExtensionsProperties(PhysicalDevice* physicalDevice, DeviceLayersProperties* deviceLayersProperties)
{
	for(auto &layerProperties : deviceLayersProperties->layersProperties)
	{
		auto &layersExtensionProperties = layersExtensionsProperties[layerProperties.layerName];

		auto vk_deviceExtensionsProperties = EnumerateDeviceExtensionProperties(physicalDevice->Vk_GetPhysicalDevice(), layerProperties.layerName.c_str());

		layersExtensionProperties.resize(vk_deviceExtensionsProperties.size());
		for(size_t i = 0; i < vk_deviceExtensionsProperties.size(); ++i)
		{
			layersExtensionProperties[i] = ExtensionProperties(vk_deviceExtensionsProperties[i]);
			extensionsName.push_back(layersExtensionProperties[i].extensionName.c_str());
		}
	}

	auto &layersExtensionProperties = layersExtensionsProperties[""];

	auto vk_deviceExtensionsProperties = EnumerateDeviceExtensionProperties(physicalDevice->Vk_GetPhysicalDevice(), "");

	layersExtensionProperties.resize(vk_deviceExtensionsProperties.size());
	for(size_t i = 0; i < vk_deviceExtensionsProperties.size(); ++i)
	{
		layersExtensionProperties[i] = ExtensionProperties(vk_deviceExtensionsProperties[i]);
		extensionsName.push_back(layersExtensionProperties[i].extensionName.c_str());
	}
}
#pragma endregion


#pragma region Device
inline void KID::Vulkan::Device::WaitIdle()
{
	DeviceWaitIdle(vk_device);
}
#pragma endregion

#pragma region Swapchain
inline uint32_t KID::Vulkan::Swapchain::GetNextImage() const
{
	//auto nextImage = KID::Vulkan::AcquireNextImageKHR(device->Vk_GetDevice(), vk_swapchain, UINT64_MAX, VK_NULL_HANDLE, VK_NULL_HANDLE);
	//
	//Log::Timestamp();
	//Log::Write(GetInfoHeader());
	//Log::Write(GetInfo());
	//Log::Write("GetNextImage return: " + std::to_string(nextImage));
	//
	//return nextImage;

	return KID::Vulkan::AcquireNextImageKHR(device->Vk_GetDevice(), vk_swapchain, UINT64_MAX, VK_NULL_HANDLE, VK_NULL_HANDLE);
}
inline KID::string KID::Vulkan::Swapchain::GetInfo() const
{
	return DeviceDependent::GetInfo() + "\n" + SurfaceDependent::GetInfo();
}
#pragma endregion

#pragma region DeviceMemory
inline void* KID::Vulkan::DeviceMemory::Map(uint32_t offset, uint32_t size)
{
	return MapMemory(device->Vk_GetDevice(), vk_deviceMemory, offset, size, 0);
}
inline void	KID::Vulkan::DeviceMemory::Unmap()
{
	UnmapMemory(device->Vk_GetDevice(), vk_deviceMemory);
}
#pragma endregion

#pragma region Queue
inline void KID::Vulkan::Queue::WaitIdle()
{
	QueueWaitIdle(vk_queue);
}
inline void KID::Vulkan::Queue::Submit(std::vector<CommandBuffer*> commandBuffers, VkPipelineStageFlags vk_pipelineStageFlags)
{
	__KID_VULKAN_LOG_BEGIN__(Submit());

	std::vector<VkCommandBuffer> vk_commandBuffers(commandBuffers.size());
	for(size_t i = 0; i < vk_commandBuffers.size(); ++i)
	{
		vk_commandBuffers[i] = commandBuffers[i]->Vk_GetCommandBuffer();
	}

	std::vector<VkSubmitInfo> vk_submitInfos(1);
	{
		vk_submitInfos[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		vk_submitInfos[0].pNext = nullptr;
		vk_submitInfos[0].waitSemaphoreCount = 0;
		vk_submitInfos[0].pWaitSemaphores = nullptr;
		vk_submitInfos[0].pWaitDstStageMask = &vk_pipelineStageFlags;
		vk_submitInfos[0].commandBufferCount = vk_commandBuffers.size();
		vk_submitInfos[0].pCommandBuffers = vk_commandBuffers.data();
		vk_submitInfos[0].signalSemaphoreCount = 0;
		vk_submitInfos[0].pSignalSemaphores = nullptr;
	}

	QueueSubmit(vk_queue, vk_submitInfos, VK_NULL_HANDLE);

	__KID_VULKAN_LOG_END__;
}
inline void KID::Vulkan::Queue::Present(const std::vector<Swapchain*>& swapchains, const std::vector<uint32_t>& imageIndices)
{
	__KID_VULKAN_LOG_BEGIN__(Present());

	std::vector<VkSwapchainKHR> vk_swapchains(swapchains.size());
	for(size_t i = 0; i < vk_swapchains.size(); ++i)
	{
		vk_swapchains[i] = swapchains[i]->Vk_GetSwapchain();
	}

	std::vector<VkResult> vk_results(swapchains.size());
	VkPresentInfoKHR vk_presentInfoKHR;
	{
		vk_presentInfoKHR.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		vk_presentInfoKHR.pNext = nullptr;
		vk_presentInfoKHR.waitSemaphoreCount = 0;
		vk_presentInfoKHR.pWaitSemaphores = nullptr;
		vk_presentInfoKHR.swapchainCount = vk_swapchains.size();
		vk_presentInfoKHR.pSwapchains = vk_swapchains.data();
		vk_presentInfoKHR.pImageIndices = imageIndices.data();
		vk_presentInfoKHR.pResults = vk_results.data();
	}

	QueuePresentKHR(vk_queue, &vk_presentInfoKHR);

	for(auto &vk_result : vk_results)
	{
		Exceptions::Perform(vk_result);
	}

	__KID_VULKAN_LOG_END__;
}
#pragma endregion

#pragma region Image
inline void KID::Vulkan::Image::SetDeviceMemory(DeviceMemory* deviceMemory_)
{
	DeviceMemoryUser::SetDeviceMemory(deviceMemory_);
	if(deviceMemory)
	{
		BindImageMemory(device->Vk_GetDevice(), vk_image, deviceMemory->Vk_GetDeviceMemory(), 0);
	}
}
#pragma endregion

#pragma region SwapchainImage
inline KID::string KID::Vulkan::SwapchainImage::GetInfo() const
{
	return SwapchainDependent::GetInfo() + "\n" + Image::GetInfo();
}
#pragma endregion

#pragma region RenderPass
inline void KID::Vulkan::RenderPass::Begin(CommandBuffer* commandBuffer, Framebuffer* framebuffer, const Rect& renderArea, const std::vector<VkClearValue>& clearValues, VkSubpassContents vk_subpassContents)
{
	std::vector<VkClearValue> vk_clearValues({
		KID::Vulkan::ClearColorf(1.0f, 0.0f, 0.0f, 1.0f),
		KID::Vulkan::ClearDepthStencil(1.0f, 0)
	});
	VkRenderPassBeginInfo vk_renderPassBeginInfo;
	{
		vk_renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		vk_renderPassBeginInfo.pNext = nullptr;
		vk_renderPassBeginInfo.renderPass = vk_renderPass;
		vk_renderPassBeginInfo.framebuffer = framebuffer->Vk_GetFramebuffer();
		vk_renderPassBeginInfo.renderArea = renderArea;
		vk_renderPassBeginInfo.clearValueCount = clearValues.size();
		vk_renderPassBeginInfo.pClearValues = clearValues.data();
	};
	vkCmdBeginRenderPass(commandBuffer->Vk_GetCommandBuffer(), &vk_renderPassBeginInfo, vk_subpassContents);
}
inline void KID::Vulkan::RenderPass::End(CommandBuffer* commandBuffer)
{
	vkCmdEndRenderPass(commandBuffer->Vk_GetCommandBuffer());
}
#pragma endregion

#pragma region CommandPool
inline void KID::Vulkan::CommandPool::Reset()
{
	KID::Vulkan::ResetCommandPool(device->Vk_GetDevice(), vk_commandPool, VkCommandPoolResetFlagBits::VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
}
#pragma endregion

#pragma region CommandBuffer
inline void KID::Vulkan::CommandBuffer::Reset()
{
	ResetCommandBuffer(
		vk_commandBuffer,
		VkCommandBufferResetFlagBits::VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT
	);
}
inline void KID::Vulkan::CommandBuffer::Begin(RenderPass* renderPass_, uint32_t subpass_, Framebuffer* framebuffer, bool occlusionQuery_, VkQueryControlFlags vk_queryControlFlags)
{
	VkCommandBufferInheritanceInfo vk_commandBufferInheritanceInfo;
	{
		vk_commandBufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
		vk_commandBufferInheritanceInfo.pNext = nullptr;
		vk_commandBufferInheritanceInfo.renderPass = renderPass_ ? renderPass_->Vk_GetRenderPass() : VK_NULL_HANDLE;
		vk_commandBufferInheritanceInfo.subpass = subpass_;
		vk_commandBufferInheritanceInfo.framebuffer = framebuffer ? framebuffer->Vk_GetFramebuffer() : VK_NULL_HANDLE;
		vk_commandBufferInheritanceInfo.occlusionQueryEnable = occlusionQuery_;
		vk_commandBufferInheritanceInfo.queryFlags = vk_queryControlFlags;
		vk_commandBufferInheritanceInfo.pipelineStatistics = 0;
	}
	VkCommandBufferBeginInfo vk_commandBufferBeginInfo;
	{
		vk_commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		vk_commandBufferBeginInfo.pNext = nullptr;
		vk_commandBufferBeginInfo.flags = 0;
		vk_commandBufferBeginInfo.pInheritanceInfo = &vk_commandBufferInheritanceInfo;
	}
	BeginCommandBuffer(vk_commandBuffer, &vk_commandBufferBeginInfo);
}
inline void KID::Vulkan::CommandBuffer::End()
{
	EndCommandBuffer(vk_commandBuffer);
}
#pragma endregion

#pragma region DescriptorSet
inline KID::string KID::Vulkan::DescriptorSet::GetInfo() const
{
	return DescriptorPoolDependent::GetInfo() + "\n" + DescriptorSetLayoutDependent::GetInfo();
}
#pragma endregion

#pragma region Pipeline
inline KID::string KID::Vulkan::Pipeline::GetInfo() const
{
	return DeviceDependent::GetInfo() + "\n" + RenderPassDependent::GetInfo();
}
#pragma endregion

#pragma region Buffer
inline void KID::Vulkan::Buffer::SetDeviceMemory(DeviceMemory* deviceMemory_)
{
	DeviceMemoryUser::SetDeviceMemory(deviceMemory_);
	BindBufferMemory(device->Vk_GetDevice(), vk_buffer, deviceMemory->Vk_GetDeviceMemory(), 0);
}
#pragma endregion


#pragma region
#pragma endregion

#pragma endregion
#pragma region Func
inline VkClearValue												KID::Vulkan::ClearColorf(float r, float g, float b, float a)
{
	VkClearValue vk_clearValue;
	{
		vk_clearValue.color.int32[0] = 0;
		vk_clearValue.color.int32[1] = 0;
		vk_clearValue.color.int32[2] = 0;
		vk_clearValue.color.int32[3] = 0;
		vk_clearValue.color.uint32[0] = 0;
		vk_clearValue.color.uint32[1] = 0;
		vk_clearValue.color.uint32[2] = 0;
		vk_clearValue.color.uint32[3] = 0;
		vk_clearValue.depthStencil.depth = 0.0f;
		vk_clearValue.depthStencil.stencil = 0;
		vk_clearValue.color.float32[0] = r;
		vk_clearValue.color.float32[1] = g;
		vk_clearValue.color.float32[2] = b;
		vk_clearValue.color.float32[3] = a;
	}
	return vk_clearValue;
}
inline VkClearValue												KID::Vulkan::ClearColori(int32_t r, int32_t g, int32_t b, int32_t a)
{
	VkClearValue vk_clearValue;
	{
		vk_clearValue.color.uint32[0] = 0;
		vk_clearValue.color.uint32[1] = 0;
		vk_clearValue.color.uint32[2] = 0;
		vk_clearValue.color.uint32[3] = 0;
		vk_clearValue.depthStencil.depth = 0.0f;
		vk_clearValue.depthStencil.stencil = 0;
		vk_clearValue.color.float32[0] = 0;
		vk_clearValue.color.float32[1] = 0;
		vk_clearValue.color.float32[2] = 0;
		vk_clearValue.color.float32[3] = 0;
		vk_clearValue.color.int32[0] = r;
		vk_clearValue.color.int32[1] = g;
		vk_clearValue.color.int32[2] = b;
		vk_clearValue.color.int32[3] = a;
	}
	return vk_clearValue;
}
inline VkClearValue												KID::Vulkan::ClearColorui(uint32_t r, uint32_t g, uint32_t b, uint32_t a)
{
	VkClearValue vk_clearValue;
	{
		vk_clearValue.color.int32[0] = 0;
		vk_clearValue.color.int32[1] = 0;
		vk_clearValue.color.int32[2] = 0;
		vk_clearValue.color.int32[3] = 0;
		vk_clearValue.depthStencil.depth = 0.0f;
		vk_clearValue.depthStencil.stencil = 0;
		vk_clearValue.color.float32[0] = 0;
		vk_clearValue.color.float32[1] = 0;
		vk_clearValue.color.float32[2] = 0;
		vk_clearValue.color.float32[3] = 0;
		vk_clearValue.color.uint32[0] = r;
		vk_clearValue.color.uint32[1] = g;
		vk_clearValue.color.uint32[2] = b;
		vk_clearValue.color.uint32[3] = a;
	}
	return vk_clearValue;
}
inline VkClearValue												KID::Vulkan::ClearDepthStencil(float depth, uint32_t stencil)
{
	VkClearValue vk_clearValue;
	{
		vk_clearValue.color.int32[0] = 0;
		vk_clearValue.color.int32[1] = 0;
		vk_clearValue.color.int32[2] = 0;
		vk_clearValue.color.int32[3] = 0;
		vk_clearValue.color.uint32[0] = 0;
		vk_clearValue.color.uint32[1] = 0;
		vk_clearValue.color.uint32[2] = 0;
		vk_clearValue.color.uint32[3] = 0;
		vk_clearValue.color.float32[0] = 0;
		vk_clearValue.color.float32[1] = 0;
		vk_clearValue.color.float32[2] = 0;
		vk_clearValue.color.float32[3] = 0;
		vk_clearValue.depthStencil.depth = depth;
		vk_clearValue.depthStencil.stencil = stencil;
	}
	return vk_clearValue;
}

inline std::vector<VkLayerProperties>							KID::Vulkan::EnumerateInstanceLayerProperties()
{
	uint32_t vk_instancelayersPropertiesCount;
	KID::Vulkan::Exceptions::Perform(vkEnumerateInstanceLayerProperties(&vk_instancelayersPropertiesCount, nullptr));

	std::vector<VkLayerProperties> vk_instancelayersProperties(vk_instancelayersPropertiesCount);
	KID::Vulkan::Exceptions::Perform(vkEnumerateInstanceLayerProperties(&vk_instancelayersPropertiesCount, vk_instancelayersProperties.data()));

	return vk_instancelayersProperties;
}
inline std::vector<VkLayerProperties>							KID::Vulkan::EnumerateDeviceLayerProperties(VkPhysicalDevice vk_physicalDevice)
{
	uint32_t vk_layersPropertiesCount;
	vkEnumerateDeviceLayerProperties(vk_physicalDevice, &vk_layersPropertiesCount, nullptr);

	std::vector<VkLayerProperties> vk_layersProperties(vk_layersPropertiesCount);
	vkEnumerateDeviceLayerProperties(vk_physicalDevice, &vk_layersPropertiesCount, vk_layersProperties.data());

	return std::move(vk_layersProperties);
}
inline std::vector<VkExtensionProperties>						KID::Vulkan::EnumerateInstanceExtensionProperties(const char* vk_layerName)
{
	uint32_t vk_instanceExtensionPropertiesCount;
	vkEnumerateInstanceExtensionProperties(vk_layerName, &vk_instanceExtensionPropertiesCount, nullptr);

	std::vector<VkExtensionProperties> vk_extensionProperties(vk_instanceExtensionPropertiesCount);
	vkEnumerateInstanceExtensionProperties(vk_layerName, &vk_instanceExtensionPropertiesCount, vk_extensionProperties.data());

	return std::move(vk_extensionProperties);
}
inline std::vector<VkExtensionProperties>						KID::Vulkan::EnumerateDeviceExtensionProperties(VkPhysicalDevice vk_physicalDevice, const char* vk_layerName)
{
	uint32_t vk_deviceExtensionPropertiesCount;
	vkEnumerateDeviceExtensionProperties(vk_physicalDevice, vk_layerName, &vk_deviceExtensionPropertiesCount, nullptr);

	std::vector<VkExtensionProperties> vk_extensionProperties(vk_deviceExtensionPropertiesCount);
	vkEnumerateDeviceExtensionProperties(vk_physicalDevice, vk_layerName, &vk_deviceExtensionPropertiesCount, vk_extensionProperties.data());

	return std::move(vk_extensionProperties);
}

inline VkInstance												KID::Vulkan::CreateInstance(VkInstanceCreateInfo* vk_instanceInfo, const VkAllocationCallbacks* vk_allocator)
{
	VkInstance vk_instance;

	auto vk_result = vkCreateInstance(vk_instanceInfo, vk_allocator, &vk_instance);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw new KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw new KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		case VK_ERROR_INITIALIZATION_FAILED:
			throw new KID::Vulkan::Exceptions::Exception_InitializationFailed();
		case VK_ERROR_LAYER_NOT_PRESENT:
			throw new KID::Vulkan::Exceptions::Exception_LayerNotPresent();
		case VK_ERROR_EXTENSION_NOT_PRESENT:
			throw new KID::Vulkan::Exceptions::Exception_ExtensionNotPresent();
		case VK_ERROR_INCOMPATIBLE_DRIVER:
			throw new KID::Vulkan::Exceptions::Exception_IncompatibleDriver();
		default:
			throw new KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_instance;
}
inline void														KID::Vulkan::DestroyInstance(VkInstance vk_instance, const VkAllocationCallbacks* vk_allocator)
{
	vkDestroyInstance(vk_instance, vk_allocator);
}

inline std::vector<VkPhysicalDevice>							KID::Vulkan::EnumeratePhysicalDevices(VkInstance vk_instance)
{
	uint32_t vk_physicalDevicesCount = 0;
	{
		auto vk_result = vkEnumeratePhysicalDevices(vk_instance, &vk_physicalDevicesCount, nullptr);
		switch(vk_result)
		{
			case VK_SUCCESS:
				break;
			case VK_ERROR_OUT_OF_HOST_MEMORY:
				throw new KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
			case VK_ERROR_OUT_OF_DEVICE_MEMORY:
				throw new KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
			case VK_ERROR_INITIALIZATION_FAILED:
				throw new KID::Vulkan::Exceptions::Exception_InitializationFailed();
			default:
				throw new KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
		}
	}

	std::vector<VkPhysicalDevice> vk_physicalDevices(vk_physicalDevicesCount);

	auto vk_result = vkEnumeratePhysicalDevices(vk_instance, &vk_physicalDevicesCount, vk_physicalDevices.data());
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw new KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw new KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		case VK_ERROR_INITIALIZATION_FAILED:
			throw new KID::Vulkan::Exceptions::Exception_InitializationFailed();
		default:
			throw new KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return std::move(vk_physicalDevices);
}

inline VkPhysicalDeviceProperties								KID::Vulkan::GetPhysicalDeviceProperties(VkPhysicalDevice vk_physicalDevice)
{
	VkPhysicalDeviceProperties vk_physicalDeviceProperties;

	vkGetPhysicalDeviceProperties(vk_physicalDevice, &vk_physicalDeviceProperties);

	return vk_physicalDeviceProperties;
}
inline std::vector<VkQueueFamilyProperties>						KID::Vulkan::GetPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice vk_physicalDevice)
{
	uint32_t vk_queueFamilyPropertiesCount;
	vkGetPhysicalDeviceQueueFamilyProperties(vk_physicalDevice, &vk_queueFamilyPropertiesCount, nullptr);

	std::vector<VkQueueFamilyProperties> vk_queueFamilyProperties(vk_queueFamilyPropertiesCount);
	vkGetPhysicalDeviceQueueFamilyProperties(vk_physicalDevice, &vk_queueFamilyPropertiesCount, vk_queueFamilyProperties.data());

	return std::move(vk_queueFamilyProperties);
}
inline VkPhysicalDeviceMemoryProperties							KID::Vulkan::GetPhysicalDeviceMemoryProperties(VkPhysicalDevice vk_physicalDevice)
{
	VkPhysicalDeviceMemoryProperties vk_physicalDeviceMemoryProperties;
	
	vkGetPhysicalDeviceMemoryProperties(vk_physicalDevice, &vk_physicalDeviceMemoryProperties);

	return vk_physicalDeviceMemoryProperties;
}

inline VkSurfaceKHR												KID::Vulkan::CreateWin32SurfaceKHR(VkInstance vk_instance, HINSTANCE hInstance, HWND hWnd, VkAllocationCallbacks* vk_allocationCallbacks)
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

	KID::Vulkan::Exceptions::Perform(vkCreateWin32SurfaceKHR(vk_instance, &vk_win32SurfaceCreateInfoKHR, vk_allocationCallbacks, &vk_surfaceKHR));

	return vk_surfaceKHR;
}
inline void														KID::Vulkan::DestroySurfaceKHR(VkInstance vk_instance, VkSurfaceKHR vk_surface, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroySurfaceKHR(vk_instance, vk_surface, vk_allocationCallbacks);
}
inline bool														KID::Vulkan::GetPhysicalDeviceSurfaceSupportKHR(VkPhysicalDevice vk_physicalDevice, uint32_t vk_queueFamilyIndex, VkSurfaceKHR vk_surface)
{
	VkBool32 vk_surfaceSupport;

	KID::Vulkan::Exceptions::Perform(vkGetPhysicalDeviceSurfaceSupportKHR(vk_physicalDevice, 0, vk_surface, &vk_surfaceSupport));

	return vk_surfaceSupport == VK_TRUE;
}

inline std::vector<VkSurfaceFormatKHR>							KID::Vulkan::GetPhysicalDeviceSurfaceFormatsKHR(VkPhysicalDevice vk_physicalDevice, VkSurfaceKHR vk_surface)
{
	uint32_t vk_surfaceFormatsCount;
	KID::Vulkan::Exceptions::Perform(vkGetPhysicalDeviceSurfaceFormatsKHR(vk_physicalDevice, vk_surface, &vk_surfaceFormatsCount, nullptr));

	std::vector<VkSurfaceFormatKHR> vk_surfaceFormats(vk_surfaceFormatsCount);
	KID::Vulkan::Exceptions::Perform(vkGetPhysicalDeviceSurfaceFormatsKHR(vk_physicalDevice, vk_surface, &vk_surfaceFormatsCount, vk_surfaceFormats.data()));

	return std::move(vk_surfaceFormats);
}
inline VkSurfaceCapabilitiesKHR									KID::Vulkan::GetPhysicalDeviceSurfaceCapabilitiesKHR(VkPhysicalDevice vk_physicalDevice, VkSurfaceKHR vk_surface)
{
	VkSurfaceCapabilitiesKHR vk_surfaceCapabilities;

	KID::Vulkan::Exceptions::Perform(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vk_physicalDevice, vk_surface, &vk_surfaceCapabilities));

	return vk_surfaceCapabilities;
}
inline std::vector<VkPresentModeKHR>							KID::Vulkan::GetPhysicalDeviceSurfacePresentModesKHR(VkPhysicalDevice vk_physicalDevice, VkSurfaceKHR vk_surface)
{
	uint32_t vk_physicalDeviceSurfacePresentModesCount;
	KID::Vulkan::Exceptions::Perform(vkGetPhysicalDeviceSurfacePresentModesKHR(vk_physicalDevice, vk_surface, &vk_physicalDeviceSurfacePresentModesCount, nullptr));

	std::vector<VkPresentModeKHR> vk_physicalDeviceSurfacePresentModes(vk_physicalDeviceSurfacePresentModesCount);
	KID::Vulkan::Exceptions::Perform(vkGetPhysicalDeviceSurfacePresentModesKHR(vk_physicalDevice, vk_surface, &vk_physicalDeviceSurfacePresentModesCount, vk_physicalDeviceSurfacePresentModes.data()));

	return std::move(vk_physicalDeviceSurfacePresentModes);
}

inline VkDevice													KID::Vulkan::CreateDevice(VkPhysicalDevice vk_physicalDevice, VkDeviceCreateInfo* vk_deviceCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkDevice vk_device;

	auto vk_result = vkCreateDevice(vk_physicalDevice, vk_deviceCreateInfo, vk_allocationCallbacks, &vk_device);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw new KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw new KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		case VK_ERROR_INITIALIZATION_FAILED:
			throw new KID::Vulkan::Exceptions::Exception_InitializationFailed();
		case VK_ERROR_DEVICE_LOST:
			throw new KID::Vulkan::Exceptions::Exception_DeviceLost();
		case VK_ERROR_LAYER_NOT_PRESENT:
			throw new KID::Vulkan::Exceptions::Exception_LayerNotPresent();
		case VK_ERROR_EXTENSION_NOT_PRESENT:
			throw new KID::Vulkan::Exceptions::Exception_ExtensionNotPresent();
		case VK_ERROR_FEATURE_NOT_PRESENT:
			throw new KID::Vulkan::Exceptions::Exception_FeatureNotPresent();
		case VK_ERROR_TOO_MANY_OBJECTS:
			throw new KID::Vulkan::Exceptions::Exception_TooManyObjects();
		default:
			throw new KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_device;
}
inline void														KID::Vulkan::DestroyDevice(VkDevice vk_device, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyDevice(vk_device, vk_allocationCallbacks);
}
inline void														KID::Vulkan::DeviceWaitIdle(VkDevice vk_device)
{
	auto vk_result = vkDeviceWaitIdle(vk_device);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		case VK_ERROR_DEVICE_LOST:
			throw KID::Vulkan::Exceptions::Exception_DeviceLost();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}
}

inline VkDeviceMemory											KID::Vulkan::AllocateMemory(VkDevice vk_device, VkMemoryAllocateInfo* vk_memoryAllocateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkDeviceMemory vk_deviceMemory;

	KID::Vulkan::Exceptions::Perform(vkAllocateMemory(vk_device, vk_memoryAllocateInfo, vk_allocationCallbacks, &vk_deviceMemory));

	return vk_deviceMemory;
}
inline void														KID::Vulkan::FreeMemory(VkDevice vk_device, VkDeviceMemory vk_deviceMemory, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkFreeMemory(vk_device, vk_deviceMemory, vk_allocationCallbacks);
}
inline void														KID::Vulkan::BindImageMemory(VkDevice vk_device, VkImage vk_image, VkDeviceMemory vk_deviceMemory, VkDeviceSize vk_deviceSize)
{
	KID::Vulkan::Exceptions::Perform(vkBindImageMemory(vk_device, vk_image, vk_deviceMemory, vk_deviceSize));
}
inline void														KID::Vulkan::BindBufferMemory(VkDevice vk_device, VkBuffer vk_buffer, VkDeviceMemory vk_deviceMemory, VkDeviceSize vk_memoryOffset)
{
	KID::Vulkan::Exceptions::Perform(vkBindBufferMemory(vk_device, vk_buffer, vk_deviceMemory, vk_memoryOffset));
}
inline void*													KID::Vulkan::MapMemory(VkDevice vk_device, VkDeviceMemory vk_deviceMemory, VkDeviceSize offset, VkDeviceSize size, VkMemoryMapFlags vk_memoryMapFlags)
{
	void* data;

	auto vk_result = vkMapMemory(vk_device, vk_deviceMemory, offset, size, vk_memoryMapFlags, &data);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		case VK_ERROR_MEMORY_MAP_FAILED:
			throw KID::Vulkan::Exceptions::Exception_MemoryMapFailed();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return data;
}
inline void														KID::Vulkan::UnmapMemory(VkDevice vk_device, VkDeviceMemory vk_deviceMemory)
{
	vkUnmapMemory(vk_device, vk_deviceMemory);
}

inline VkSwapchainKHR											KID::Vulkan::CreateSwapchainKHR(VkDevice vk_device, VkSwapchainCreateInfoKHR* vk_SwapchainCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkSwapchainKHR vk_swapchain;

	KID::Vulkan::Exceptions::Perform(vkCreateSwapchainKHR(vk_device, vk_SwapchainCreateInfo, vk_allocationCallbacks, &vk_swapchain));

	return vk_swapchain;
}
inline std::vector<VkImage>										KID::Vulkan::GetSwapchainImagesKHR(VkDevice vk_device, VkSwapchainKHR vk_swapchain)
{
	uint32_t vk_imagesCount;
	KID::Vulkan::Exceptions::Perform(vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &vk_imagesCount, nullptr));

	std::vector<VkImage> vk_images(vk_imagesCount);
	KID::Vulkan::Exceptions::Perform(vkGetSwapchainImagesKHR(vk_device, vk_swapchain, &vk_imagesCount, vk_images.data()));

	return std::move(vk_images);
}
inline uint32_t													KID::Vulkan::AcquireNextImageKHR(VkDevice vk_device, VkSwapchainKHR vk_swapchain, uint64_t timeout, VkSemaphore vk_semaphore, VkFence vk_fence)
{
	uint32_t vk_currentImage;

	Exceptions::Perform(vkAcquireNextImageKHR(vk_device, vk_swapchain, UINT64_MAX, 0, (VkFence)0, &vk_currentImage));

	return vk_currentImage;
}
inline void														KID::Vulkan::DestroySwapchainKHR(VkDevice vk_device, VkSwapchainKHR vk_swapchain, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroySwapchainKHR(vk_device, vk_swapchain, vk_allocationCallbacks);
}

inline VkQueue													KID::Vulkan::GetDeviceQueue(VkDevice vk_device, uint32_t vk_queueFamilyIndex, uint32_t vk_queueIndex)
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
inline void														KID::Vulkan::QueueSubmit(VkQueue vk_queue, std::vector<VkSubmitInfo>& vk_submitInfos, VkFence vk_fence)
{
	KID::Vulkan::Exceptions::Perform(vkQueueSubmit(vk_queue, vk_submitInfos.size(), vk_submitInfos.data(), vk_fence));
}
inline void														KID::Vulkan::QueueWaitIdle(VkQueue vk_queue)
{
	auto vk_result = vkQueueWaitIdle(vk_queue);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		case VK_ERROR_DEVICE_LOST:
			throw KID::Vulkan::Exceptions::Exception_DeviceLost();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}
}
inline void														KID::Vulkan::QueuePresentKHR(VkQueue vk_queue, VkPresentInfoKHR* vk_presentInfoKHR)
{
	Exceptions::Perform(vkQueuePresentKHR(vk_queue, vk_presentInfoKHR));
}

inline VkCommandPool											KID::Vulkan::CreateCommandPool(VkDevice vk_device, VkCommandPoolCreateFlags vk_commandPoolCreateFlags, uint32 vk_commandPoolQueueFamilyIndex, VkAllocationCallbacks* vk_allocationCallbacks)
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
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_commandPool;
}
inline void														KID::Vulkan::DestroyCommandPool(VkDevice vk_device, VkCommandPool vk_commandPool, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyCommandPool(vk_device, vk_commandPool, vk_allocationCallbacks);
}
inline void														KID::Vulkan::ResetCommandPool(VkDevice vk_device, VkCommandPool vk_commandPool, VkCommandPoolResetFlags vk_commandPoolResetFlags)
{
	auto vk_result = vkResetCommandPool(vk_device, vk_commandPool, vk_commandPoolResetFlags);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}
}

inline std::vector<VkCommandBuffer>								KID::Vulkan::AllocateCommandBuffers(VkDevice vk_device, VkCommandPool vk_commandPool, uint32_t vk_commandBuffersCount, VkCommandBufferLevel vk_commandBufferLevel)
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
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return std::move(vk_commandBuffers);
}
inline void														KID::Vulkan::FreeCommandBuffers(VkDevice vk_device, VkCommandPool vk_commandPool, std::vector<VkCommandBuffer>& vk_commandBuffers)
{
	vkFreeCommandBuffers(vk_device, vk_commandPool, vk_commandBuffers.size(), vk_commandBuffers.data());
	vk_commandBuffers.clear();
}
inline void														KID::Vulkan::ResetCommandBuffer(VkCommandBuffer vk_commandBuffer, VkCommandBufferResetFlags vk_commandBufferResetFlags)
{
	auto vk_result = vkResetCommandBuffer(vk_commandBuffer, vk_commandBufferResetFlags);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}
}
inline void														KID::Vulkan::BeginCommandBuffer(VkCommandBuffer vk_commandBuffer, VkCommandBufferBeginInfo *vk_commandBufferBeginInfo)
{
	auto vk_result = vkBeginCommandBuffer(vk_commandBuffer, vk_commandBufferBeginInfo);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}
}
inline void														KID::Vulkan::EndCommandBuffer(VkCommandBuffer vk_commandBuffer)
{
	auto vk_result = vkEndCommandBuffer(vk_commandBuffer);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}
}

inline VkBuffer													KID::Vulkan::CreateBuffer(VkDevice vk_device, VkBufferCreateInfo* vk_bufferCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkBuffer vk_buffer;

	auto vk_result = vkCreateBuffer(vk_device, vk_bufferCreateInfo, vk_allocationCallbacks, &vk_buffer);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_buffer;
}
inline void														KID::Vulkan::DestroyBuffer(VkDevice vk_device, VkBuffer vk_buffer, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyBuffer(vk_device, vk_buffer, vk_allocationCallbacks);
}
inline VkMemoryRequirements										KID::Vulkan::GetBufferMemoryRequirements(VkDevice vk_device, VkBuffer vk_buffer)
{
	VkMemoryRequirements vk_memoryRequirements;

	vkGetBufferMemoryRequirements(vk_device, vk_buffer, &vk_memoryRequirements);

	return vk_memoryRequirements;
}

inline void														KID::Vulkan::CmdExecuteCommands(VkCommandBuffer vk_commandBuffer, std::vector<VkCommandBuffer>& vk_commandBuffers)
{
	vkCmdExecuteCommands(vk_commandBuffer, vk_commandBuffers.size(), vk_commandBuffers.data());
}

VkRenderPass													KID::Vulkan::CreateRenderPass(VkDevice vk_device, VkRenderPassCreateInfo* vk_renderPassCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkRenderPass vk_renderPass;

	auto vk_result = vkCreateRenderPass(vk_device, vk_renderPassCreateInfo, vk_allocationCallbacks, &vk_renderPass);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_renderPass;
}
inline void														KID::Vulkan::DestroyRenderPass(VkDevice vk_device, VkRenderPass vk_renderPass, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyRenderPass(vk_device, vk_renderPass, vk_allocationCallbacks);
}

inline VkFramebuffer											KID::Vulkan::CreateFramebuffer(VkDevice vk_device, VkFramebufferCreateInfo* vk_framebufferCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkFramebuffer vk_framebuffer;

	auto vk_result = vkCreateFramebuffer(vk_device, vk_framebufferCreateInfo, vk_allocationCallbacks, &vk_framebuffer);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_framebuffer;
}
inline void														KID::Vulkan::DestroyFramebuffer(VkDevice vk_device, VkFramebuffer vk_framebuffer, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyFramebuffer(vk_device, vk_framebuffer, vk_allocationCallbacks);
}

inline VkImage													KID::Vulkan::CreateImage(VkDevice vk_device, VkImageCreateInfo* vk_imageCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkImage vk_image;

	auto vk_result = vkCreateImage(vk_device, vk_imageCreateInfo, vk_allocationCallbacks, &vk_image);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_image;
}
inline VkMemoryRequirements										KID::Vulkan::GetImageMemoryRequirements(VkDevice vk_device, VkImage vk_image)
{
	VkMemoryRequirements vk_memoryRequirements;

	vkGetImageMemoryRequirements(vk_device, vk_image, &vk_memoryRequirements);

	return vk_memoryRequirements;
}
inline void														KID::Vulkan::DestroyImage(VkDevice vk_device, VkImage vk_image, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyImage(vk_device, vk_image, vk_allocationCallbacks);
}

inline VkImageView												KID::Vulkan::CreateImageView(VkDevice vk_device, VkImageViewCreateInfo* vk_imageViewCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkImageView vk_imageView;

	auto vk_result = vkCreateImageView(vk_device, vk_imageViewCreateInfo, vk_allocationCallbacks, &vk_imageView);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_imageView;
}
inline void														KID::Vulkan::DestroyImageView(VkDevice vk_device, VkImageView vk_imageView, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyImageView(vk_device, vk_imageView, vk_allocationCallbacks);
}

inline VkSampler												KID::Vulkan::CreateSampler(VkDevice vk_device, VkSamplerCreateInfo* vk_samplerCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkSampler vk_sampler;

	auto vk_result = vkCreateSampler(vk_device, vk_samplerCreateInfo, vk_allocationCallbacks, &vk_sampler);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		case VK_ERROR_TOO_MANY_OBJECTS:
			throw KID::Vulkan::Exceptions::Exception_TooManyObjects();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_sampler;
}
inline void														KID::Vulkan::DestroySampler(VkDevice vk_device, VkSampler vk_sampler, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroySampler(vk_device, vk_sampler, vk_allocationCallbacks);
}

inline VkShaderModule											KID::Vulkan::CreateShaderModule(VkDevice vk_device, VkShaderModuleCreateInfo* vk_shaderModuleCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkShaderModule vk_ShaderModule;

	auto vk_result = vkCreateShaderModule(vk_device, vk_shaderModuleCreateInfo, vk_allocationCallbacks, &vk_ShaderModule);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_ShaderModule;
}
inline void														KID::Vulkan::DestroyShaderModule(VkDevice vk_device, VkShaderModule vk_shaderModule, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyShaderModule(vk_device, vk_shaderModule, vk_allocationCallbacks);
}

inline VkPipelineCache											KID::Vulkan::CreatePipelineCache(VkDevice vk_device, VkPipelineCacheCreateInfo* vk_pipelineCacheCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkPipelineCache vk_pipelineCache;

	auto vk_result = vkCreatePipelineCache(vk_device, vk_pipelineCacheCreateInfo, vk_allocationCallbacks, &vk_pipelineCache);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_pipelineCache;
}
inline void														KID::Vulkan::DestroyPipelineCache(VkDevice vk_device, VkPipelineCache vk_pipelineCache, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyPipelineCache(vk_device, vk_pipelineCache, vk_allocationCallbacks);
}

inline VkDescriptorSetLayout									KID::Vulkan::CreateDescriptorSetLayout(VkDevice vk_device, VkDescriptorSetLayoutCreateInfo* vk_descriptorSetLayoutCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkDescriptorSetLayout vk_descriptorSetLayout;

	auto vk_result = vkCreateDescriptorSetLayout(vk_device, vk_descriptorSetLayoutCreateInfo, vk_allocationCallbacks, &vk_descriptorSetLayout);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_descriptorSetLayout;
}
inline void														KID::Vulkan::DestroyDescriptorSetLayout(VkDevice vk_device, VkDescriptorSetLayout vk_descriptorSetLayout, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyDescriptorSetLayout(vk_device, vk_descriptorSetLayout, vk_allocationCallbacks);
}

inline VkPipelineLayout											KID::Vulkan::CreatePipelineLayout(VkDevice vk_device, VkPipelineLayoutCreateInfo* vk_pipelineLayoutCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkPipelineLayout vk_pipelineLayout;

	auto vk_result = vkCreatePipelineLayout(vk_device, vk_pipelineLayoutCreateInfo, vk_allocationCallbacks, &vk_pipelineLayout);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_pipelineLayout;
}
inline void														KID::Vulkan::DestroyPipelineLayout(VkDevice vk_device, VkPipelineLayout vk_pipelineLayout, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyPipelineLayout(vk_device, vk_pipelineLayout, vk_allocationCallbacks);
}

inline std::vector<VkPipeline>									KID::Vulkan::CreateGraphicsPipelines(VkDevice vk_device, VkPipelineCache& vk_pipelineCache, std::vector<VkGraphicsPipelineCreateInfo>& vk_graphicsPipelineCreateInfos, VkAllocationCallbacks* vk_allocationCallbacks)
{
	std::vector<VkPipeline> vk_pipelines(vk_graphicsPipelineCreateInfos.size());

	auto vk_result = vkCreateGraphicsPipelines(vk_device, vk_pipelineCache, vk_graphicsPipelineCreateInfos.size(), vk_graphicsPipelineCreateInfos.data(), vk_allocationCallbacks, vk_pipelines.data());
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return std::move(vk_pipelines);
}
inline void														KID::Vulkan::DestroyPipeline(VkDevice vk_device, VkPipeline vk_pipeline, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyPipeline(vk_device, vk_pipeline, vk_allocationCallbacks);
}

inline VkDescriptorPool											KID::Vulkan::CreateDescriptorPool(VkDevice vk_device, VkDescriptorPoolCreateInfo* vk_descriptorPoolCreateInfo, VkAllocationCallbacks* vk_allocationCallbacks)
{
	VkDescriptorPool vk_descriptorPool= VK_NULL_HANDLE;

	auto vk_result = vkCreateDescriptorPool(vk_device, vk_descriptorPoolCreateInfo, vk_allocationCallbacks, &vk_descriptorPool);
	switch(vk_result)
	{
		case VK_SUCCESS:
			break;
		case VK_ERROR_OUT_OF_HOST_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return vk_descriptorPool;
}
inline void														KID::Vulkan::DestroyDescriptorPool(VkDevice vk_device, VkDescriptorPool vk_descriptorPool, VkAllocationCallbacks* vk_allocationCallbacks)
{
	vkDestroyDescriptorPool(vk_device, vk_descriptorPool, vk_allocationCallbacks);
}

inline std::vector<VkDescriptorSet>								KID::Vulkan::AllocateDescriptorSets(VkDevice vk_device, VkDescriptorPool vk_descriptorPool, uint32_t count, const VkDescriptorSetLayout* vk_descriptorSetLayout)
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
			throw KID::Vulkan::Exceptions::Exception_OutOfHostMemory();
		case VK_ERROR_OUT_OF_DEVICE_MEMORY:
			throw KID::Vulkan::Exceptions::Exception_OutOfDeviceMemory();
		default:
			throw KID::Vulkan::Exceptions::Exception_UnknownException(vk_result);
	}

	return std::move(vk_descriptorSets);
}
inline void														KID::Vulkan::FreeDescriptorSets(VkDevice vk_device, VkDescriptorPool vk_descriptorPool, const std::vector<VkDescriptorSet>& vk_descriptorSets)
{
	vkFreeDescriptorSets(vk_device, vk_descriptorPool, vk_descriptorSets.size(), vk_descriptorSets.data());
}

inline uint32_t													KID::Vulkan::GetCorrectMemoryType(const VkPhysicalDeviceMemoryProperties& vk_physicalDeviceMemoryProperties, uint32_t vk_memoryTypeBits, VkFlags flags)
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
inline void														KID::Vulkan::Log::Clear()
{
#if __KID_VULKAN_LOG__
	writeMutex.lock();

	FILE* file = nullptr;

	auto error = fopen_s(&file, logFilename.c_str(), "w");
	if(error != 0)
	{
		// throw Exception();
	}

#if __KID_VULKAN_DEBUG__
	//if(!file) throw Exception("Can't write log.");
#endif

	string text = "[KID][Log] Cleared";

	fwrite(text.c_str(), sizeof(char), text.size(), file);
	fwrite("\n", sizeof(char), 1, file);

	fclose(file);

	timestampClock = std::clock();

	writeMutex.unlock();
#endif
}
inline void														KID::Vulkan::Log::Write(const string& text)
{
#if __KID_VULKAN_LOG__
	writeMutex.lock();

	FILE* file = nullptr;

	auto error = fopen_s(&file, logFilename.c_str(), "a");
	if(error != 0)
	{
		throw Exception();
	}

#if __KID_VULKAN_DEBUG__
	if(!file) throw Exception("Can't write log.");
#endif

	auto tabifyText = [](const string& source)
	{
		string text = source;

		//for(size_t i = 0; i < tab; ++i) text.insert(text.begin(), '\t');
		//
		//auto t = text.begin();
		//while((t = std::find(t, text.end(), '\n')) != text.end())
		//{
		//	++t;
		//	for(size_t i = 0; i < tab; ++i) text.insert(t, '\t');
		//}

		for(size_t i = 0; i < tab; ++i) text.insert(0, 1, '\t');

		auto t = 0;
		while((t = text.find('\n', t)) != UINT_MAX)
		{
			++t;
			for(size_t i = 0; i < tab; ++i) text.insert(t, 1, '\t');
		}

		return text;
	}(text);

	fwrite(tabifyText.c_str(), sizeof(char), tabifyText.size(), file);
	fwrite("\n", sizeof(char), 1, file);
	fclose(file);

	writeMutex.unlock();
#endif
}
inline void														KID::Vulkan::Log::Timestamp()
{
#if __KID_VULKAN_LOG__
	writeMutex.lock();

	FILE* file = nullptr;

	auto error = fopen_s(&file, logFilename.c_str(), "a");
	if(error != 0)
	{
		// throw Exception();
	}

#if __KID_VULKAN_DEBUG__
	//if(!file) throw Exception("Can't write log.");
#endif

	auto timestamp = std::to_string(std::clock());// -timestampClock);
	while(timestamp.size() < 16) timestamp.insert(timestamp.begin(), '0');

	fwrite(timestamp.c_str(), sizeof(char), timestamp.size(), file);
	fwrite(":\n", sizeof(char), 2, file);

	fclose(file);

	timestampClock = std::clock();

	writeMutex.unlock();
#endif
}
inline void														KID::Vulkan::Log::Tab()
{
	writeMutex.lock();
	++tab;
	writeMutex.unlock();
}
inline void														KID::Vulkan::Log::Untab()
{
	writeMutex.lock();
	--tab;
	writeMutex.unlock();
}
#pragma endregion
#pragma endregion

#pragma region
#pragma endregion
