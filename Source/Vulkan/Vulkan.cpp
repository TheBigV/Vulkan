#include "Vulkan.hpp"

#pragma region Vulkan
#pragma region Exception
KID::string														KID::Vulkan::Exception::Get() const
{
	return text;
}
#pragma endregion
#pragma region Definitions
#pragma region Instance
KID::Vulkan::Instance::Instance(const LayersName& layersName, const ExtensionsName& extensionsName)
{
	__KID_VULKAN_LOG_BEGIN__(Instance());

	VkApplicationInfo vk_applicationInfo;
	{
		vk_applicationInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		vk_applicationInfo.pNext = nullptr;
		vk_applicationInfo.pApplicationName = "amd suck large cocks";
		vk_applicationInfo.applicationVersion = 0;
		vk_applicationInfo.pEngineName = "fuck amd mom";
		vk_applicationInfo.engineVersion = 0;
		vk_applicationInfo.apiVersion = VK_MAKE_VERSION(1, 0, 3);// VK_MAKE_VERSION(1, 0, 3); VK_API_VERSION;
	}
	VkInstanceCreateInfo vk_instanceInfo;
	{
		vk_instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		vk_instanceInfo.pNext = nullptr;
		vk_instanceInfo.flags = 0;
		vk_instanceInfo.pApplicationInfo = &vk_applicationInfo;
		vk_instanceInfo.enabledLayerCount = layersName.size();
		vk_instanceInfo.ppEnabledLayerNames = layersName.data();
		vk_instanceInfo.enabledExtensionCount = extensionsName.size();
		vk_instanceInfo.ppEnabledExtensionNames = extensionsName.data();
	}

	vk_instance = CreateInstance(&vk_instanceInfo, nullptr);

#if __KID_VULKAN_DEBUG__
	vk_vkCreateDebugReportCallbackEXT = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(vk_instance, "vkCreateDebugReportCallbackEXT");

	VkDebugReportCallbackCreateInfoEXT vk_debugReportCallbackCreateInfoEXT;
	{
		vk_debugReportCallbackCreateInfoEXT.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
		vk_debugReportCallbackCreateInfoEXT.pNext = nullptr;
		vk_debugReportCallbackCreateInfoEXT.flags = VK_DEBUG_REPORT_INFORMATION_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_DEBUG_BIT_EXT;
		//vk_debugReportCallbackCreateInfoEXT.flags = VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_ERROR_BIT_EXT;
		vk_debugReportCallbackCreateInfoEXT.pfnCallback = Vk_DebugCallback;
		vk_debugReportCallbackCreateInfoEXT.pUserData = nullptr;
	}
	KID::Vulkan::Exceptions::Perform(vk_vkCreateDebugReportCallbackEXT(vk_instance, &vk_debugReportCallbackCreateInfoEXT, nullptr, &vk_debugReportCallbackEXT));
#endif

	auto vk_physicalDevices = KID::Vulkan::EnumeratePhysicalDevices(vk_instance);

	for(auto &vk_physicalDevice : vk_physicalDevices)
	{
		physicalDevices.push_back(new PhysicalDevice((Instance*)this, vk_physicalDevice));
	}

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::Instance::~Instance()
{
	__KID_VULKAN_LOG_BEGIN__(~Instance());

	for(auto &physicalDevice : physicalDevices)
	{
		delete physicalDevice;
	}

	__KID_DEPENDENCY_CHECK__;

#if __KID_VULKAN_DEBUG__
	vk_vkDestroyDebugReportCallbackEXT = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(vk_instance, "vkDestroyDebugReportCallbackEXT");

	vk_vkDestroyDebugReportCallbackEXT(vk_instance, vk_debugReportCallbackEXT, nullptr);
#endif

	DestroyInstance(vk_instance, nullptr);

	__KID_VULKAN_LOG_END__;
}
#if __KID_VULKAN_DEBUG__
VkBool32 __stdcall KID::Vulkan::Instance::Vk_DebugCallback(
	VkDebugReportFlagsEXT                       flags,
	VkDebugReportObjectTypeEXT                  objectType,
	uint64_t                                    object,
	size_t                                      location,
	int32_t                                     messageCode,
	const char*                                 pLayerPrefix,
	const char*                                 pMessage,
	void*                                       pUserData)
{
	auto info = [&]() {
		string text = "";
		if((flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) != 0) text += "{INFORMATION}";
		if((flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) != 0) text += "{WARNING}";
		if((flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) != 0) text += "{PERFORMANCE}";
		if((flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) != 0) text += "{ERROR}";
		if((flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) != 0) text += "{DEBUG}";
		return text;
	}();

	KID::Vulkan::Log::Timestamp();
	KID::Vulkan::Log::Write("\t[Instance][Debug][" + KID::string(pLayerPrefix) + "]" + info + KID::string(pMessage));
	return false;
}
#endif
#pragma endregion
#pragma region Physical Device
KID::Vulkan::PhysicalDevice::PhysicalDevice(Instance* instance_, VkPhysicalDevice vk_physicalDevice_):
	InstanceDependent(instance_),
	vk_physicalDevice(vk_physicalDevice_)
{
	__KID_VULKAN_LOG_BEGIN__(PhysicalDevice());

	vk_physicalDeviceProperties = KID::Vulkan::GetPhysicalDeviceProperties(vk_physicalDevice);
	vk_physicalDeviceMemoryProperties = KID::Vulkan::GetPhysicalDeviceMemoryProperties(vk_physicalDevice);

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::PhysicalDevice::~PhysicalDevice()
{
	__KID_VULKAN_LOG_BEGIN__(~PhysicalDevice());

	__KID_DEPENDENCY_CHECK__;

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region Device
KID::Vulkan::Device::Device(PhysicalDevice* physicalDevice_, const LayersName& layersName, const ExtensionsName& extensionsName):
	PhysicalDeviceDependent(physicalDevice_)
{
	__KID_VULKAN_LOG_BEGIN__(Device());

	std::vector<VkDeviceQueueCreateInfo> vk_deviceQueueCreateInfos(1);
	std::vector<std::vector<float32>> vk_deviceQueuesPriorities(vk_deviceQueueCreateInfos.size(), std::vector<float32>(1, 0.0f));
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

	//VkPhysicalDeviceFeatures::shaderClipDistance
	//VkPhysicalDeviceFeatures::shaderCullDistance
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
		vk_physicalDeviceFeatures.shaderClipDistance = VK_TRUE; //VK_FALSE;
		vk_physicalDeviceFeatures.shaderCullDistance = VK_TRUE; //VK_FALSE;
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
		vk_deviceCreateInfo.enabledLayerCount = layersName.size();
		vk_deviceCreateInfo.ppEnabledLayerNames = layersName.data();
		vk_deviceCreateInfo.enabledExtensionCount = extensionsName.size();
		vk_deviceCreateInfo.ppEnabledExtensionNames = extensionsName.data();
		vk_deviceCreateInfo.pEnabledFeatures = &vk_physicalDeviceFeatures;
	};

	vk_device = KID::Vulkan::CreateDevice(physicalDevice->Vk_GetPhysicalDevice(), &vk_deviceCreateInfo, nullptr);

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::Device::~Device()
{
	__KID_VULKAN_LOG_BEGIN__(~Device());

	__KID_DEPENDENCY_CHECK__;

	KID::Vulkan::DestroyDevice(vk_device, nullptr);

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region Surface
KID::Vulkan::Surface::Surface(PhysicalDevice* physicalDevice_, HINSTANCE hInstance_, HWND hWnd_):
	PhysicalDeviceDependent(physicalDevice_),
	win_hInstance(hInstance_),
	win_hWnd(hWnd_)
{
	__KID_VULKAN_LOG_BEGIN__(Surface());

	vk_surface = KID::Vulkan::CreateWin32SurfaceKHR(physicalDevice->GetInstance()->Vk_GetInstance(), win_hInstance, win_hWnd, nullptr);

	vk_surfaceCapabilities = KID::Vulkan::GetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice->Vk_GetPhysicalDevice(), vk_surface);
	vk_physicalDeviceSurfacePresentModes = std::move(KID::Vulkan::GetPhysicalDeviceSurfacePresentModesKHR(physicalDevice->Vk_GetPhysicalDevice(), vk_surface));
	vk_surfaceFormats = std::move(KID::Vulkan::GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice->Vk_GetPhysicalDevice(), vk_surface));

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::Surface::~Surface()
{
	__KID_VULKAN_LOG_BEGIN__(~Surface());

	__KID_DEPENDENCY_CHECK__;

	KID::Vulkan::DestroySurfaceKHR(physicalDevice->GetInstance()->Vk_GetInstance(), vk_surface, nullptr);

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region Swapchain
KID::Vulkan::Swapchain::Swapchain(Device* device_, Surface* surface_, uint32_t queueFamilyIndex_, Format format_, ColorSpace colorSpace_, Size vk_size_):
	DeviceDependent(device_),
	SurfaceDependent(surface_),
	vk_queueFamilyIndex(queueFamilyIndex_),
	vk_format(format_),
	vk_colorSpace(colorSpace_),
	vk_size(vk_size_)
{
	__KID_VULKAN_LOG_BEGIN__(Swapchain());

	if(GetPhysicalDeviceSurfaceSupportKHR(device->GetPhysicalDevice()->Vk_GetPhysicalDevice(), vk_queueFamilyIndex, surface->Vk_GetSurface()))
	{
		VkSwapchainCreateInfoKHR vk_SwapchainCreateInfo;
		{
			vk_SwapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			vk_SwapchainCreateInfo.pNext = nullptr;
			vk_SwapchainCreateInfo.flags = 0;
			vk_SwapchainCreateInfo.surface = surface->Vk_GetSurface();
			vk_SwapchainCreateInfo.minImageCount = 2;
			vk_SwapchainCreateInfo.imageFormat = vk_format;
			vk_SwapchainCreateInfo.imageColorSpace = vk_colorSpace;
			vk_SwapchainCreateInfo.imageExtent = vk_size;
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

		vk_swapchain = KID::Vulkan::CreateSwapchainKHR(device->Vk_GetDevice(), &vk_SwapchainCreateInfo, nullptr);

		auto vk_images = std::move(KID::Vulkan::GetSwapchainImagesKHR(device->Vk_GetDevice(), vk_swapchain));

		images.reserve(vk_images.size());
		{
			for(auto &vk_image : vk_images)
			{
				auto image = new SwapchainImage(
					this,
					vk_image,
					{vk_size_.width, vk_size_.height, 1},
					Image::Type::VK_IMAGE_TYPE_2D,
					vk_format,
					Image::UsageBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
				);
				images.push_back(image);
			}
		}
	}
	else
	{
		throw new Exception_NotSupported;
	}

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::Swapchain::~Swapchain()
{
	__KID_VULKAN_LOG_BEGIN__(~Swapchain());

	for(auto &image : images)
	{
		delete image;
	}

	__KID_DEPENDENCY_CHECK__;

	DestroySwapchainKHR(device->Vk_GetDevice(), vk_swapchain, nullptr);

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region DeviceMemory
KID::Vulkan::DeviceMemory::DeviceMemory(Image* image, VkMemoryPropertyFlags vk_memoryPropertyFlags):
	DeviceDependent(image->GetDevice())
{
	__KID_VULKAN_LOG_BEGIN__(DeviceMemory());

	auto vk_memoryRequirements = GetImageMemoryRequirements(device->Vk_GetDevice(), image->Vk_GetImage());

	VkMemoryAllocateInfo vk_memoryAllocateInfo;
	{
		vk_memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		vk_memoryAllocateInfo.pNext = nullptr;
		vk_memoryAllocateInfo.allocationSize = vk_memoryRequirements.size;
		vk_memoryAllocateInfo.memoryTypeIndex = KID::Vulkan::GetCorrectMemoryType(
			device->GetPhysicalDevice()->Vk_GetPhysicalDeviceMemoryProperties(),
			vk_memoryRequirements.memoryTypeBits,
			vk_memoryPropertyFlags
		);
	}

	vk_deviceMemory = AllocateMemory(device->Vk_GetDevice(), &vk_memoryAllocateInfo, nullptr);

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::DeviceMemory::DeviceMemory(Buffer* buffer, VkMemoryPropertyFlags vk_memoryPropertyFlags):
	DeviceDependent(buffer->GetDevice())
{
	__KID_VULKAN_LOG_BEGIN__(DeviceMemory());

	auto vk_memoryRequirements = GetBufferMemoryRequirements(device->Vk_GetDevice(), buffer->Vk_GetBuffer());

	VkMemoryAllocateInfo vk_memoryAllocateInfo;
	{
		vk_memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		vk_memoryAllocateInfo.pNext = nullptr;
		vk_memoryAllocateInfo.allocationSize = vk_memoryRequirements.size;
		vk_memoryAllocateInfo.memoryTypeIndex = KID::Vulkan::GetCorrectMemoryType(
			device->GetPhysicalDevice()->Vk_GetPhysicalDeviceMemoryProperties(),
			vk_memoryRequirements.memoryTypeBits,
			vk_memoryPropertyFlags
		);
	}

	vk_deviceMemory = AllocateMemory(device->Vk_GetDevice(), &vk_memoryAllocateInfo, nullptr);

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::DeviceMemory::~DeviceMemory()
{
	__KID_VULKAN_LOG_BEGIN__(DeviceMemory());

	__KID_DEPENDENCY_CHECK__;

	FreeMemory(device->Vk_GetDevice(), vk_deviceMemory, nullptr);

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region Queue
KID::Vulkan::Queue::Queue(Device* device_, uint32_t queueFamilyIndex_, uint32_t queueIndex_):
	DeviceDependent(device_),
	vk_queueFamilyIndex(queueFamilyIndex_),
	vk_queueIndex(queueIndex_)
{
	__KID_VULKAN_LOG_BEGIN__(Queue());

	vk_queue = KID::Vulkan::GetDeviceQueue(device->Vk_GetDevice(), vk_queueFamilyIndex, vk_queueIndex);

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::Queue::~Queue()
{
	__KID_VULKAN_LOG_BEGIN__(~Queue());

	__KID_DEPENDENCY_CHECK__;

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region Image
KID::Vulkan::Image::Image(Device* device_, Size size_, Type type_, Format format_, Tiling tiling_, UsageBits usage_, Layout layout_):
	DeviceDependent(device_),
	DeviceMemoryUser(),
	size(size_),
	type(type_),
	format(format_),
	usage(usage_)
{
	__KID_VULKAN_LOG_BEGIN__(Image());

	VkImageCreateInfo vk_imageCreateInfo;
	{
		vk_imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		vk_imageCreateInfo.pNext = nullptr;
		vk_imageCreateInfo.flags = 0;
		vk_imageCreateInfo.imageType = type;
		vk_imageCreateInfo.format = format;
		vk_imageCreateInfo.extent = {size.width, size.height, size.depth};
		vk_imageCreateInfo.mipLevels = 1;
		vk_imageCreateInfo.arrayLayers = 1;
		vk_imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		vk_imageCreateInfo.tiling = tiling_;// VK_IMAGE_TILING_OPTIMAL;
		vk_imageCreateInfo.usage = usage;	// VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		vk_imageCreateInfo.sharingMode = VkSharingMode::VK_SHARING_MODE_EXCLUSIVE;
		vk_imageCreateInfo.queueFamilyIndexCount = 0;
		vk_imageCreateInfo.pQueueFamilyIndices = nullptr;
		vk_imageCreateInfo.initialLayout = layout_;// VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;// VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	}

	vk_image = KID::Vulkan::CreateImage(device->Vk_GetDevice(), &vk_imageCreateInfo, nullptr);
	
	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::Image::Image(Device* device_, VkImage vk_image_, Size size_, Type type_, Format format_, Usage usage_):
	DeviceDependent(device_),
	DeviceMemoryUser(),
	vk_image(vk_image_),
	size(size_),
	type(type_),
	format(format_),
	usage(usage_)
{
	__KID_VULKAN_LOG_BEGIN__(Image());
	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::Image::~Image()
{
	__KID_VULKAN_LOG_BEGIN__(~Image());

	__KID_DEPENDENCY_CHECK__;

	//KID::Vulkan::DestroyImage(device->Vk_GetDevice(), vk_image, nullptr);
	if(vk_image != VK_NULL_HANDLE)
	{
		KID::Vulkan::DestroyImage(device->Vk_GetDevice(), vk_image, nullptr);
	}

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region SwapchainImage
KID::Vulkan::SwapchainImage::SwapchainImage(Swapchain* swapchain_, VkImage vk_image_, Size size_, Type type_, Format format_, Usage usage_):
	SwapchainDependent(swapchain_),
	Image(swapchain_->GetDevice(), vk_image_, size_, type_, format_, usage_)
{
	__KID_VULKAN_LOG_BEGIN__(SwapchainImage());
	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::SwapchainImage::~SwapchainImage()
{
	__KID_VULKAN_LOG_BEGIN__(~SwapchainImage());

	vk_image = VK_NULL_HANDLE;

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region ImageView
KID::Vulkan::ImageView::ImageView(Image* image_, Type type_, Aspect aspect_, Components components_):
	ImageDependent(image_),
	vk_imageViewType(type_),
	vk_imageAspect(aspect_)
{
	__KID_VULKAN_LOG_BEGIN__(ImageView());

	VkImageViewCreateInfo vk_imageViewCreateInfo;
	{
		vk_imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		vk_imageViewCreateInfo.pNext = nullptr;
		vk_imageViewCreateInfo.flags = 0;
		vk_imageViewCreateInfo.image = image->Vk_GetImage();
		vk_imageViewCreateInfo.viewType = vk_imageViewType;
		vk_imageViewCreateInfo.format = image->Vk_GetFormat();
		vk_imageViewCreateInfo.components = components_;		
		vk_imageViewCreateInfo.subresourceRange;
		{
			vk_imageViewCreateInfo.subresourceRange.aspectMask = vk_imageAspect;
			vk_imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
			vk_imageViewCreateInfo.subresourceRange.levelCount = 1;
			vk_imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
			vk_imageViewCreateInfo.subresourceRange.layerCount = 1;
		}
	}

	vk_imageView = KID::Vulkan::CreateImageView(image->GetDevice()->Vk_GetDevice(), &vk_imageViewCreateInfo, nullptr);

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::ImageView::~ImageView()
{
	__KID_VULKAN_LOG_BEGIN__(~ImageView());

	__KID_DEPENDENCY_CHECK__;

	KID::Vulkan::DestroyImageView(image->GetDevice()->Vk_GetDevice(), vk_imageView, nullptr);

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region Sampler
KID::Vulkan::Sampler::Sampler(Device* device_,
	Filter vk_minFilter_, Filter vk_magFilter_,
	MipmapMode vk_mipmapMode_,
	AddressMode vk_addressModeU_, AddressMode vk_addressModeV_, AddressMode vk_addressModeW_
):
	DeviceDependent(device_),
	vk_magFilter(vk_magFilter_),
	vk_minFilter(vk_minFilter_),
	vk_mipmapMode(vk_mipmapMode_),
	vk_addressModeU(vk_addressModeU_),
	vk_addressModeV(vk_addressModeV_),
	vk_addressModeW(vk_addressModeW_)
{
	__KID_VULKAN_LOG_BEGIN__(Sampler());

	VkSamplerCreateInfo vk_samplerCreateInfo;
	{
		vk_samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		vk_samplerCreateInfo.pNext = nullptr;
		vk_samplerCreateInfo.flags = 0;
		vk_samplerCreateInfo.magFilter = vk_magFilter_;
		vk_samplerCreateInfo.minFilter = vk_minFilter_;
		vk_samplerCreateInfo.mipmapMode = vk_mipmapMode_;
		vk_samplerCreateInfo.addressModeU = vk_addressModeU_;
		vk_samplerCreateInfo.addressModeV = vk_addressModeV_;
		vk_samplerCreateInfo.addressModeW = vk_addressModeW_;
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

	vk_sampler = CreateSampler(device->Vk_GetDevice(), &vk_samplerCreateInfo, nullptr);

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::Sampler::~Sampler()
{
	__KID_VULKAN_LOG_BEGIN__(~Sampler());

	__KID_DEPENDENCY_CHECK__;

	DestroySampler(device->Vk_GetDevice(), vk_sampler, nullptr);

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region RenderPass
KID::Vulkan::RenderPass::RenderPass(Device* device_, const std::vector<Format>& colorFormats_, const std::vector<Format>& depthFormats_):
	DeviceDependent(device_)
{
	__KID_VULKAN_LOG_BEGIN__(RenderPass());

	std::vector<VkAttachmentDescription> vk_subpassAttachmentDescriptions(2);
	{
		vk_subpassAttachmentDescriptions[0];
		{
			vk_subpassAttachmentDescriptions[0].flags = 0;	// VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;
			vk_subpassAttachmentDescriptions[0].format = colorFormats_[0];
			vk_subpassAttachmentDescriptions[0].samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
			vk_subpassAttachmentDescriptions[0].loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
			vk_subpassAttachmentDescriptions[0].storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_STORE;
			vk_subpassAttachmentDescriptions[0].stencilLoadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			vk_subpassAttachmentDescriptions[0].stencilStoreOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
			vk_subpassAttachmentDescriptions[0].initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED;// VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			vk_subpassAttachmentDescriptions[0].finalLayout = VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}
		vk_subpassAttachmentDescriptions[1];
		{
			vk_subpassAttachmentDescriptions[1].flags = 0;	// VK_ATTACHMENT_DESCRIPTION_MAY_ALIAS_BIT;
			vk_subpassAttachmentDescriptions[1].format = depthFormats_[0];
			vk_subpassAttachmentDescriptions[1].samples = VkSampleCountFlagBits::VK_SAMPLE_COUNT_1_BIT;
			vk_subpassAttachmentDescriptions[1].loadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_CLEAR;
			vk_subpassAttachmentDescriptions[1].storeOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
			vk_subpassAttachmentDescriptions[1].stencilLoadOp = VkAttachmentLoadOp::VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			vk_subpassAttachmentDescriptions[1].stencilStoreOp = VkAttachmentStoreOp::VK_ATTACHMENT_STORE_OP_DONT_CARE;
			vk_subpassAttachmentDescriptions[1].initialLayout = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			vk_subpassAttachmentDescriptions[1].finalLayout = VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
		}
	}

	std::vector<VkAttachmentReference> vk_colorAttachmentReferences(1);
	{
		vk_colorAttachmentReferences[0];
		{
			vk_colorAttachmentReferences[0].attachment = 0;
			vk_colorAttachmentReferences[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
		}
	}

	VkAttachmentReference vk_depthAttachmentReference;
	{
		vk_depthAttachmentReference.attachment = 1;
		vk_depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	}

	std::vector<VkSubpassDescription> vk_subpassDescriptions(1);
	{
		vk_subpassDescriptions[0];
		{
			vk_subpassDescriptions[0].flags = 0;
			vk_subpassDescriptions[0].pipelineBindPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;
			vk_subpassDescriptions[0].inputAttachmentCount = 0;
			vk_subpassDescriptions[0].pInputAttachments = nullptr;
			vk_subpassDescriptions[0].colorAttachmentCount = vk_colorAttachmentReferences.size();
			vk_subpassDescriptions[0].pColorAttachments = vk_colorAttachmentReferences.data();
			vk_subpassDescriptions[0].pResolveAttachments = nullptr;
			vk_subpassDescriptions[0].pDepthStencilAttachment = &vk_depthAttachmentReference;
			vk_subpassDescriptions[0].preserveAttachmentCount = 0;
			vk_subpassDescriptions[0].pPreserveAttachments = nullptr;
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

	vk_renderPass = KID::Vulkan::CreateRenderPass(device->Vk_GetDevice(), &vk_renderPassCreateInfo, nullptr);

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::RenderPass::~RenderPass()
{
	__KID_VULKAN_LOG_BEGIN__(~RenderPass());

	__KID_DEPENDENCY_CHECK__;

	DestroyRenderPass(device->Vk_GetDevice(), vk_renderPass, nullptr);

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region Framebuffer
KID::Vulkan::Framebuffer::Framebuffer(RenderPass* renderPass_, const std::vector<ImageView*>& colorAttachments_, ImageView* depthAttachment_):
	RenderPassDependent(renderPass_)
{
	__KID_VULKAN_LOG_BEGIN__(Framebuffer());

	VkExtent2D size_;
	std::vector<VkImageView> vk_framebufferAttachments(colorAttachments_.size() + (depthAttachment_ ? 1 : 0));
	{
		if(colorAttachments_.size() > 0)
		{
			size_.width = colorAttachments_[0]->GetImage()->Vk_GetSize().width;
			size_.height = colorAttachments_[0]->GetImage()->Vk_GetSize().height;
		}
		else
		{
			size_.width = depthAttachment_->GetImage()->Vk_GetSize().width;
			size_.height = depthAttachment_->GetImage()->Vk_GetSize().height;
		}

		size_t i = 0;
		while(i < colorAttachments_.size())
		{
			vk_framebufferAttachments[i] = colorAttachments_[i]->Vk_GetImageView();
			++i;
		}

		if(depthAttachment_)
		{
			vk_framebufferAttachments[i] = depthAttachment_->Vk_GetImageView();
			++i;
		}
	}

	VkFramebufferCreateInfo vk_framebufferCreateInfo;
	{
		vk_framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		vk_framebufferCreateInfo.pNext = nullptr;
		vk_framebufferCreateInfo.flags = 0;
		vk_framebufferCreateInfo.renderPass = renderPass->Vk_GetRenderPass();
		vk_framebufferCreateInfo.attachmentCount = vk_framebufferAttachments.size();
		vk_framebufferCreateInfo.pAttachments = vk_framebufferAttachments.data();
		vk_framebufferCreateInfo.width = size_.width;
		vk_framebufferCreateInfo.height = size_.height;
		vk_framebufferCreateInfo.layers = 1;
	}

	vk_framebuffer = KID::Vulkan::CreateFramebuffer(renderPass->GetDevice()->Vk_GetDevice(), &vk_framebufferCreateInfo, nullptr);

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::Framebuffer::~Framebuffer()
{
	__KID_VULKAN_LOG_BEGIN__(~Framebuffer());

	__KID_DEPENDENCY_CHECK__;

	KID::Vulkan::DestroyFramebuffer(renderPass->GetDevice()->Vk_GetDevice(), vk_framebuffer, nullptr);

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region CommandPool
KID::Vulkan::CommandPool::CommandPool(Device* device_, uint32_t vk_queueFamilyIndex_):
	DeviceDependent(device_),
	vk_queueFamilyIndex(vk_queueFamilyIndex_)
{
	__KID_VULKAN_LOG_BEGIN__(CommandPool());

	vk_commandPool = KID::Vulkan::CreateCommandPool(
		device->Vk_GetDevice(),
		VkCommandPoolCreateFlagBits::VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		vk_queueFamilyIndex,
		nullptr
	);

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::CommandPool::~CommandPool()
{
	__KID_VULKAN_LOG_BEGIN__(~CommandPool());

	__KID_DEPENDENCY_CHECK__;

	KID::Vulkan::DestroyCommandPool(device->Vk_GetDevice(), vk_commandPool, nullptr);

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region CommandBuffer
KID::Vulkan::CommandBuffer::CommandBuffer(CommandPool* commandPool_, Level commandBufferLevel_):
	CommandPoolDependent(commandPool_),
	commandBufferLevel(commandBufferLevel_)
{
	__KID_VULKAN_LOG_BEGIN__(CommandBuffer());

	vk_commandBuffer = std::move(AllocateCommandBuffers(
		commandPool->GetDevice()->Vk_GetDevice(),
		commandPool->Vk_GetCommandPool(),
		1,
		VkCommandBufferLevel::VK_COMMAND_BUFFER_LEVEL_PRIMARY)
	)[0];

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::CommandBuffer::~CommandBuffer()
{
	__KID_VULKAN_LOG_BEGIN__(~CommandBuffer());

	__KID_DEPENDENCY_CHECK__;

	FreeCommandBuffers(commandPool->GetDevice()->Vk_GetDevice(), commandPool->Vk_GetCommandPool(), std::vector<VkCommandBuffer>({vk_commandBuffer}));

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region DescriptorPool
KID::Vulkan::DescriptorPool::DescriptorPool(Device* device_, const std::vector<VkDescriptorPoolSize>& descriptorPoolSizes):
	DeviceDependent(device_)
{
	__KID_VULKAN_LOG_BEGIN__(DescriptorPool());

	VkDescriptorPoolCreateInfo vk_descriptorPoolCreateInfo;
	{
		vk_descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		vk_descriptorPoolCreateInfo.pNext = nullptr;
		vk_descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		vk_descriptorPoolCreateInfo.maxSets = 1;
		vk_descriptorPoolCreateInfo.poolSizeCount = descriptorPoolSizes.size();
		vk_descriptorPoolCreateInfo.pPoolSizes = descriptorPoolSizes.data();
	}
	vk_descriptorPool = CreateDescriptorPool(device->Vk_GetDevice(), &vk_descriptorPoolCreateInfo, nullptr);

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::DescriptorPool::~DescriptorPool()
{
	__KID_VULKAN_LOG_BEGIN__(~DescriptorPool());

	__KID_DEPENDENCY_CHECK__;

	DestroyDescriptorPool(device->Vk_GetDevice(), vk_descriptorPool, nullptr);

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region DescriptorSetLayout
KID::Vulkan::DescriptorSetLayout::DescriptorSetLayout(Device* device_, const std::vector<VkDescriptorSetLayoutBinding>& vk_descriptorSetLayoutBindings):
	DeviceDependent(device_)
{
	__KID_VULKAN_LOG_BEGIN__(DescriptorSetLayout());

	VkDescriptorSetLayoutCreateInfo vk_descriptorSetLayoutCreateInfo;
	{
		vk_descriptorSetLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		vk_descriptorSetLayoutCreateInfo.pNext = nullptr;
		vk_descriptorSetLayoutCreateInfo.flags = 0;
		vk_descriptorSetLayoutCreateInfo.bindingCount = vk_descriptorSetLayoutBindings.size();
		vk_descriptorSetLayoutCreateInfo.pBindings = vk_descriptorSetLayoutBindings.data();
	}
	vk_descriptorSetLayout = CreateDescriptorSetLayout(device->Vk_GetDevice(), &vk_descriptorSetLayoutCreateInfo, nullptr);

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::DescriptorSetLayout::~DescriptorSetLayout()
{
	__KID_VULKAN_LOG_BEGIN__(~DescriptorSetLayout());

	__KID_DEPENDENCY_CHECK__;

	DestroyDescriptorSetLayout(device->Vk_GetDevice(), vk_descriptorSetLayout, nullptr);

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region DescriptorSet
KID::Vulkan::DescriptorSet::DescriptorSet(DescriptorPool* descriptorPool_, DescriptorSetLayout* descriptorSetLayout_):
	DescriptorPoolDependent(descriptorPool_),
	DescriptorSetLayoutDependent(descriptorSetLayout_)
{
	__KID_VULKAN_LOG_BEGIN__(DescriptorSet());

	vk_descriptorSet = AllocateDescriptorSets(
		descriptorPool->GetDevice()->Vk_GetDevice(),
		descriptorPool->Vk_GetDescriptorPool(),
		1,
		&descriptorSetLayout->Vk_GetDescriptorSetLayout()
	)[0];

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::DescriptorSet::~DescriptorSet()
{
	__KID_VULKAN_LOG_BEGIN__(~DescriptorSet());

	__KID_DEPENDENCY_CHECK__;

	FreeDescriptorSets(
		descriptorPool->GetDevice()->Vk_GetDevice(),
		descriptorPool->Vk_GetDescriptorPool(),
		{vk_descriptorSet}
	);

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region ShaderModule
KID::Vulkan::ShaderModule::ShaderModule(Device* device_, const SourceCode& sourceCode):
	DeviceDependent(device_)
{
	__KID_VULKAN_LOG_BEGIN__(ShaderModule());

	VkShaderModuleCreateInfo vk_shaderModuleCreateInfo;
	{
		vk_shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		vk_shaderModuleCreateInfo.pNext = nullptr;
		vk_shaderModuleCreateInfo.flags = 0;
		vk_shaderModuleCreateInfo.codeSize = sourceCode.size();
		vk_shaderModuleCreateInfo.pCode = sourceCode.data();
	}

	vk_shaderModule = KID::Vulkan::CreateShaderModule(device->Vk_GetDevice(), &vk_shaderModuleCreateInfo, nullptr);

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::ShaderModule::~ShaderModule()
{
	__KID_VULKAN_LOG_BEGIN__(~ShaderModule());

	__KID_DEPENDENCY_CHECK__;

	DestroyShaderModule(device->Vk_GetDevice(), vk_shaderModule, nullptr);

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region PipelineLayout
KID::Vulkan::PipelineLayout::PipelineLayout(Device* device_, const std::vector<DescriptorSetLayout*>& descriptorSetLayouts):
	DeviceDependent(device_)
{
	__KID_VULKAN_LOG_BEGIN__(DescriptorSetLayout());

	std::vector<VkDescriptorSetLayout> vk_descriptorSetLayouts(descriptorSetLayouts.size());
	for(size_t i = 0; i < vk_descriptorSetLayouts.size(); ++i)
	{
		vk_descriptorSetLayouts[i] = descriptorSetLayouts[i]->Vk_GetDescriptorSetLayout();
	}
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
	vk_pipelineLayout = CreatePipelineLayout(device->Vk_GetDevice(), &vk_pipelineLayoutCreateInfo, nullptr);

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::PipelineLayout::~PipelineLayout()
{
	__KID_VULKAN_LOG_BEGIN__(~DescriptorSetLayout());

	__KID_DEPENDENCY_CHECK__;

	DestroyPipelineLayout(device->Vk_GetDevice(), vk_pipelineLayout, nullptr);

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region PipelineCache
KID::Vulkan::PipelineCache::PipelineCache(Device* device_):
DeviceDependent(device_)
{
	__KID_VULKAN_LOG_BEGIN__(PipelineCache());

	VkPipelineCacheCreateInfo vk_pipelineCacheCreateInfo;
	{
		vk_pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
		vk_pipelineCacheCreateInfo.pNext = nullptr;
		vk_pipelineCacheCreateInfo.flags = 0;
		vk_pipelineCacheCreateInfo.initialDataSize = 0;
		vk_pipelineCacheCreateInfo.pInitialData = nullptr;
	}
	vk_pipelineCache = CreatePipelineCache(device->Vk_GetDevice(), &vk_pipelineCacheCreateInfo, nullptr);

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::PipelineCache::~PipelineCache()
{
	__KID_VULKAN_LOG_BEGIN__(~PipelineCache());

	__KID_DEPENDENCY_CHECK__;

	DestroyPipelineCache(device->Vk_GetDevice(), vk_pipelineCache, nullptr);

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region Pipeline
KID::Vulkan::Pipeline::Pipeline(
	Device* device_, RenderPass* renderPass_, PipelineLayout* pipelineLayout_, PipelineCache* pipelineCache_,
	const ShaderModules& shaderModules_,
	const VerticesBindings& verticesBindings, const VerticesAttributes& verticesAttributes,
	Topology topology, VkBool32 primitiveRestart,
	const Viewports& viewports_, const Scissors& scissors_,
	Fill fill, Cull cull, Front front
):
	DeviceDependent(device_), RenderPassDependent(renderPass_), PipelineLayoutDependent(pipelineLayout_), PipelineCacheDependent(pipelineCache_),
	vk_viewports(viewports_),
	vk_scissors(scissors_)
{
	__KID_VULKAN_LOG_BEGIN__(Pipeline());

	std::vector<VkPipelineShaderStageCreateInfo> vk_pipelineShaderStageCreateInfos(
		(shaderModules_.vertex.shader ? 1 : 0) +
		(shaderModules_.tessellationControl.shader ? 1 : 0) +
		(shaderModules_.tessellationEvaluation.shader ? 1 : 0) +
		(shaderModules_.geometry.shader ? 1 : 0) +
		(shaderModules_.fragment.shader ? 1 : 0)
	);
	{
		int i = 0;
		if(shaderModules_.vertex.shader)
		{
			vk_pipelineShaderStageCreateInfos[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vk_pipelineShaderStageCreateInfos[i].pNext = nullptr;
			vk_pipelineShaderStageCreateInfos[i].flags = 0;
			vk_pipelineShaderStageCreateInfos[i].stage = VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
			vk_pipelineShaderStageCreateInfos[i].module = shaderModules_.vertex.shader->Vk_GetShaderModule();
			vk_pipelineShaderStageCreateInfos[i].pName = "main";
			vk_pipelineShaderStageCreateInfos[i].pSpecializationInfo = nullptr;
			++i;
		}
		if(shaderModules_.tessellationControl.shader)
		{
			vk_pipelineShaderStageCreateInfos[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vk_pipelineShaderStageCreateInfos[i].pNext = nullptr;
			vk_pipelineShaderStageCreateInfos[i].flags = 0;
			vk_pipelineShaderStageCreateInfos[i].stage = VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
			vk_pipelineShaderStageCreateInfos[i].module = shaderModules_.tessellationControl.shader->Vk_GetShaderModule();
			vk_pipelineShaderStageCreateInfos[i].pName = "main";
			vk_pipelineShaderStageCreateInfos[i].pSpecializationInfo = nullptr;
			++i;
		}
		if(shaderModules_.tessellationEvaluation.shader)
		{
			vk_pipelineShaderStageCreateInfos[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vk_pipelineShaderStageCreateInfos[i].pNext = nullptr;
			vk_pipelineShaderStageCreateInfos[i].flags = 0;
			vk_pipelineShaderStageCreateInfos[i].stage = VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
			vk_pipelineShaderStageCreateInfos[i].module = shaderModules_.tessellationEvaluation.shader->Vk_GetShaderModule();
			vk_pipelineShaderStageCreateInfos[i].pName = "main";
			vk_pipelineShaderStageCreateInfos[i].pSpecializationInfo = nullptr;
			++i;
		}
		if(shaderModules_.geometry.shader)
		{
			vk_pipelineShaderStageCreateInfos[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vk_pipelineShaderStageCreateInfos[i].pNext = nullptr;
			vk_pipelineShaderStageCreateInfos[i].flags = 0;
			vk_pipelineShaderStageCreateInfos[i].stage = VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT;
			vk_pipelineShaderStageCreateInfos[i].module = shaderModules_.geometry.shader->Vk_GetShaderModule();
			vk_pipelineShaderStageCreateInfos[i].pName = "main";
			vk_pipelineShaderStageCreateInfos[i].pSpecializationInfo = nullptr;
			++i;
		}
		if(shaderModules_.fragment.shader)
		{
			vk_pipelineShaderStageCreateInfos[i].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			vk_pipelineShaderStageCreateInfos[i].pNext = nullptr;
			vk_pipelineShaderStageCreateInfos[i].flags = 0;
			vk_pipelineShaderStageCreateInfos[i].stage = VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
			vk_pipelineShaderStageCreateInfos[i].module = shaderModules_.fragment.shader->Vk_GetShaderModule();
			vk_pipelineShaderStageCreateInfos[i].pName = "main";
			vk_pipelineShaderStageCreateInfos[i].pSpecializationInfo = nullptr;
			++i;
		}
	}

	VkPipelineVertexInputStateCreateInfo vk_pipelineVertexInputStateCreateInfo;
	{
		vk_pipelineVertexInputStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vk_pipelineVertexInputStateCreateInfo.pNext = nullptr;
		vk_pipelineVertexInputStateCreateInfo.flags = 0;
		vk_pipelineVertexInputStateCreateInfo.vertexBindingDescriptionCount = verticesBindings.size();
		vk_pipelineVertexInputStateCreateInfo.pVertexBindingDescriptions = verticesBindings.data();
		vk_pipelineVertexInputStateCreateInfo.vertexAttributeDescriptionCount = verticesAttributes.size();
		vk_pipelineVertexInputStateCreateInfo.pVertexAttributeDescriptions = verticesAttributes.data();
	}

	VkPipelineInputAssemblyStateCreateInfo vk_pipelineInputAssemblyStateCreateInfo;
	{
		vk_pipelineInputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		vk_pipelineInputAssemblyStateCreateInfo.pNext = nullptr;
		vk_pipelineInputAssemblyStateCreateInfo.flags = 0;
		vk_pipelineInputAssemblyStateCreateInfo.topology = topology;
		vk_pipelineInputAssemblyStateCreateInfo.primitiveRestartEnable = primitiveRestart ? VK_TRUE : VK_FALSE;
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
		vk_pipelineRasterizationStateCreateInfo.polygonMode = fill;
		vk_pipelineRasterizationStateCreateInfo.cullMode = cull;
		vk_pipelineRasterizationStateCreateInfo.frontFace = front;
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
		vk_pipelineDepthStencilStateCreateInfo.depthTestEnable = VK_TRUE;
		vk_pipelineDepthStencilStateCreateInfo.depthWriteEnable = VK_TRUE;
		vk_pipelineDepthStencilStateCreateInfo.depthCompareOp = VkCompareOp::VK_COMPARE_OP_LESS_OR_EQUAL;
		vk_pipelineDepthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
		vk_pipelineDepthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;
		vk_pipelineDepthStencilStateCreateInfo.front;
		{
			vk_pipelineDepthStencilStateCreateInfo.front.failOp = VkStencilOp::VK_STENCIL_OP_KEEP;
			vk_pipelineDepthStencilStateCreateInfo.front.passOp = VkStencilOp::VK_STENCIL_OP_KEEP;
			vk_pipelineDepthStencilStateCreateInfo.front.depthFailOp = VkStencilOp::VK_STENCIL_OP_KEEP;
			vk_pipelineDepthStencilStateCreateInfo.front.compareOp = VkCompareOp::VK_COMPARE_OP_ALWAYS;
			vk_pipelineDepthStencilStateCreateInfo.front.compareMask = 0;
			vk_pipelineDepthStencilStateCreateInfo.front.writeMask = 0;
			vk_pipelineDepthStencilStateCreateInfo.front.reference = 0;
		}
		vk_pipelineDepthStencilStateCreateInfo.back;
		{
			vk_pipelineDepthStencilStateCreateInfo.back.failOp = VkStencilOp::VK_STENCIL_OP_KEEP;
			vk_pipelineDepthStencilStateCreateInfo.back.passOp = VkStencilOp::VK_STENCIL_OP_KEEP;
			vk_pipelineDepthStencilStateCreateInfo.back.depthFailOp = VkStencilOp::VK_STENCIL_OP_KEEP;
			vk_pipelineDepthStencilStateCreateInfo.back.compareOp = VkCompareOp::VK_COMPARE_OP_ALWAYS;
			vk_pipelineDepthStencilStateCreateInfo.back.compareMask = 0;
			vk_pipelineDepthStencilStateCreateInfo.back.writeMask = 0;
			vk_pipelineDepthStencilStateCreateInfo.back.reference = 0;
		}
		vk_pipelineDepthStencilStateCreateInfo.minDepthBounds = 0.0f;
		vk_pipelineDepthStencilStateCreateInfo.maxDepthBounds = 0.0f;
	}

	/*std::vector<VkAttachmentReference> vk_colorAttachmentReferences(1);
	{
		vk_colorAttachmentReferences[0].attachment = 0;
		vk_colorAttachmentReferences[0].layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
	}
	VkAttachmentReference vk_depthAttachmentReference;
	{
		vk_depthAttachmentReference.attachment = 1;
		vk_depthAttachmentReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	}*/
	std::vector<VkPipelineColorBlendAttachmentState> vk_pipelineColorBlendAttachmentStates(1);// vk_colorAttachmentReferences.size());
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

	std::vector<VkDynamicState> vk_dynamicStates = {
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
	
	VkGraphicsPipelineCreateInfo vk_graphicsPipelineCreateInfo;
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
		vk_graphicsPipelineCreateInfo.layout = pipelineLayout->Vk_GetPipelineLayout();
		vk_graphicsPipelineCreateInfo.renderPass = renderPass->Vk_GetRenderPass();
		vk_graphicsPipelineCreateInfo.subpass = 0;
		vk_graphicsPipelineCreateInfo.basePipelineIndex = 0;
		vk_graphicsPipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
	}

	auto vk_pipelineCache = pipelineCache->Vk_GetPipelineCache();
	vk_pipeline = KID::Vulkan::CreateGraphicsPipelines(
		device->Vk_GetDevice(),
		vk_pipelineCache,
		std::vector<VkGraphicsPipelineCreateInfo>{vk_graphicsPipelineCreateInfo},
		nullptr
	)[0];

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::Pipeline::~Pipeline()
{
	__KID_VULKAN_LOG_BEGIN__(~Pipeline());

	__KID_DEPENDENCY_CHECK__;

	DestroyPipeline(device->Vk_GetDevice(), vk_pipeline, nullptr);

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region Buffer
KID::Vulkan::Buffer::Buffer(Device* device_, uint32_t size_, Usage usage_):
	DeviceDependent(device_),
	size(size_),
	vk_usage(usage_)
{
	__KID_VULKAN_LOG_BEGIN__(Buffer());

	VkBufferCreateInfo vk_bufferCreateInfo;
	{
		vk_bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		vk_bufferCreateInfo.pNext = nullptr;
		vk_bufferCreateInfo.flags = 0;
		vk_bufferCreateInfo.size = size;
		vk_bufferCreateInfo.usage = vk_usage;
		vk_bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		vk_bufferCreateInfo.queueFamilyIndexCount = 0;
		vk_bufferCreateInfo.pQueueFamilyIndices = nullptr;
	}

	vk_buffer = KID::Vulkan::CreateBuffer(device->Vk_GetDevice(), &vk_bufferCreateInfo, nullptr);

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::Buffer::~Buffer()
{
	__KID_VULKAN_LOG_BEGIN__(~Buffer());

	__KID_DEPENDENCY_CHECK__;

	DestroyBuffer(device->Vk_GetDevice(), vk_buffer, nullptr);

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region Fence
KID::Vulkan::Fence::Fence(Device* device_, bool on_):
	DeviceDependent(device_)
{
	__KID_VULKAN_LOG_BEGIN__(Fence());

	VkFenceCreateInfo vk_fenceCreateInfo;
	{
		vk_fenceCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		vk_fenceCreateInfo.pNext = nullptr;
		vk_fenceCreateInfo.flags = on_ ? VkFenceCreateFlagBits::VK_FENCE_CREATE_SIGNALED_BIT : 0;
	}
	
	vk_fence = CreateFence(device->Vk_GetDevice(), &vk_fenceCreateInfo, nullptr);

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::Fence::~Fence()
{
	__KID_VULKAN_LOG_BEGIN__(~Fence());

	__KID_DEPENDENCY_CHECK__;

	DestroyFence(device->Vk_GetDevice(), vk_fence, nullptr);

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma region Semaphore
KID::Vulkan::Semaphore::Semaphore(Device* device_):
	DeviceDependent(device_)
{
	__KID_VULKAN_LOG_BEGIN__(Semaphore());

	VkSemaphoreCreateInfo vk_semaphoreCreateInfo;
	{
		vk_semaphoreCreateInfo.sType = VkStructureType::VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		vk_semaphoreCreateInfo.pNext = nullptr;
		vk_semaphoreCreateInfo.flags = 0;
	}
	vk_semaphore = CreateSemaphore(device->Vk_GetDevice(), &vk_semaphoreCreateInfo, nullptr);

	__KID_VULKAN_LOG_END__;
}
KID::Vulkan::Semaphore::~Semaphore()
{
	__KID_VULKAN_LOG_BEGIN__(~Semaphore());

	__KID_DEPENDENCY_CHECK__;

	DestroySemaphore(device->Vk_GetDevice(), vk_semaphore, nullptr);

	__KID_VULKAN_LOG_END__;
}
#pragma endregion
#pragma endregion
#pragma region Func
void KID::Vulkan::ChangeImageLayout(Queue* queue, CommandPool* commandPool, Image* image, VkAccessFlags vk_srcAccessFlags, VkAccessFlags vk_dstAccessFlags, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags vk_imageAspectFlags)
{
	auto commandBuffer = new KID::Vulkan::CommandBuffer(commandPool, CommandBuffer::Level::VK_COMMAND_BUFFER_LEVEL_PRIMARY);
	{
		commandBuffer->Reset();
	}
	commandBuffer->Begin(nullptr, 0, nullptr, false);
	{
		VkImageMemoryBarrier vk_imageMemoryBarrier;
		{
			vk_imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			vk_imageMemoryBarrier.pNext = nullptr;
			vk_imageMemoryBarrier.srcAccessMask = vk_srcAccessFlags;
			vk_imageMemoryBarrier.dstAccessMask = vk_dstAccessFlags;	// VK_ACCESS_MEMORY_READ_BIT;
			vk_imageMemoryBarrier.oldLayout = oldLayout;
			vk_imageMemoryBarrier.newLayout = newLayout;
			vk_imageMemoryBarrier.srcQueueFamilyIndex = queue->Vk_GetQueueFamilyIndex();// 0;
			vk_imageMemoryBarrier.dstQueueFamilyIndex = queue->Vk_GetQueueFamilyIndex();// 0;
			vk_imageMemoryBarrier.image = image->Vk_GetImage();
			vk_imageMemoryBarrier.subresourceRange = {vk_imageAspectFlags, 0, 1, 0, 1};
		}

		vkCmdPipelineBarrier(
			commandBuffer->Vk_GetCommandBuffer(),
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
			0,
			0, nullptr,
			0, nullptr,
			1, &vk_imageMemoryBarrier
		);
	}
	commandBuffer->End();

	queue->Submit({commandBuffer}, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);

	queue->WaitIdle();

	delete commandBuffer;
}
#pragma endregion
#pragma region Log
size_t															KID::Vulkan::Log::tab = 0;
std::mutex														KID::Vulkan::Log::writeMutex;
std::clock_t													KID::Vulkan::Log::timestampClock;
#pragma endregion
#pragma endregion

#pragma region
#pragma endregion























