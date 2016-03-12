#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "../Vulkan/Vulkan.hpp"
#pragma comment(lib, "Vulkan.lib")

//#define BVE_PATH(path) std::wstring(L"") + path
#define BVE_PATH(path) std::wstring(L"../../../") + path

#include <fstream>

std::vector<uint32_t> loadFile(const std::wstring& filename)
{
	//{
	//	auto l = GetCurrentDirectoryW(0, 0);
	//
	//	std::wstring dir; dir.resize(l + 0);
	//	GetCurrentDirectoryW(dir.size(), (LPWSTR)dir.data());
	//
	//	dir.at(0);
	//}
	//
	//_wchdir(L"F:\\Projects\\Projects\\Vulkan\\");
		
	FILE* file = nullptr;
	if(_wfopen_s(&file, filename.c_str(), L"rb") != 0)
	{
		throw std::exception("");
	}
	fseek(file, 0, FILE_END);
	auto size = ftell(file);
	if(size % 4 != 0) throw std::exception("");

	rewind(file);

	std::vector<uint32_t> result(size);// / 4); // for now
	fread((void*)result.data(), 1, size, file);

	fclose(file);

	return std::move(result);
}

VkBool32 __stdcall vk_debugCallback(
	VkDebugReportFlagsEXT                       flags,
	VkDebugReportObjectTypeEXT                  objectType,
	uint64_t                                    object,
	size_t                                      location,
	int32_t                                     messageCode,
	const char*                                 pLayerPrefix,
	const char*                                 pMessage,
	void*                                       pUserData)
{
	//MessageBoxA(0, pMessage, pLayerPrefix, MB_OK);
	BVE::Vulkan::Log::Write("[Vulkan][" + BVE::string(pLayerPrefix) + "]" + BVE::string(pMessage));
	return false;
}

void vulkan()
{
	system("mode con: cols=80 lines=1000");
	int width = 512;
	int height = 512;
	auto hInstance = GetModuleHandle(NULL);
	auto hWnd = [&](){
		WNDCLASSEX wndClass;
		{
			wndClass.cbSize = sizeof(wndClass);
			wndClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wndClass.lpfnWndProc = [](HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) -> LRESULT { return DefWindowProc(hWnd, msg, wParam, lParam); };
			wndClass.cbClsExtra = 0;
			wndClass.cbWndExtra = 0;
			wndClass.hInstance = hInstance;
			wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	//NULL_BRUSH;//GetStockObject(NULL_BRUSH);//WHITE_BRUSH);
			wndClass.lpszMenuName = NULL;
			wndClass.lpszClassName = L"my window class";
			wndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		}

		if(!RegisterClassEx(&wndClass)) throw std::exception("");

		auto wndStyle = WS_OVERLAPPEDWINDOW;
		auto wndStyleEx = WS_EX_OVERLAPPEDWINDOW;

		RECT wndRect;
		{
			wndRect.left = 100;
			wndRect.top = 100;
			wndRect.right = wndRect.top + width;
			wndRect.bottom = wndRect.left + height;
			AdjustWindowRectEx(&wndRect, wndStyle, NULL, wndStyleEx);
		}

		auto hWnd = CreateWindowEx(
			wndStyleEx,
			L"my window class",
			L"window",
			wndStyle,
			wndRect.left,
			wndRect.top,
			wndRect.right - wndRect.left,
			wndRect.bottom - wndRect.top,
			(HWND)NULL,
			(HMENU)NULL,
			hInstance,
			NULL
			);

		if(!hWnd) throw std::exception("");

		ShowWindow(hWnd, SW_SHOW);

		return hWnd;
	}();
	BVE::Vulkan::Log::Write("[WinAPI] Window created");

	auto vk_instance = [&]()
	{
		BVE::Vulkan::InstanceLayersProperties instanceLayersProperties;
		BVE::Vulkan::InstanceExtensionsProperties instanceExtensionsProperties(instanceLayersProperties);

		//auto vk_layers = instanceLayersProperties.layersName;
		std::vector<const char*> vk_layers = {
			"VK_LAYER_GOOGLE_unique_objects",		//wrap all Vulkan objects in a unique pointer at create time and unwrap them at use time
			"VK_LAYER_LUNARG_device_limits",		//validate that app properly queries features and obeys feature limitations
			"VK_LAYER_LUNARG_draw_state",			//validate the descriptor set, pipeline state, and dynamic state; validate the interfaces between SPIR - V modules and the graphics pipeline
			"VK_LAYER_LUNARG_image",				//validate texture formats and render target formats
			"VK_LAYER_LUNARG_mem_tracker",			//track and validate GPU memory and its binding to objects and command buffers
			"VK_LAYER_LUNARG_object_tracker",		//track all Vulkan objects and flag invalid objects and object memory leaks
			"VK_LAYER_LUNARG_param_checker",		//validate API parameter values
			"VK_LAYER_LUNARG_swapchain",			//validate the use of the WSI "swapchain" extensions
			"VK_LAYER_LUNARG_threading",			//check validity of multi - threaded API usage			
			"VK_LAYER_LUNARG_standard_validation",	// include all above			
			"VK_LAYER_LUNARG_api_dump",				//print API calls and their parameters and values
		};

		//auto vk_extensions = instanceExtensionsProperties.extensionsName;
		std::vector<const char*> vk_extensions = {
			VK_KHR_SURFACE_EXTENSION_NAME,				// supported
			//VK_KHR_SWAPCHAIN_EXTENSION_NAME,			// not supported!
			//VK_KHR_DISPLAY_EXTENSION_NAME,			// not supported!
			//VK_KHR_DISPLAY_SWAPCHAIN_EXTENSION_NAME,	// not supported!
			VK_KHR_WIN32_SURFACE_EXTENSION_NAME,		// supported
			VK_EXT_DEBUG_REPORT_EXTENSION_NAME,			// supported
		};

		VkApplicationInfo vk_applicationInfo;
		{
			vk_applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			vk_applicationInfo.pNext = nullptr;
			vk_applicationInfo.pApplicationName = "amd suck large cocks";
			vk_applicationInfo.applicationVersion = 0;
			vk_applicationInfo.pEngineName = "fuck amd mom";
			vk_applicationInfo.engineVersion = 0;
			vk_applicationInfo.apiVersion = VK_API_VERSION;
		}
		VkInstanceCreateInfo vk_instanceInfo;
		{
			vk_instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			vk_instanceInfo.pNext = nullptr;
			vk_instanceInfo.flags = 0;
			vk_instanceInfo.pApplicationInfo = &vk_applicationInfo;
			vk_instanceInfo.enabledLayerCount = vk_layers.size();
			vk_instanceInfo.ppEnabledLayerNames = vk_layers.data();
			vk_instanceInfo.enabledExtensionCount = vk_extensions.size();
			vk_instanceInfo.ppEnabledExtensionNames = vk_extensions.data();
		}

		auto vk_instance = BVE::Vulkan::CreateInstance(&vk_instanceInfo, nullptr);

		return vk_instance;
	}();
		
	PFN_vkCreateDebugReportCallbackEXT fnCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(vk_instance, "vkCreateDebugReportCallbackEXT");

	VkDebugReportCallbackEXT vk_debugReportCallbackEXT;
	VkDebugReportCallbackCreateInfoEXT vk_debugReportCallbackCreateInfoEXT;
	{
		vk_debugReportCallbackCreateInfoEXT.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		vk_debugReportCallbackCreateInfoEXT.pNext = nullptr;
		vk_debugReportCallbackCreateInfoEXT.flags =	VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT;			
		//vk_debugReportCallbackCreateInfoEXT.flags = VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT;
		vk_debugReportCallbackCreateInfoEXT.pfnCallback = vk_debugCallback;
		vk_debugReportCallbackCreateInfoEXT.pUserData = nullptr;
	}
	BVE::Vulkan::Exceptions::Perform(fnCreateDebugReportCallbackEXT(vk_instance, &vk_debugReportCallbackCreateInfoEXT, nullptr, &vk_debugReportCallbackEXT));

	auto vk_physicalDevice = BVE::Vulkan::EnumeratePhysicalDevices(vk_instance)[0];
	auto vk_physicalDeviceProperties = BVE::Vulkan::GetPhysicalDeviceProperties(vk_physicalDevice);
	auto vk_physicalDeviceMemoryProperties = BVE::Vulkan::GetPhysicalDeviceMemoryProperties(vk_physicalDevice);
	auto vk_physicalDeviceQueueFamilyProperties = BVE::Vulkan::GetPhysicalDeviceQueueFamilyProperties(vk_physicalDevice);

	auto deviceLayerProperties = BVE::Vulkan::DeviceLayersProperties(vk_physicalDevice);
	auto deviceExtensionsProperties = BVE::Vulkan::DeviceExtensionsProperties(vk_physicalDevice, deviceLayerProperties);
	auto vk_device = [&]()
	{
		//auto vk_layers = deviceLayerProperties.layersName;
		std::vector<const char*> vk_layers = {
			"VK_LAYER_GOOGLE_unique_objects", //wrap all Vulkan objects in a unique pointer at create time and unwrap them at use time
			"VK_LAYER_LUNARG_device_limits", //validate that app properly queries features and obeys feature limitations
			"VK_LAYER_LUNARG_draw_state", //validate the descriptor set, pipeline state, and dynamic state; validate the interfaces between SPIR - V modules and the graphics pipeline
			"VK_LAYER_LUNARG_image", //validate texture formats and render target formats
			"VK_LAYER_LUNARG_mem_tracker", //track and validate GPU memory and its binding to objects and command buffers
			"VK_LAYER_LUNARG_object_tracker", //track all Vulkan objects and flag invalid objects and object memory leaks
			"VK_LAYER_LUNARG_param_checker", //validate API parameter values
			"VK_LAYER_LUNARG_swapchain", //validate the use of the WSI "swapchain" extensions
			"VK_LAYER_LUNARG_threading", //check validity of multi - threaded API usage		
			"VK_LAYER_LUNARG_standard_validation", // include all above		
			"VK_LAYER_LUNARG_api_dump", //print API calls and their parameters and values
		};

		//auto vk_extensions = deviceExtensionsProperties.extensionsName;
		std::vector<const char*> vk_extensions = {
			//"VK_LUNARG_DEBUG_MARKER", // not supported!
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,	//"VK_KHR_swapchain",
			//"VK_NV_glsl_shader", // nv only!
		};

		std::vector<VkDeviceQueueCreateInfo> vk_deviceQueueCreateInfos(1);
		std::vector<std::vector<float>> vk_deviceQueuesPriorities(vk_deviceQueueCreateInfos.size(), std::vector<float>(1, 0.0f));
		{
			for(size_t i = 0; i < vk_deviceQueueCreateInfos.size(); ++i)
			{
				auto &vk_deviceQueueCreateInfo = vk_deviceQueueCreateInfos[i];
				auto &vk_deviceQueuePriorities = vk_deviceQueuesPriorities[i];

				vk_deviceQueueCreateInfos[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				vk_deviceQueueCreateInfos[i].pNext = nullptr;
				vk_deviceQueueCreateInfos[i].flags = 0;
				vk_deviceQueueCreateInfos[i].queueFamilyIndex = i;
				vk_deviceQueueCreateInfos[i].queueCount = vk_deviceQueuePriorities.size();
				vk_deviceQueueCreateInfos[i].pQueuePriorities = vk_deviceQueuePriorities.data();
			}
		}

		VkPhysicalDeviceFeatures vk_physicalDeviceFeatures;
		{
			vk_physicalDeviceFeatures.robustBufferAccess = VK_FALSE;
			vk_physicalDeviceFeatures.fullDrawIndexUint32 = VK_FALSE;
			vk_physicalDeviceFeatures.imageCubeArray = VK_FALSE;
			vk_physicalDeviceFeatures.independentBlend = VK_FALSE;
			vk_physicalDeviceFeatures.geometryShader = VK_FALSE;
			vk_physicalDeviceFeatures.tessellationShader = VK_FALSE;
			vk_physicalDeviceFeatures.sampleRateShading = VK_FALSE;
			vk_physicalDeviceFeatures.dualSrcBlend = VK_FALSE;
			vk_physicalDeviceFeatures.logicOp = VK_FALSE;
			vk_physicalDeviceFeatures.multiDrawIndirect = VK_FALSE;
			vk_physicalDeviceFeatures.drawIndirectFirstInstance = VK_FALSE;
			vk_physicalDeviceFeatures.depthClamp = VK_FALSE;
			vk_physicalDeviceFeatures.depthBiasClamp = VK_FALSE;
			vk_physicalDeviceFeatures.fillModeNonSolid = VK_FALSE;
			vk_physicalDeviceFeatures.depthBounds = VK_FALSE;
			vk_physicalDeviceFeatures.wideLines = VK_FALSE;
			vk_physicalDeviceFeatures.largePoints = VK_FALSE;
			vk_physicalDeviceFeatures.alphaToOne = VK_FALSE;
			vk_physicalDeviceFeatures.multiViewport = VK_FALSE;
			vk_physicalDeviceFeatures.samplerAnisotropy = VK_FALSE;
			vk_physicalDeviceFeatures.textureCompressionETC2 = VK_FALSE;
			vk_physicalDeviceFeatures.textureCompressionASTC_LDR = VK_FALSE;
			vk_physicalDeviceFeatures.textureCompressionBC = VK_FALSE;
			vk_physicalDeviceFeatures.occlusionQueryPrecise = VK_FALSE;
			vk_physicalDeviceFeatures.pipelineStatisticsQuery = VK_FALSE;
			vk_physicalDeviceFeatures.vertexPipelineStoresAndAtomics = VK_FALSE;
			vk_physicalDeviceFeatures.fragmentStoresAndAtomics = VK_FALSE;
			vk_physicalDeviceFeatures.shaderTessellationAndGeometryPointSize = VK_FALSE;
			vk_physicalDeviceFeatures.shaderImageGatherExtended = VK_FALSE;
			vk_physicalDeviceFeatures.shaderStorageImageExtendedFormats = VK_FALSE;
			vk_physicalDeviceFeatures.shaderStorageImageMultisample = VK_FALSE;
			vk_physicalDeviceFeatures.shaderStorageImageReadWithoutFormat = VK_FALSE;
			vk_physicalDeviceFeatures.shaderStorageImageWriteWithoutFormat = VK_FALSE;
			vk_physicalDeviceFeatures.shaderUniformBufferArrayDynamicIndexing = VK_FALSE;
			vk_physicalDeviceFeatures.shaderSampledImageArrayDynamicIndexing = VK_FALSE;
			vk_physicalDeviceFeatures.shaderStorageBufferArrayDynamicIndexing = VK_FALSE;
			vk_physicalDeviceFeatures.shaderStorageImageArrayDynamicIndexing = VK_FALSE;
			vk_physicalDeviceFeatures.shaderClipDistance = VK_FALSE;
			vk_physicalDeviceFeatures.shaderCullDistance = VK_FALSE;
			vk_physicalDeviceFeatures.shaderFloat64 = VK_FALSE;
			vk_physicalDeviceFeatures.shaderInt64 = VK_FALSE;
			vk_physicalDeviceFeatures.shaderInt16 = VK_FALSE;
			vk_physicalDeviceFeatures.shaderResourceResidency = VK_FALSE;
			vk_physicalDeviceFeatures.shaderResourceMinLod = VK_FALSE;
			vk_physicalDeviceFeatures.sparseBinding = VK_FALSE;
			vk_physicalDeviceFeatures.sparseResidencyBuffer = VK_FALSE;
			vk_physicalDeviceFeatures.sparseResidencyImage2D = VK_FALSE;
			vk_physicalDeviceFeatures.sparseResidencyImage3D = VK_FALSE;
			vk_physicalDeviceFeatures.sparseResidency2Samples = VK_FALSE;
			vk_physicalDeviceFeatures.sparseResidency4Samples = VK_FALSE;
			vk_physicalDeviceFeatures.sparseResidency8Samples = VK_FALSE;
			vk_physicalDeviceFeatures.sparseResidency16Samples = VK_FALSE;
			vk_physicalDeviceFeatures.sparseResidencyAliased = VK_FALSE;
			vk_physicalDeviceFeatures.variableMultisampleRate = VK_FALSE;
			vk_physicalDeviceFeatures.inheritedQueries = VK_FALSE;
		}
		VkDeviceCreateInfo vk_deviceCreateInfo;
		{
			vk_deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			vk_deviceCreateInfo.pNext = nullptr;
			vk_deviceCreateInfo.flags = 0;
			vk_deviceCreateInfo.queueCreateInfoCount = vk_deviceQueueCreateInfos.size();
			vk_deviceCreateInfo.pQueueCreateInfos = vk_deviceQueueCreateInfos.data();
			vk_deviceCreateInfo.enabledLayerCount = vk_layers.size();
			vk_deviceCreateInfo.ppEnabledLayerNames = vk_layers.data();
			vk_deviceCreateInfo.enabledExtensionCount = vk_extensions.size();
			vk_deviceCreateInfo.ppEnabledExtensionNames = vk_extensions.data();
			vk_deviceCreateInfo.pEnabledFeatures = &vk_physicalDeviceFeatures;
		};

		auto vk_device = BVE::Vulkan::CreateDevice(vk_physicalDevice, &vk_deviceCreateInfo, nullptr);

		return vk_device;
	}();

	auto vk_queue = BVE::Vulkan::GetDeviceQueue(vk_device, 0, 0);
	auto vk_commandPool = BVE::Vulkan::CreateCommandPool(vk_device, VkCommandPoolCreateFlagBits::VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT, 0, nullptr);
	{
		BVE::Vulkan::ResetCommandPool(vk_device, vk_commandPool, VkCommandPoolResetFlagBits::VK_COMMAND_POOL_RESET_RELEASE_RESOURCES_BIT);
	}
	auto vk_descriptorPool = [&]()
	{
		std::vector<VkDescriptorPoolSize> vk_descriptorPoolSizes(2);
		{
			vk_descriptorPoolSizes[0].descriptorCount = 1;
			vk_descriptorPoolSizes[0].type = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			vk_descriptorPoolSizes[1].descriptorCount = 1;
			vk_descriptorPoolSizes[1].type = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		}
		VkDescriptorPoolCreateInfo vk_descriptorPoolCreateInfo;
		{
			vk_descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
			vk_descriptorPoolCreateInfo.pNext = nullptr;
			vk_descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
			vk_descriptorPoolCreateInfo.maxSets = 1;
			vk_descriptorPoolCreateInfo.poolSizeCount = vk_descriptorPoolSizes.size();
			vk_descriptorPoolCreateInfo.pPoolSizes = vk_descriptorPoolSizes.data();
		}
		auto vk_descriptorPool = BVE::Vulkan::CreateDescriptorPool(vk_device, &vk_descriptorPoolCreateInfo, nullptr);

		return vk_descriptorPool;
	}();

	auto vk_surface = BVE::Vulkan::CreateWin32SurfaceKHR(vk_instance, hInstance, hWnd, nullptr);
	auto vk_surfaceCapabilities = BVE::Vulkan::GetPhysicalDeviceSurfaceCapabilitiesKHR(vk_physicalDevice, vk_surface);
	auto vk_physicalDeviceSurfacePresentModes = BVE::Vulkan::GetPhysicalDeviceSurfacePresentModesKHR(vk_physicalDevice, vk_surface);
	auto vk_surfaceFormats = BVE::Vulkan::GetPhysicalDeviceSurfaceFormatsKHR(vk_physicalDevice, vk_surface);
	auto vk_format = vk_surfaceFormats[0].format;
	auto vk_colorSpace = vk_surfaceFormats[0].colorSpace;

	auto vk_swapchain = [&]()
	{
		VkBool32 vk_surfaceSupport = VK_FALSE;
		BVE::Vulkan::Exceptions::Perform(vkGetPhysicalDeviceSurfaceSupportKHR(vk_physicalDevice, 0, vk_surface, &vk_surfaceSupport));

		VkSwapchainCreateInfoKHR vk_SwapchainCreateInfo;
		{
			vk_SwapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			vk_SwapchainCreateInfo.pNext = nullptr;
			vk_SwapchainCreateInfo.flags = 0;
			vk_SwapchainCreateInfo.surface = vk_surface;
			vk_SwapchainCreateInfo.minImageCount = 2;
			vk_SwapchainCreateInfo.imageFormat = vk_format;
			vk_SwapchainCreateInfo.imageColorSpace = vk_colorSpace;
			vk_SwapchainCreateInfo.imageExtent;
			{
				vk_SwapchainCreateInfo.imageExtent.width = width;
				vk_SwapchainCreateInfo.imageExtent.height = height;
			}
			vk_SwapchainCreateInfo.imageArrayLayers = 1;
			vk_SwapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
			vk_SwapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			vk_SwapchainCreateInfo.queueFamilyIndexCount = 0;
			vk_SwapchainCreateInfo.pQueueFamilyIndices = nullptr;
			vk_SwapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			vk_SwapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			vk_SwapchainCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
			vk_SwapchainCreateInfo.clipped = true;
			vk_SwapchainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
		}

		auto vk_swapchain = BVE::Vulkan::CreateSwapchainKHR(vk_device, &vk_SwapchainCreateInfo, nullptr);

		return vk_swapchain;
	}();
	auto vk_swapchainImages = BVE::Vulkan::GetSwapchainImagesKHR(vk_device, vk_swapchain);
	auto vk_swapchainImageViews = [&]()
	{
		std::vector<VkImageView> vk_swapchainImageViews(vk_swapchainImages.size());

		for(size_t i = 0; i < vk_swapchainImages.size(); i++)
		{
			auto &vk_swapchainImage = vk_swapchainImages[i];

			auto vk_commandBuffers = BVE::Vulkan::AllocateCommandBuffers(vk_device, vk_commandPool, 1, VK_COMMAND_BUFFER_LEVEL_PRIMARY);
			{
				VkCommandBufferInheritanceInfo vk_commandBufferInheritanceInfo;
				{
					vk_commandBufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
					vk_commandBufferInheritanceInfo.pNext = NULL;
					vk_commandBufferInheritanceInfo.renderPass = VK_NULL_HANDLE;
					vk_commandBufferInheritanceInfo.subpass = 0;
					vk_commandBufferInheritanceInfo.framebuffer = VK_NULL_HANDLE;
					vk_commandBufferInheritanceInfo.occlusionQueryEnable = VK_FALSE;
					vk_commandBufferInheritanceInfo.queryFlags = 0;
					vk_commandBufferInheritanceInfo.pipelineStatistics = 0;
				}
				VkCommandBufferBeginInfo vk_commandBufferBeginInfo;
				{
					vk_commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
					vk_commandBufferBeginInfo.pNext = NULL;
					vk_commandBufferBeginInfo.flags = 0;
					vk_commandBufferBeginInfo.pInheritanceInfo = &vk_commandBufferInheritanceInfo;
				};
				BVE::Vulkan::BeginCommandBuffer(vk_commandBuffers[0], &vk_commandBufferBeginInfo);
				{
					VkImageMemoryBarrier vk_imageMemoryBarrier;
					{
						vk_imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
						vk_imageMemoryBarrier.pNext = nullptr;
						vk_imageMemoryBarrier.srcAccessMask = 0;
						vk_imageMemoryBarrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
						vk_imageMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
						vk_imageMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
						vk_imageMemoryBarrier.srcQueueFamilyIndex = 0;
						vk_imageMemoryBarrier.dstQueueFamilyIndex = 0;
						vk_imageMemoryBarrier.image = vk_swapchainImage;
						vk_imageMemoryBarrier.subresourceRange = {VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
					}

					vkCmdPipelineBarrier(
						vk_commandBuffers[0],
						VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
						VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
						0,
						0, nullptr,
						0, nullptr,
						1, &vk_imageMemoryBarrier
					);
					/*vkCmdWaitEvents(
						vk_commandBuffers[0],
						0, nullptr,
						0, 0,
						0, nullptr,
						0, nullptr,
						1, &vk_imageMemoryBarrier
					);*/
				}
				BVE::Vulkan::EndCommandBuffer(vk_commandBuffers[0]);

				VkPipelineStageFlags vk_pipelineStageFlags = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
				std::vector<VkSubmitInfo> vk_submitInfos(1);
				{
					vk_submitInfos[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
					vk_submitInfos[0].pNext = NULL;
					vk_submitInfos[0].waitSemaphoreCount = 0;
					vk_submitInfos[0].pWaitSemaphores = NULL;
					vk_submitInfos[0].pWaitDstStageMask = &vk_pipelineStageFlags;
					vk_submitInfos[0].commandBufferCount = vk_commandBuffers.size();
					vk_submitInfos[0].pCommandBuffers = vk_commandBuffers.data();
					vk_submitInfos[0].signalSemaphoreCount = 0;
					vk_submitInfos[0].pSignalSemaphores = NULL;
				}
				BVE::Vulkan::QueueSubmit(vk_queue, vk_submitInfos, 0);
				BVE::Vulkan::QueueWaitIdle(vk_queue);
				BVE::Vulkan::DeviceWaitIdle(vk_device);
			}

			auto &vk_swapchainImageView = vk_swapchainImageViews[i];

			VkImageViewCreateInfo vk_imageViewCreateInfo;
			{
				vk_imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				vk_imageViewCreateInfo.pNext = nullptr;
				vk_imageViewCreateInfo.flags = 0;
				vk_imageViewCreateInfo.image = vk_swapchainImage;
				vk_imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
				vk_imageViewCreateInfo.format = vk_format;
				vk_imageViewCreateInfo.components;
				{
					vk_imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
					vk_imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
					vk_imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
					vk_imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
				};
				vk_imageViewCreateInfo.subresourceRange;
				{
					vk_imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
					vk_imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
					vk_imageViewCreateInfo.subresourceRange.levelCount = 1;
					vk_imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
					vk_imageViewCreateInfo.subresourceRange.layerCount = 1;
				}
			}

			vk_swapchainImageView = BVE::Vulkan::CreateImageView(vk_device, &vk_imageViewCreateInfo, nullptr);
		}

		return std::move(vk_swapchainImageViews);
	}();

	auto vk_depthFormat = VK_FORMAT_D16_UNORM;// VK_FORMAT_D32_SFLOAT;
	auto vk_depthImage = [&]()
	{
		VkImageCreateInfo vk_imageCreateInfo;
		{
			vk_imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			vk_imageCreateInfo.pNext = nullptr;
			vk_imageCreateInfo.flags = 0;
			vk_imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
			vk_imageCreateInfo.format = vk_depthFormat;
			vk_imageCreateInfo.extent = {width, height, 1};
			vk_imageCreateInfo.mipLevels = 1;
			vk_imageCreateInfo.arrayLayers = 1;
			vk_imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
			vk_imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
			vk_imageCreateInfo.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			vk_imageCreateInfo.sharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
			vk_imageCreateInfo.queueFamilyIndexCount = 0;
			vk_imageCreateInfo.pQueueFamilyIndices = nullptr;
			vk_imageCreateInfo.initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;// VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		}

		auto vk_depthImage = BVE::Vulkan::CreateImage(vk_device, &vk_imageCreateInfo, nullptr);

		auto vk_memoryRequirements = BVE::Vulkan::GetImageMemoryRequirements(vk_device, vk_depthImage);

		VkDeviceMemory vk_deviceMemory;
		{
			VkMemoryAllocateInfo vk_memoryAllocateInfo;
			{
				vk_memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				vk_memoryAllocateInfo.pNext = nullptr;
				vk_memoryAllocateInfo.allocationSize = vk_memoryRequirements.size;
				vk_memoryAllocateInfo.memoryTypeIndex = BVE::Vulkan::GetCorrectMemoryType(vk_physicalDeviceMemoryProperties, vk_memoryRequirements.memoryTypeBits, 0);
			}

			vk_deviceMemory = BVE::Vulkan::AllocateMemory(vk_device, &vk_memoryAllocateInfo, nullptr);

			BVE::Vulkan::BindImageMemory(vk_device, vk_depthImage, vk_deviceMemory, 0);
		}

		return vk_depthImage;
	}();
	auto vk_depthImageView = [&]()
	{
		VkImageViewCreateInfo vk_imageViewCreateInfo;
		{
			vk_imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			vk_imageViewCreateInfo.pNext = nullptr;
			vk_imageViewCreateInfo.flags = 0;
			vk_imageViewCreateInfo.image = vk_depthImage;
			vk_imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			vk_imageViewCreateInfo.format = vk_depthFormat;
			vk_imageViewCreateInfo.components;
			{
				vk_imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
				vk_imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
				vk_imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
				vk_imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
			}
			vk_imageViewCreateInfo.subresourceRange;
			{
				vk_imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
				vk_imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
				vk_imageViewCreateInfo.subresourceRange.levelCount = 1;
				vk_imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
				vk_imageViewCreateInfo.subresourceRange.layerCount = 1;
			}
		}

		auto vk_depthImageView = BVE::Vulkan::CreateImageView(vk_device, &vk_imageViewCreateInfo, nullptr);

		return vk_depthImageView;
	}();

	auto vk_subpassAttachmentDescriptions = [&]()
	{
		std::vector<VkAttachmentDescription> vk_subpassAttachmentDescriptions(2);

		vk_subpassAttachmentDescriptions[0];
		{
			vk_subpassAttachmentDescriptions[0].flags = 0;	// VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;
			vk_subpassAttachmentDescriptions[0].format = vk_format;
			vk_subpassAttachmentDescriptions[0].samples = VK_SAMPLE_COUNT_1_BIT;
			vk_subpassAttachmentDescriptions[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			vk_subpassAttachmentDescriptions[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			vk_subpassAttachmentDescriptions[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			vk_subpassAttachmentDescriptions[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			vk_subpassAttachmentDescriptions[0].initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			vk_subpassAttachmentDescriptions[0].finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}
		vk_subpassAttachmentDescriptions[1];
		{
			vk_subpassAttachmentDescriptions[1].flags = 0;	// VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;
			vk_subpassAttachmentDescriptions[1].format = vk_depthFormat;
			vk_subpassAttachmentDescriptions[1].samples = VK_SAMPLE_COUNT_1_BIT;
			vk_subpassAttachmentDescriptions[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			vk_subpassAttachmentDescriptions[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			vk_subpassAttachmentDescriptions[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			vk_subpassAttachmentDescriptions[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			vk_subpassAttachmentDescriptions[1].initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			vk_subpassAttachmentDescriptions[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		}

		return std::move(vk_subpassAttachmentDescriptions);
	}();
	auto vk_colorAttachmentReferences = [&]()
	{
		std::vector<VkAttachmentReference> vk_colorAttachmentReferences(1);

		vk_colorAttachmentReferences[0];
		{
			vk_colorAttachmentReferences[0].attachment = 0;
			vk_colorAttachmentReferences[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}

		return std::move(vk_colorAttachmentReferences);
	}();
	auto vk_depthAttachmentReference = [&]()
	{
		VkAttachmentReference vk_depthAttachmentReference;

		vk_depthAttachmentReference.attachment = 1;
		vk_depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		return vk_depthAttachmentReference;
	}();
	auto vk_renderPass = [&]()
	{
		std::vector<VkSubpassDescription> vk_subpassDescriptions(1);
		{
			vk_subpassDescriptions[0];
			{
				vk_subpassDescriptions[0].flags = 0;
				vk_subpassDescriptions[0].pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
				vk_subpassDescriptions[0].inputAttachmentCount = 0;
				vk_subpassDescriptions[0].pInputAttachments = nullptr;
				vk_subpassDescriptions[0].colorAttachmentCount = vk_colorAttachmentReferences.size();
				vk_subpassDescriptions[0].pColorAttachments = vk_colorAttachmentReferences.data();
				vk_subpassDescriptions[0].pResolveAttachments = nullptr;
				vk_subpassDescriptions[0].pDepthStencilAttachment = &vk_depthAttachmentReference;
				vk_subpassDescriptions[0].preserveAttachmentCount = 0;
				vk_subpassDescriptions[0].pResolveAttachments = nullptr;
			}
		}

		VkRenderPassCreateInfo vk_renderPassCreateInfo;
		{
			vk_renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			vk_renderPassCreateInfo.pNext = nullptr;
			vk_renderPassCreateInfo.flags = 0;
			vk_renderPassCreateInfo.attachmentCount = vk_subpassAttachmentDescriptions.size();
			vk_renderPassCreateInfo.pAttachments = vk_subpassAttachmentDescriptions.data();
			vk_renderPassCreateInfo.subpassCount = vk_subpassDescriptions.size(); // greater than 0
			vk_renderPassCreateInfo.pSubpasses = vk_subpassDescriptions.data();
			vk_renderPassCreateInfo.dependencyCount = 0;
			vk_renderPassCreateInfo.pDependencies = nullptr;
		}

		auto vk_renderPass = BVE::Vulkan::CreateRenderPass(vk_device, &vk_renderPassCreateInfo, nullptr);

		return vk_renderPass;
	}();

	auto vk_vertexShaderModule = [&]()
	{
		auto vk_vertexShaderSourceCode = std::move(loadFile(BVE_PATH(L"Media/Shaders/1.vert.spv")));

		VkShaderModuleCreateInfo vk_shaderModuleCreateInfo;
		{
			vk_shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			vk_shaderModuleCreateInfo.pNext = nullptr;
			vk_shaderModuleCreateInfo.flags = 0;
			vk_shaderModuleCreateInfo.codeSize = vk_vertexShaderSourceCode.size();
			vk_shaderModuleCreateInfo.pCode = vk_vertexShaderSourceCode.data();
		}

		auto vk_vertexShaderModule = BVE::Vulkan::CreateShaderModule(vk_device, &vk_shaderModuleCreateInfo, nullptr);

		return vk_vertexShaderModule;
	}();
	auto vk_fragmentShaderModule = [&]()
	{
		auto vk_fragmentShaderSourceCode = std::move(loadFile(BVE_PATH(L"Media/Shaders/1.frag.spv")));

		VkShaderModuleCreateInfo vk_shaderModuleCreateInfo;
		{
			vk_shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			vk_shaderModuleCreateInfo.pNext = nullptr;
			vk_shaderModuleCreateInfo.flags = 0;
			vk_shaderModuleCreateInfo.codeSize = vk_fragmentShaderSourceCode.size();
			vk_shaderModuleCreateInfo.pCode = vk_fragmentShaderSourceCode.data();
		}

		auto vk_fragmentShaderModule = BVE::Vulkan::CreateShaderModule(vk_device, &vk_shaderModuleCreateInfo, nullptr);

		return vk_fragmentShaderModule;
	}();

	auto vk_descriptorSetLayouts = [&]()
	{
		std::vector<VkDescriptorSetLayout> vk_descriptorSetLayouts(1);

		std::vector<VkDescriptorSetLayoutBinding> vk_descriptorSetLayoutBindings(2);
		{
			vk_descriptorSetLayoutBindings[0];
			{
				vk_descriptorSetLayoutBindings[0].binding = 0;
				vk_descriptorSetLayoutBindings[0].descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				vk_descriptorSetLayoutBindings[0].descriptorCount = 1;
				vk_descriptorSetLayoutBindings[0].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
				vk_descriptorSetLayoutBindings[0].pImmutableSamplers = nullptr;
			}
			vk_descriptorSetLayoutBindings[1];
			{
				vk_descriptorSetLayoutBindings[1].binding = 1;
				vk_descriptorSetLayoutBindings[1].descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				vk_descriptorSetLayoutBindings[1].descriptorCount = 1;
				vk_descriptorSetLayoutBindings[1].stageFlags = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
				vk_descriptorSetLayoutBindings[1].pImmutableSamplers = nullptr;
			}
		}
		VkDescriptorSetLayoutCreateInfo vk_descriptorSetLayoutCreateInfo;
		{
			vk_descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			vk_descriptorSetLayoutCreateInfo.pNext = nullptr;
			vk_descriptorSetLayoutCreateInfo.flags = 0;
			vk_descriptorSetLayoutCreateInfo.bindingCount = vk_descriptorSetLayoutBindings.size();
			vk_descriptorSetLayoutCreateInfo.pBindings = vk_descriptorSetLayoutBindings.data();
		}
		for(auto &vk_descriptorSetLayout : vk_descriptorSetLayouts)
		{
			vk_descriptorSetLayout = BVE::Vulkan::CreateDescriptorSetLayout(vk_device, &vk_descriptorSetLayoutCreateInfo, nullptr);
		}

		return vk_descriptorSetLayouts;
	}();
	auto vk_pipelineLayout = [&]()
	{
		VkPipelineLayoutCreateInfo vk_pipelineLayoutCreateInfo;
		{
			vk_pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			vk_pipelineLayoutCreateInfo.pNext = nullptr;
			vk_pipelineLayoutCreateInfo.flags = 0;
			vk_pipelineLayoutCreateInfo.setLayoutCount = vk_descriptorSetLayouts.size();
			vk_pipelineLayoutCreateInfo.pSetLayouts = vk_descriptorSetLayouts.data();
			vk_pipelineLayoutCreateInfo.pushConstantRangeCount = 0;
			vk_pipelineLayoutCreateInfo.pPushConstantRanges = nullptr;
		}
		auto vk_pipelineLayout = BVE::Vulkan::CreatePipelineLayout(vk_device, &vk_pipelineLayoutCreateInfo, nullptr);

		return vk_pipelineLayout;
	}();
	auto vk_pipelineCache = [&]()
	{
		VkPipelineCacheCreateInfo vk_pipelineCacheCreateInfo;
		{
			//memset(&vk_pipelineCacheCreateInfo, 0, sizeof(vk_pipelineCacheCreateInfo));
			//vk_pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;

			vk_pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
			vk_pipelineCacheCreateInfo.pNext = nullptr;
			vk_pipelineCacheCreateInfo.flags = 0;
			vk_pipelineCacheCreateInfo.initialDataSize = 0;
			vk_pipelineCacheCreateInfo.pInitialData = nullptr;
		}

		auto vk_pipelineCache = BVE::Vulkan::CreatePipelineCache(vk_device, &vk_pipelineCacheCreateInfo, nullptr);

		return vk_pipelineCache;
	}();

	auto vk_viewports = [&]()
	{
		std::vector<VkViewport> vk_viewports(1);

		for(auto &vk_viewport : vk_viewports)
		{
			vk_viewport.x = (float)0;
			vk_viewport.y = (float)0;
			vk_viewport.width = (float)width;
			vk_viewport.height = (float)height;
			vk_viewport.minDepth = (float)0;
			vk_viewport.maxDepth = (float)1;
		}

		return std::move(vk_viewports);
	}();
	auto vk_scissors = [&]()
	{
		std::vector<VkRect2D> vk_scissors(1);

		for(auto &vk_scissor : vk_scissors)
		{
			vk_scissor.offset.x = 0;
			vk_scissor.offset.y = 0;
			vk_scissor.extent.width = width;
			vk_scissor.extent.height = height;
		}

		return std::move(vk_scissors);
	}();

	auto vk_pipelines = [&]()
	{
		std::vector<VkPipelineShaderStageCreateInfo> vk_pipelineShaderStageCreateInfos(2);
		{
			vk_pipelineShaderStageCreateInfos[0];
			{
				vk_pipelineShaderStageCreateInfos[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				vk_pipelineShaderStageCreateInfos[0].pNext = nullptr;
				vk_pipelineShaderStageCreateInfos[0].flags = 0;
				vk_pipelineShaderStageCreateInfos[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
				vk_pipelineShaderStageCreateInfos[0].module = vk_vertexShaderModule;
				vk_pipelineShaderStageCreateInfos[0].pName = "main";
				vk_pipelineShaderStageCreateInfos[0].pSpecializationInfo = nullptr;
			}
			vk_pipelineShaderStageCreateInfos[1];
			{
				vk_pipelineShaderStageCreateInfos[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
				vk_pipelineShaderStageCreateInfos[1].pNext = nullptr;
				vk_pipelineShaderStageCreateInfos[1].flags = 0;
				vk_pipelineShaderStageCreateInfos[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
				vk_pipelineShaderStageCreateInfos[1].module = vk_fragmentShaderModule;
				vk_pipelineShaderStageCreateInfos[1].pName = "main";
				vk_pipelineShaderStageCreateInfos[1].pSpecializationInfo = nullptr;
			}
		}

		std::vector<VkVertexInputBindingDescription> vk_vertexInputBindingDescriptions(1);
		{
			vk_vertexInputBindingDescriptions[0].binding = 0;
			vk_vertexInputBindingDescriptions[0].stride = sizeof(BVE::float32) * 2;
			vk_vertexInputBindingDescriptions[0].inputRate = VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX;
		}
		std::vector<VkVertexInputAttributeDescription> vk_vertexInputAttributeDescriptions(1);
		{
			vk_vertexInputAttributeDescriptions[0].location = 0;
			vk_vertexInputAttributeDescriptions[0].binding = 0;
			vk_vertexInputAttributeDescriptions[0].format = VkFormat::VK_FORMAT_R32G32_SFLOAT;
			vk_vertexInputAttributeDescriptions[0].offset = 0;
		}
		VkPipelineVertexInputStateCreateInfo vk_pipelineVertexInputStateCreateInfo;
		{
			vk_pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			vk_pipelineVertexInputStateCreateInfo.pNext = nullptr;
			vk_pipelineVertexInputStateCreateInfo.flags = 0;
			vk_pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = vk_vertexInputBindingDescriptions.size();
			vk_pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = vk_vertexInputBindingDescriptions.data();
			vk_pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = vk_vertexInputAttributeDescriptions.size();
			vk_pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = vk_vertexInputAttributeDescriptions.data();
		}

		VkPipelineInputAssemblyStateCreateInfo vk_pipelineInputAssemblyStateCreateInfo;
		{
			vk_pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			vk_pipelineInputAssemblyStateCreateInfo.pNext = nullptr;
			vk_pipelineInputAssemblyStateCreateInfo.flags = 0;
			vk_pipelineInputAssemblyStateCreateInfo.topology = VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			//vk_pipelineInputAssemblyStateCreateInfo.topology = VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
			vk_pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;
		}

		VkPipelineViewportStateCreateInfo vk_pipelineViewportStateCreateInfo;
		{
			vk_pipelineViewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			vk_pipelineViewportStateCreateInfo.pNext = nullptr;
			vk_pipelineViewportStateCreateInfo.flags = 0;
			vk_pipelineViewportStateCreateInfo.viewportCount = vk_viewports.size();
			vk_pipelineViewportStateCreateInfo.pViewports = vk_viewports.data();
			vk_pipelineViewportStateCreateInfo.scissorCount = vk_scissors.size();
			vk_pipelineViewportStateCreateInfo.pScissors = vk_scissors.data();
		}

		VkPipelineRasterizationStateCreateInfo vk_pipelineRasterizationStateCreateInfo;
		{
			vk_pipelineRasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			vk_pipelineRasterizationStateCreateInfo.pNext = nullptr;
			vk_pipelineRasterizationStateCreateInfo.flags = 0;
			vk_pipelineRasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
			vk_pipelineRasterizationStateCreateInfo.rasterizerDiscardEnable = VK_FALSE;
			vk_pipelineRasterizationStateCreateInfo.polygonMode = VkPolygonMode::VK_POLYGON_MODE_FILL;
			vk_pipelineRasterizationStateCreateInfo.cullMode = VkCullModeFlagBits::VK_CULL_MODE_NONE;
			vk_pipelineRasterizationStateCreateInfo.frontFace = VkFrontFace::VK_FRONT_FACE_COUNTER_CLOCKWISE;
			vk_pipelineRasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
			vk_pipelineRasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
			vk_pipelineRasterizationStateCreateInfo.depthBiasClamp = 0.0f;
			vk_pipelineRasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;
			vk_pipelineRasterizationStateCreateInfo.lineWidth = 0.0f;
		}

		VkPipelineMultisampleStateCreateInfo vk_pipelineMultisampleStateCreateInfo;
		{
			vk_pipelineMultisampleStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			vk_pipelineMultisampleStateCreateInfo.pNext = nullptr;
			vk_pipelineMultisampleStateCreateInfo.flags = 0;
			vk_pipelineMultisampleStateCreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			vk_pipelineMultisampleStateCreateInfo.sampleShadingEnable = VK_FALSE;
			vk_pipelineMultisampleStateCreateInfo.minSampleShading = 0;
			vk_pipelineMultisampleStateCreateInfo.pSampleMask = nullptr;
			vk_pipelineMultisampleStateCreateInfo.alphaToCoverageEnable = VK_FALSE;
			vk_pipelineMultisampleStateCreateInfo.alphaToOneEnable = VK_FALSE;
		}

		VkPipelineDepthStencilStateCreateInfo vk_pipelineDepthStencilStateCreateInfo;
		{
			vk_pipelineDepthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			vk_pipelineDepthStencilStateCreateInfo.pNext = nullptr;
			vk_pipelineDepthStencilStateCreateInfo.flags = 0;
			vk_pipelineDepthStencilStateCreateInfo.depthTestEnable = VK_FALSE;
			vk_pipelineDepthStencilStateCreateInfo.depthWriteEnable = VK_FALSE;
			vk_pipelineDepthStencilStateCreateInfo.depthCompareOp = VkCompareOp::VK_COMPARE_OP_ALWAYS;
			vk_pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
			vk_pipelineDepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
			vk_pipelineDepthStencilStateCreateInfo.front;
			{
				vk_pipelineDepthStencilStateCreateInfo.front.failOp = VK_STENCIL_OP_KEEP;
				vk_pipelineDepthStencilStateCreateInfo.front.passOp = VK_STENCIL_OP_KEEP;
				vk_pipelineDepthStencilStateCreateInfo.front.depthFailOp = VK_STENCIL_OP_KEEP;
				vk_pipelineDepthStencilStateCreateInfo.front.compareOp = VK_COMPARE_OP_ALWAYS;
				vk_pipelineDepthStencilStateCreateInfo.front.compareMask = 0;
				vk_pipelineDepthStencilStateCreateInfo.front.writeMask = 0;
				vk_pipelineDepthStencilStateCreateInfo.front.reference = 0;
			}
			vk_pipelineDepthStencilStateCreateInfo.back;
			{
				vk_pipelineDepthStencilStateCreateInfo.back.failOp = VK_STENCIL_OP_KEEP;
				vk_pipelineDepthStencilStateCreateInfo.back.passOp = VK_STENCIL_OP_KEEP;
				vk_pipelineDepthStencilStateCreateInfo.back.depthFailOp = VK_STENCIL_OP_KEEP;
				vk_pipelineDepthStencilStateCreateInfo.back.compareOp = VK_COMPARE_OP_ALWAYS;
				vk_pipelineDepthStencilStateCreateInfo.back.compareMask = 0;
				vk_pipelineDepthStencilStateCreateInfo.back.writeMask = 0;
				vk_pipelineDepthStencilStateCreateInfo.back.reference = 0;
			}
			vk_pipelineDepthStencilStateCreateInfo.minDepthBounds = 0.0f;
			vk_pipelineDepthStencilStateCreateInfo.maxDepthBounds = 1.0f;
		}

		std::vector<VkPipelineColorBlendAttachmentState> vk_pipelineColorBlendAttachmentStates(vk_colorAttachmentReferences.size());
		{
			for(auto &vk_pipelineColorBlendAttachmentState : vk_pipelineColorBlendAttachmentStates)
			{
				vk_pipelineColorBlendAttachmentState.blendEnable = VK_FALSE;
				vk_pipelineColorBlendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
				vk_pipelineColorBlendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE;
				vk_pipelineColorBlendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
				vk_pipelineColorBlendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
				vk_pipelineColorBlendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
				vk_pipelineColorBlendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
				vk_pipelineColorBlendAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT;// | VK_COLOR_COMPONENT_A_BIT;
			}
		}
		VkPipelineColorBlendStateCreateInfo vk_pipelineColorBlendStateCreateInfo;
		{
			vk_pipelineColorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			vk_pipelineColorBlendStateCreateInfo.pNext = nullptr;
			vk_pipelineColorBlendStateCreateInfo.flags = 0;
			vk_pipelineColorBlendStateCreateInfo.logicOpEnable = VK_FALSE;
			vk_pipelineColorBlendStateCreateInfo.logicOp = VkLogicOp::VK_LOGIC_OP_CLEAR;
			vk_pipelineColorBlendStateCreateInfo.attachmentCount = vk_pipelineColorBlendAttachmentStates.size();
			vk_pipelineColorBlendStateCreateInfo.pAttachments = vk_pipelineColorBlendAttachmentStates.data();
			vk_pipelineColorBlendStateCreateInfo.blendConstants;
			{
				vk_pipelineColorBlendStateCreateInfo.blendConstants[0] = 1.0f;
				vk_pipelineColorBlendStateCreateInfo.blendConstants[1] = 1.0f;
				vk_pipelineColorBlendStateCreateInfo.blendConstants[2] = 1.0f;
				vk_pipelineColorBlendStateCreateInfo.blendConstants[3] = 1.0f;
			}
		}

		std::vector<VkDynamicState> vk_dynamicStates =  {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_SCISSOR
		};
		VkPipelineDynamicStateCreateInfo vk_pipelineDynamicStateCreateInfo;
		{
			vk_pipelineDynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			vk_pipelineDynamicStateCreateInfo.pNext = nullptr;
			vk_pipelineDynamicStateCreateInfo.flags = 0;
			vk_pipelineDynamicStateCreateInfo.dynamicStateCount = vk_dynamicStates.size();
			vk_pipelineDynamicStateCreateInfo.pDynamicStates = vk_dynamicStates.data();
		}

		std::vector<VkGraphicsPipelineCreateInfo> vk_graphicsPipelineCreateInfos(1);
		{
			for(auto &vk_graphicsPipelineCreateInfo : vk_graphicsPipelineCreateInfos)
			{
				vk_graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
				vk_graphicsPipelineCreateInfo.pNext = nullptr;
				vk_graphicsPipelineCreateInfo.flags = 0;// VkPipelineCreateFlagBits::VK_PIPELINE_CREATE_DISABLE_OPTIMIZATION_BIT;
				vk_graphicsPipelineCreateInfo.stageCount = vk_pipelineShaderStageCreateInfos.size();
				vk_graphicsPipelineCreateInfo.pStages = vk_pipelineShaderStageCreateInfos.data();
				vk_graphicsPipelineCreateInfo.pVertexInputState = &vk_pipelineVertexInputStateCreateInfo;
				vk_graphicsPipelineCreateInfo.pInputAssemblyState = &vk_pipelineInputAssemblyStateCreateInfo;
				vk_graphicsPipelineCreateInfo.pTessellationState = VK_NULL_HANDLE;
				vk_graphicsPipelineCreateInfo.pViewportState = &vk_pipelineViewportStateCreateInfo;
				vk_graphicsPipelineCreateInfo.pRasterizationState = &vk_pipelineRasterizationStateCreateInfo;
				vk_graphicsPipelineCreateInfo.pMultisampleState = &vk_pipelineMultisampleStateCreateInfo;
				vk_graphicsPipelineCreateInfo.pDepthStencilState = &vk_pipelineDepthStencilStateCreateInfo;
				vk_graphicsPipelineCreateInfo.pColorBlendState = &vk_pipelineColorBlendStateCreateInfo;
				vk_graphicsPipelineCreateInfo.pDynamicState = &vk_pipelineDynamicStateCreateInfo;
				vk_graphicsPipelineCreateInfo.layout = vk_pipelineLayout;
				vk_graphicsPipelineCreateInfo.renderPass = vk_renderPass;
				vk_graphicsPipelineCreateInfo.subpass = 0;
				vk_graphicsPipelineCreateInfo.basePipelineIndex = 0;
				vk_graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
			}
		}

		auto vk_pipelines = BVE::Vulkan::CreateGraphicsPipelines(vk_device, vk_pipelineCache, vk_graphicsPipelineCreateInfos, nullptr);

		return std::move(vk_pipelines);
	}();

	auto vk_framebufferAttachments = [&]()
	{
		std::vector<std::vector<VkImageView>> vk_framebufferAttachments(2, std::vector<VkImageView>(2));

		for(size_t i = 0; i < vk_framebufferAttachments.size(); ++i)
		{
			auto &vk_framebufferAttachment = vk_framebufferAttachments[i];
			vk_framebufferAttachment[0] = vk_swapchainImageViews[i];
			vk_framebufferAttachment[1] = vk_depthImageView;
		}

		return std::move(vk_framebufferAttachments);
	}();
	auto vk_framebuffers = [&]()
	{
		std::vector<VkFramebuffer> vk_framebuffers(vk_swapchainImages.size());

		for(size_t i = 0; i < vk_framebuffers.size(); ++i)
		{
			auto &vk_framebuffer = vk_framebuffers[i];

			VkFramebufferCreateInfo vk_framebufferCreateInfo;
			{
				vk_framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				vk_framebufferCreateInfo.pNext = nullptr;
				vk_framebufferCreateInfo.flags = 0;
				vk_framebufferCreateInfo.renderPass = vk_renderPass;
				vk_framebufferCreateInfo.attachmentCount = vk_framebufferAttachments[i].size();
				vk_framebufferCreateInfo.pAttachments = vk_framebufferAttachments[i].data();
				vk_framebufferCreateInfo.width = width;
				vk_framebufferCreateInfo.height = height;
				vk_framebufferCreateInfo.layers = 1;
			}

			vk_framebuffer = BVE::Vulkan::CreateFramebuffer(vk_device, &vk_framebufferCreateInfo, nullptr);
		}

		return std::move(vk_framebuffers);
	}();

	auto vk_textureLayout = VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	auto vk_textureImage = [&]()
	{
		VkImageCreateInfo vk_imageCreateInfo;
		{
			vk_imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			vk_imageCreateInfo.pNext = nullptr;
			vk_imageCreateInfo.flags = 0;// VkImageCreateFlagBits::VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT;
			vk_imageCreateInfo.imageType = VkImageType::VK_IMAGE_TYPE_2D;
			vk_imageCreateInfo.format = VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
			vk_imageCreateInfo.extent;
			{
				vk_imageCreateInfo.extent.width = 1024;
				vk_imageCreateInfo.extent.height = 1024;
				vk_imageCreateInfo.extent.depth = 1;
			}
			vk_imageCreateInfo.mipLevels = 1;
			vk_imageCreateInfo.arrayLayers = 1;
			vk_imageCreateInfo.samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
			vk_imageCreateInfo.tiling = VkImageTiling::VK_IMAGE_TILING_LINEAR;
			vk_imageCreateInfo.usage = VkImageUsageFlagBits::VK_IMAGE_USAGE_SAMPLED_BIT;
			vk_imageCreateInfo.sharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
			vk_imageCreateInfo.queueFamilyIndexCount = 0;
			vk_imageCreateInfo.pQueueFamilyIndices = nullptr;
			vk_imageCreateInfo.initialLayout = vk_textureLayout;
		}

		auto vk_image = BVE::Vulkan::CreateImage(vk_device, &vk_imageCreateInfo, nullptr);

		auto vk_memoryRequirements = BVE::Vulkan::GetImageMemoryRequirements(vk_device, vk_image);

		VkDeviceMemory vk_deviceMemory;
		{
			VkMemoryAllocateInfo vk_memoryAllocateInfo;
			{
				vk_memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				vk_memoryAllocateInfo.pNext = nullptr;
				vk_memoryAllocateInfo.allocationSize = vk_memoryRequirements.size;
				vk_memoryAllocateInfo.memoryTypeIndex = BVE::Vulkan::GetCorrectMemoryType(vk_physicalDeviceMemoryProperties, vk_memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			}

			vk_deviceMemory = BVE::Vulkan::AllocateMemory(vk_device, &vk_memoryAllocateInfo, nullptr);

			std::vector<BVE::uint32> textureData(1024 * 1024 * 1);
			{
				for(size_t x = 0; x < 1024; ++x)
				for(size_t y = 0; y < 1024; ++y)
				{
					auto &color = textureData[y * 1024 + x];
					color = 0xFF000000 | (((BVE::uint32)(x * 255 / 1024)) << 8) | (((BVE::uint32)(y * 255 / 1024)) << 16);
				}
				//for(auto &color : textureData)
				//{
				//	color = 0xFFFF0000; //  AA BB GG RR
				//}
			}

			auto data = BVE::Vulkan::MapMemory(vk_device, vk_deviceMemory, 0, vk_memoryAllocateInfo.allocationSize, 0);

			memcpy(data, textureData.data(), textureData.size() * sizeof(BVE::uint32));

			BVE::Vulkan::UnmapMemory(vk_device, vk_deviceMemory);

			BVE::Vulkan::BindImageMemory(vk_device, vk_image, vk_deviceMemory, 0);
		}

		return vk_image;
	}();
	auto vk_textureImageView = [&]()
	{
		VkImageViewCreateInfo vk_imageViewCreateInfo;
		{
			vk_imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			vk_imageViewCreateInfo.pNext = nullptr;
			vk_imageViewCreateInfo.flags = 0;
			vk_imageViewCreateInfo.image = vk_textureImage;
			vk_imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
			vk_imageViewCreateInfo.format = VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
			vk_imageViewCreateInfo.components;
			{
				vk_imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
				vk_imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
				vk_imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
				vk_imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
			}
			vk_imageViewCreateInfo.subresourceRange;
			{
				vk_imageViewCreateInfo.subresourceRange.aspectMask = VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT;
				vk_imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
				vk_imageViewCreateInfo.subresourceRange.levelCount = 1;
				vk_imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
				vk_imageViewCreateInfo.subresourceRange.layerCount = 1;
			}
		}

		auto vk_imageView = BVE::Vulkan::CreateImageView(vk_device, &vk_imageViewCreateInfo, nullptr);

		return vk_imageView;
	}();
	auto vk_textureSampler = [&]()
	{
		VkSamplerCreateInfo vk_samplerCreateInfo;
		{
			vk_samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			vk_samplerCreateInfo.pNext = nullptr;
			vk_samplerCreateInfo.flags = 0;
			vk_samplerCreateInfo.magFilter = VkFilter::VK_FILTER_NEAREST;
			vk_samplerCreateInfo.minFilter = VkFilter::VK_FILTER_NEAREST;
			vk_samplerCreateInfo.mipmapMode = VkSamplerMipmapMode::VK_SAMPLER_MIPMAP_MODE_NEAREST;
			vk_samplerCreateInfo.addressModeU = VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT;
			vk_samplerCreateInfo.addressModeV = VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT;
			vk_samplerCreateInfo.addressModeW = VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT;
			vk_samplerCreateInfo.mipLodBias = 0.0f;
			vk_samplerCreateInfo.anisotropyEnable = VK_FALSE;
			vk_samplerCreateInfo.maxAnisotropy = 0.0f;
			vk_samplerCreateInfo.compareEnable = VK_FALSE;
			vk_samplerCreateInfo.compareOp = VkCompareOp::VK_COMPARE_OP_ALWAYS;
			vk_samplerCreateInfo.minLod = 0.0f;
			vk_samplerCreateInfo.maxLod = 0.0f;
			vk_samplerCreateInfo.borderColor = VkBorderColor::VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
			vk_samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
		}

		auto vk_sampler = BVE::Vulkan::CreateSampler(vk_device, &vk_samplerCreateInfo, nullptr);

		return vk_sampler;
	}();

	auto vk_vertexBuffer = [&]()
	{
		VkBufferCreateInfo vk_bufferCreateInfo;
		{
			vk_bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vk_bufferCreateInfo.pNext = nullptr;
			vk_bufferCreateInfo.flags = 0;
			vk_bufferCreateInfo.size = sizeof(BVE::float32) * 2 * 3;
			vk_bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			vk_bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			vk_bufferCreateInfo.queueFamilyIndexCount = 0;
			vk_bufferCreateInfo.pQueueFamilyIndices = nullptr;
		}

		auto vk_buffer = BVE::Vulkan::CreateBuffer(vk_device, &vk_bufferCreateInfo, nullptr);

		auto vk_memoryRequirements = BVE::Vulkan::GetBufferMemoryRequirements(vk_device, vk_buffer);

		auto vk_deviceMemory = [&]()
		{
			VkMemoryAllocateInfo vk_memoryAllocateInfo;
			{
				vk_memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				vk_memoryAllocateInfo.pNext = nullptr;
				vk_memoryAllocateInfo.allocationSize = vk_memoryRequirements.size;
				vk_memoryAllocateInfo.memoryTypeIndex = BVE::Vulkan::GetCorrectMemoryType(vk_physicalDeviceMemoryProperties, vk_memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			}

			auto vk_deviceMemory = BVE::Vulkan::AllocateMemory(vk_device, &vk_memoryAllocateInfo, nullptr);

			return vk_deviceMemory;
		}();

		std::vector<BVE::float32> vArr = {
			-0.5f, +0.5f,
			+0.5f, +0.5f,
			-0.5f, -0.5f,
			+0.5f, -0.5f,
		};

		auto data = BVE::Vulkan::MapMemory(vk_device, vk_deviceMemory, 0, sizeof(BVE::float32) * 2 * 3, 0);

		memcpy(data, vArr.data(), vArr.size() * sizeof(BVE::float32));

		BVE::Vulkan::UnmapMemory(vk_device, vk_deviceMemory);

		BVE::Vulkan::BindBufferMemory(vk_device, vk_buffer, vk_deviceMemory, 0);

		return vk_buffer;
	}();

	auto vk_indexBuffer = [&]()
	{
		VkBufferCreateInfo vk_bufferCreateInfo;
		{
			vk_bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vk_bufferCreateInfo.pNext = nullptr;
			vk_bufferCreateInfo.flags = 0;
			vk_bufferCreateInfo.size = sizeof(BVE::uint32) * 6;
			vk_bufferCreateInfo.usage = VkBufferUsageFlagBits::VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			vk_bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			vk_bufferCreateInfo.queueFamilyIndexCount = 0;
			vk_bufferCreateInfo.pQueueFamilyIndices = nullptr;
		}

		auto vk_buffer = BVE::Vulkan::CreateBuffer(vk_device, &vk_bufferCreateInfo, nullptr);

		auto vk_memoryRequirements = BVE::Vulkan::GetBufferMemoryRequirements(vk_device, vk_buffer);

		auto vk_deviceMemory = [&]()
		{
			VkMemoryAllocateInfo vk_memoryAllocateInfo;
			{
				vk_memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				vk_memoryAllocateInfo.pNext = nullptr;
				vk_memoryAllocateInfo.allocationSize = vk_memoryRequirements.size;
				vk_memoryAllocateInfo.memoryTypeIndex = BVE::Vulkan::GetCorrectMemoryType(vk_physicalDeviceMemoryProperties, vk_memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			}

			auto vk_deviceMemory = BVE::Vulkan::AllocateMemory(vk_device, &vk_memoryAllocateInfo, nullptr);

			return vk_deviceMemory;
		}();

		std::vector<BVE::uint32> vArr = {
			0, 1, 2,
			1, 3, 2
		};

		auto data = BVE::Vulkan::MapMemory(vk_device, vk_deviceMemory, 0, sizeof(BVE::uint32) * 6, 0);

		memcpy(data, vArr.data(), vArr.size() * sizeof(BVE::uint32));

		BVE::Vulkan::UnmapMemory(vk_device, vk_deviceMemory);

		BVE::Vulkan::BindBufferMemory(vk_device, vk_buffer, vk_deviceMemory, 0);

		return vk_buffer;
	}();

	auto vk_uniformBuffer = [&]()
	{
		VkBufferCreateInfo vk_bufferCreateInfo;
		{
			vk_bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			vk_bufferCreateInfo.pNext = nullptr;
			vk_bufferCreateInfo.flags = 0;
			vk_bufferCreateInfo.size = sizeof(BVE::float32) * 4;
			vk_bufferCreateInfo.usage = VkBufferUsageFlagBits::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			vk_bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			vk_bufferCreateInfo.queueFamilyIndexCount = 0;
			vk_bufferCreateInfo.pQueueFamilyIndices = nullptr;
		}

		auto vk_buffer = BVE::Vulkan::CreateBuffer(vk_device, &vk_bufferCreateInfo, nullptr);

		auto vk_memoryRequirements = BVE::Vulkan::GetBufferMemoryRequirements(vk_device, vk_buffer);

		auto vk_deviceMemory = [&]()
		{
			VkMemoryAllocateInfo vk_memoryAllocateInfo;
			{
				vk_memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
				vk_memoryAllocateInfo.pNext = nullptr;
				vk_memoryAllocateInfo.allocationSize = vk_memoryRequirements.size;
				vk_memoryAllocateInfo.memoryTypeIndex = BVE::Vulkan::GetCorrectMemoryType(vk_physicalDeviceMemoryProperties, vk_memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			}

			auto vk_deviceMemory = BVE::Vulkan::AllocateMemory(vk_device, &vk_memoryAllocateInfo, nullptr);

			return vk_deviceMemory;
		}();

		std::vector<BVE::float32> vArr = {
			1.0f, 1.0f, 0.0f, 1.0f
		};

		auto data = BVE::Vulkan::MapMemory(vk_device, vk_deviceMemory, 0, sizeof(BVE::uint32) * 4, 0);

		memcpy(data, vArr.data(), vArr.size() * sizeof(BVE::float32));

		BVE::Vulkan::UnmapMemory(vk_device, vk_deviceMemory);

		BVE::Vulkan::BindBufferMemory(vk_device, vk_buffer, vk_deviceMemory, 0);

		return vk_buffer;
	}();

	auto vk_descriptorSets = BVE::Vulkan::AllocateDescriptorSets(vk_device, vk_descriptorPool, 1, vk_descriptorSetLayouts.data());
	{
		std::vector<VkDescriptorBufferInfo> vk_descriptorBufferInfos(1);
		{
			vk_descriptorBufferInfos[0].buffer = vk_uniformBuffer;
			vk_descriptorBufferInfos[0].offset = 0;
			vk_descriptorBufferInfos[0].range = VK_WHOLE_SIZE;
		}
		std::vector<VkDescriptorImageInfo> vk_descriptorImageInfos(1);
		{
			vk_descriptorImageInfos[0].sampler = vk_textureSampler;
			vk_descriptorImageInfos[0].imageView = vk_textureImageView;
			vk_descriptorImageInfos[0].imageLayout = vk_textureLayout;
		}

		VkDescriptorBufferInfo;
		std::vector<VkWriteDescriptorSet> vk_writeDescriptorSet(2);
		{
			vk_writeDescriptorSet[0];
			{
				vk_writeDescriptorSet[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				vk_writeDescriptorSet[0].pNext = nullptr;
				vk_writeDescriptorSet[0].dstSet = vk_descriptorSets[0];
				vk_writeDescriptorSet[0].dstBinding = 0;
				vk_writeDescriptorSet[0].dstArrayElement = 0;
				vk_writeDescriptorSet[0].descriptorCount = vk_descriptorBufferInfos.size();
				vk_writeDescriptorSet[0].descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
				vk_writeDescriptorSet[0].pImageInfo = nullptr;
				vk_writeDescriptorSet[0].pBufferInfo = vk_descriptorBufferInfos.data();
				vk_writeDescriptorSet[0].pTexelBufferView = nullptr;
			}
			vk_writeDescriptorSet[1];
			{
				vk_writeDescriptorSet[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
				vk_writeDescriptorSet[1].pNext = nullptr;
				vk_writeDescriptorSet[1].dstSet = vk_descriptorSets[0];
				vk_writeDescriptorSet[1].dstBinding = 1;
				vk_writeDescriptorSet[1].dstArrayElement = 0;
				vk_writeDescriptorSet[1].descriptorCount = vk_descriptorImageInfos.size();
				vk_writeDescriptorSet[1].descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				vk_writeDescriptorSet[1].pImageInfo = vk_descriptorImageInfos.data();
				vk_writeDescriptorSet[1].pBufferInfo = nullptr;
				vk_writeDescriptorSet[1].pTexelBufferView = nullptr;
			}
		}
		vkUpdateDescriptorSets(vk_device, vk_writeDescriptorSet.size(), vk_writeDescriptorSet.data(), 0, nullptr);
	}

	auto vk_commandBuffers = [&]()
	{
		auto vk_commandBuffers = BVE::Vulkan::AllocateCommandBuffers(vk_device, vk_commandPool, vk_framebuffers.size(), VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY);

		for(size_t i = 0; i < vk_commandBuffers.size(); ++i)
		{
			BVE::Vulkan::ResetCommandBuffer(vk_commandBuffers[i], VkCommandBufferResetFlagBits::VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
			vk_commandBuffers[i];
			{
				VkCommandBufferInheritanceInfo vk_commandBufferInheritanceInfo;
				{
					vk_commandBufferInheritanceInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_INHERITANCE_INFO;
					vk_commandBufferInheritanceInfo.pNext = nullptr;
					vk_commandBufferInheritanceInfo.renderPass = VK_NULL_HANDLE;
					vk_commandBufferInheritanceInfo.subpass = 0;
					vk_commandBufferInheritanceInfo.framebuffer = vk_framebuffers[i];
					vk_commandBufferInheritanceInfo.occlusionQueryEnable = VK_FALSE;
					vk_commandBufferInheritanceInfo.queryFlags = 0;
					vk_commandBufferInheritanceInfo.pipelineStatistics = 0;
				}
				VkCommandBufferBeginInfo vk_commandBufferBeginInfo;
				{
					vk_commandBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
					vk_commandBufferBeginInfo.pNext = nullptr;
					vk_commandBufferBeginInfo.flags = 0;
					vk_commandBufferBeginInfo.pInheritanceInfo = &vk_commandBufferInheritanceInfo;
				}
				BVE::Vulkan::Exceptions::Perform(vkBeginCommandBuffer(vk_commandBuffers[i], &vk_commandBufferBeginInfo));
				{
					std::vector<VkClearValue> vk_clearValues(2);
					{
						vk_clearValues[0];
						{
							vk_clearValues[0].color.float32[0] = 1.0f;
							vk_clearValues[0].color.float32[1] = 0.0f;
							vk_clearValues[0].color.float32[2] = 0.0f;
							vk_clearValues[0].color.float32[3] = 1.0f;
						}
						vk_clearValues[1];
						{
							vk_clearValues[1].depthStencil.depth = 1.0f;
							vk_clearValues[1].depthStencil.stencil = 0;
						}
					}
					VkRenderPassBeginInfo vk_renderPassBeginInfo;
					{
						vk_renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
						vk_renderPassBeginInfo.pNext = nullptr;
						vk_renderPassBeginInfo.renderPass = vk_renderPass;
						vk_renderPassBeginInfo.framebuffer = vk_framebuffers[i];
						vk_renderPassBeginInfo.renderArea.offset;
						{
							vk_renderPassBeginInfo.renderArea.offset.x = 0;
							vk_renderPassBeginInfo.renderArea.offset.y = 0;
							vk_renderPassBeginInfo.renderArea.extent.width = width;
							vk_renderPassBeginInfo.renderArea.extent.height = height;
						}
						vk_renderPassBeginInfo.clearValueCount = vk_clearValues.size();
						vk_renderPassBeginInfo.pClearValues = vk_clearValues.data();
					};
					vkCmdBeginRenderPass(vk_commandBuffers[i], &vk_renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
					{
						vkCmdBindDescriptorSets(
							vk_commandBuffers[i],
							VK_PIPELINE_BIND_POINT_GRAPHICS,
							vk_pipelineLayout,
							0,
							vk_descriptorSets.size(),
							vk_descriptorSets.data(),
							0,
							nullptr
						);

						vkCmdBindPipeline(
							vk_commandBuffers[i],
							VK_PIPELINE_BIND_POINT_GRAPHICS,
							vk_pipelines[0]
						);

						vkCmdSetViewport(
							vk_commandBuffers[i],
							0,
							vk_viewports.size(),
							vk_viewports.data()
						);

						vkCmdSetScissor(
							vk_commandBuffers[i],
							0,
							vk_scissors.size(),
							vk_scissors.data()
						);

						std::vector<VkBuffer> vk_vertexBuffers = { vk_vertexBuffer };
						std::vector<VkDeviceSize> vk_vertexOffsets = { 0 };
						vkCmdBindVertexBuffers(
							vk_commandBuffers[i],
							0,
							vk_vertexBuffers.size(),
							vk_vertexBuffers.data(),
							vk_vertexOffsets.data()
						);

						/*vkCmdDraw(
							vk_commandBuffers[i],
							3,
							1,
							0,
							0
						);*/

						vkCmdBindIndexBuffer(
							vk_commandBuffers[i],
							vk_indexBuffer,
							0,
							VkIndexType::VK_INDEX_TYPE_UINT32
						);

						vkCmdDrawIndexed(
							vk_commandBuffers[i],
							6,
							1,
							0,
							0,
							0
						);
					}
					vkCmdEndRenderPass(vk_commandBuffers[i]);
				}
				BVE::Vulkan::Exceptions::Perform(vkEndCommandBuffer(vk_commandBuffers[i]));
			}
		}

		return std::move(vk_commandBuffers);
	}();

	bool done = false;
	while(!done)
	{
		MSG msg;
		{
			PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
			if(msg.message == WM_QUIT)
			{
				done = true;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		auto vk_currentImage = BVE::Vulkan::AcquireNextImageKHR(vk_device, vk_swapchain, UINT64_MAX, VK_NULL_HANDLE, VK_NULL_HANDLE);

		BVE::Vulkan::QueueWaitIdle(vk_queue);
		BVE::Vulkan::DeviceWaitIdle(vk_device);

		VkPipelineStageFlags vk_pipelineStageFlags = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		std::vector<VkSubmitInfo> vk_submitInfos(1);
		{
			vk_submitInfos[0].sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			vk_submitInfos[0].pNext = nullptr;
			vk_submitInfos[0].waitSemaphoreCount = 0;
			vk_submitInfos[0].pWaitSemaphores = nullptr;
			vk_submitInfos[0].pWaitDstStageMask = &vk_pipelineStageFlags;
			vk_submitInfos[0].commandBufferCount = 1;// vk_commandBuffers.size();
			vk_submitInfos[0].pCommandBuffers = &vk_commandBuffers[vk_currentImage];// vk_commandBuffers.data();
			vk_submitInfos[0].signalSemaphoreCount = 0;
			vk_submitInfos[0].pSignalSemaphores = nullptr;
		}

		BVE::Vulkan::QueueSubmit(vk_queue, vk_submitInfos, VK_NULL_HANDLE);

		BVE::Vulkan::QueueWaitIdle(vk_queue);
		BVE::Vulkan::DeviceWaitIdle(vk_device);

		VkResult vk_result;
		VkPresentInfoKHR vk_presentInfoKHR;
		{
			vk_presentInfoKHR.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			vk_presentInfoKHR.pNext = nullptr;
			vk_presentInfoKHR.waitSemaphoreCount = 0;
			vk_presentInfoKHR.pWaitSemaphores = nullptr;
			vk_presentInfoKHR.swapchainCount = 1;
			vk_presentInfoKHR.pSwapchains = &vk_swapchain;
			vk_presentInfoKHR.pImageIndices = &vk_currentImage;
			vk_presentInfoKHR.pResults = &vk_result;
		}
		BVE::Vulkan::Exceptions::Perform(vkQueuePresentKHR(vk_queue, &vk_presentInfoKHR));

		BVE::Vulkan::QueueWaitIdle(vk_queue);
		BVE::Vulkan::DeviceWaitIdle(vk_device);
	}
}

void main()
{
	BVE::Vulkan::Log::Clear();

	try
	{
		BVE::Vulkan::Log::Write("Start vulkan shit...");
		vulkan();
	}
	catch(...)
	{
		BVE::Vulkan::Log::Write("Some shit happen...");
		std::cout << "Some shit happen..." << std::endl;
	}	
}