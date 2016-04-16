#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <KID/Header.hpp>
#include <KID/Geometry.hpp>

#include <Vulkan/Vulkan.hpp>
#pragma comment(lib, "Vulkan.lib")

#include <Math/Math.hpp>
#pragma comment(lib, "Math.lib")

#include <DevIL/DevIL.hpp>
#pragma comment(lib, "DevIL.lib")

//#define KID_PATHW(path) std::wstring(L"") + path
#define KID_PATHW(path) std::wstring(L"../../../") + path
//#define KID_PATHA(path) std::wstring(L"") + path
#define KID_PATHA(path) std::string("../../../") + path

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
	KID::Vulkan::Log::Write("[Vulkan][" + KID::string(pLayerPrefix) + "]" + KID::string(pMessage));
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
	KID::Vulkan::Log::Write("[WinAPI] Window created");

	auto instanceLayersProperties = new KID::Vulkan::InstanceLayersProperties();
	auto instanceExtensionsProperties = new KID::Vulkan::InstanceExtensionsProperties(*instanceLayersProperties);

	//auto layersName = instanceLayersProperties->layersName;
	KID::Vulkan::Device::LayersName instanceLayersName = {
#if _DEBUG
		"VK_LAYER_LUNARG_api_dump",
		"VK_LAYER_LUNARG_core_validation",
		"VK_LAYER_LUNARG_device_limits",
		"VK_LAYER_LUNARG_image",
		"VK_LAYER_LUNARG_object_tracker",
		"VK_LAYER_LUNARG_parameter_validation",
		"VK_LAYER_LUNARG_screenshot",
		"VK_LAYER_LUNARG_swapchain",
		"VK_LAYER_GOOGLE_threading",
		"VK_LAYER_GOOGLE_unique_objects",
		"VK_LAYER_LUNARG_vktrace",
		"VK_LAYER_RENDERDOC_Capture",
		"VK_LAYER_VALVE_steam_overlay",
		"VK_LAYER_LUNARG_standard_validation",
#endif
	};
	//auto extensionsName = instanceExtensionsProperties->extensionsName;
	KID::Vulkan::Device::ExtensionsName instanceExtensionsName = {
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
	};

	auto instance = new KID::Vulkan::Instance(instanceLayersName, instanceExtensionsName);
	{
		auto &physicalDevice = instance->GetPhysicalDevices()[0];

		auto deviceLayersProperties = new KID::Vulkan::DeviceLayersProperties(physicalDevice);
		auto deviceExtensionsProperties = new KID::Vulkan::DeviceExtensionsProperties(physicalDevice, deviceLayersProperties);

		//auto layersName = deviceLayersProperties->layersName;
		KID::Vulkan::Device::LayersName deviceLayersName = {
#if _DEBUG
			"VK_LAYER_LUNARG_api_dump",
			"VK_LAYER_LUNARG_core_validation",
			"VK_LAYER_LUNARG_device_limits",
			"VK_LAYER_LUNARG_image",
			"VK_LAYER_LUNARG_object_tracker",
			"VK_LAYER_LUNARG_parameter_validation",
			"VK_LAYER_LUNARG_screenshot",
			"VK_LAYER_LUNARG_swapchain",
			"VK_LAYER_GOOGLE_threading",
			"VK_LAYER_GOOGLE_unique_objects",
			"VK_LAYER_LUNARG_vktrace",
			"VK_LAYER_RENDERDOC_Capture",
			"VK_LAYER_VALVE_steam_overlay",
			"VK_LAYER_LUNARG_standard_validation",
#endif
		};
		//auto extensionsName = deviceExtensionsProperties->extensionsName;
		KID::Vulkan::Device::ExtensionsName deviceExtensionsName = {
			VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		};

		auto device = new KID::Vulkan::Device(physicalDevice, deviceLayersName, deviceExtensionsName);

		auto queue = new KID::Vulkan::Queue(device, 0, 0);
		auto commandPool = new KID::Vulkan::CommandPool(device, 0);
		{
			commandPool->Reset();
		}
		auto descriptorPool = new KID::Vulkan::DescriptorPool(device, {
			{VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2},
			{VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1}}
		);

		auto surface = new KID::Vulkan::Surface(physicalDevice, hInstance, hWnd);
		auto swapchain = new KID::Vulkan::Swapchain(
			device, surface, 0,
			surface->Vk_GetSurfaceFormats()[0].format, surface->Vk_GetSurfaceFormats()[0].colorSpace,
			{width, height}
		);

		auto depthImage = new KID::Vulkan::Image(
			device,
			{width, height, 1},
			KID::Vulkan::Image::Type::VK_IMAGE_TYPE_2D,
			KID::Vulkan::Image::Format::VK_FORMAT_D16_UNORM,
			KID::Vulkan::Image::Tiling::VK_IMAGE_TILING_OPTIMAL,
			KID::Vulkan::Image::UsageBits::VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
		{
			depthImage->SetDeviceMemory(new KID::Vulkan::DeviceMemory(depthImage, 0));
			KID::Vulkan::ChangeImageLayout(
				queue,
				commandPool,
				depthImage,
				0,
				//VkAccessFlagBits::VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,//VkAccessFlagBits::VK_ACCESS_MEMORY_READ_BIT | VkAccessFlagBits::VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VkAccessFlagBits::VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
				VkAccessFlagBits::VK_ACCESS_HOST_READ_BIT | VkAccessFlagBits::VK_ACCESS_HOST_READ_BIT |
				VkAccessFlagBits::VK_ACCESS_SHADER_READ_BIT | VkAccessFlagBits::VK_ACCESS_SHADER_WRITE_BIT |
				VkAccessFlagBits::VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VkAccessFlagBits::VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT |
				VkAccessFlagBits::VK_ACCESS_MEMORY_READ_BIT |  VkAccessFlagBits::VK_ACCESS_MEMORY_WRITE_BIT,
				VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
				VkImageLayout::VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
				VkImageAspectFlagBits::VK_IMAGE_ASPECT_DEPTH_BIT
			);
		}
		auto depthImageView = new KID::Vulkan::ImageView(
			depthImage,
			KID::Vulkan::ImageView::Type::VK_IMAGE_VIEW_TYPE_2D,
			KID::Vulkan::ImageView::AspectBits::VK_IMAGE_ASPECT_DEPTH_BIT,
			KID::Vulkan::ImageView::Components{
				KID::Vulkan::ImageView::Swizzle::VK_COMPONENT_SWIZZLE_IDENTITY,
				KID::Vulkan::ImageView::Swizzle::VK_COMPONENT_SWIZZLE_IDENTITY,
				KID::Vulkan::ImageView::Swizzle::VK_COMPONENT_SWIZZLE_IDENTITY,
				KID::Vulkan::ImageView::Swizzle::VK_COMPONENT_SWIZZLE_IDENTITY
			}
		);

		/*auto renderTexture = new KID::Vulkan::Image(
			device,
			{width, height, 1},
			KID::Vulkan::Image::Type::VK_IMAGE_TYPE_2D,
			KID::Vulkan::Image::Format::VK_FORMAT_R16G16B16A16_SFLOAT,
			KID::Vulkan::Image::Tiling::VK_IMAGE_TILING_OPTIMAL,
			KID::Vulkan::Image::UsageBits::VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
		);
		{
			renderTexture->SetDeviceMemory(new KID::Vulkan::DeviceMemory(renderTexture, 0));
			KID::Vulkan::ChangeImageLayout(
				queue,
				commandPool,
				renderTexture,
				0,
				//VkAccessFlagBits::VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,//VkAccessFlagBits::VK_ACCESS_MEMORY_READ_BIT | VkAccessFlagBits::VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VkAccessFlagBits::VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
				VkAccessFlagBits::VK_ACCESS_HOST_READ_BIT | VkAccessFlagBits::VK_ACCESS_HOST_READ_BIT |
				VkAccessFlagBits::VK_ACCESS_SHADER_READ_BIT | VkAccessFlagBits::VK_ACCESS_SHADER_WRITE_BIT |
				VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
				VkAccessFlagBits::VK_ACCESS_MEMORY_READ_BIT |  VkAccessFlagBits::VK_ACCESS_MEMORY_WRITE_BIT,
				VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
				VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT
			);
		}*/

		auto renderPass = new KID::Vulkan::RenderPass(device, {swapchain->GetImages()[0]->Vk_GetFormat()}, {depthImage->Vk_GetFormat()});

		auto vertexShaderSourceCode = std::move(loadFile(KID_PATHW(L"Media/Shaders/2.vert.spv")));
		auto fragmentShaderSourceCode = std::move(loadFile(KID_PATHW(L"Media/Shaders/2.frag.spv")));
		auto vertexShaderModule = new KID::Vulkan::ShaderModule(device, vertexShaderSourceCode);
		auto fragmentShaderModule = new KID::Vulkan::ShaderModule(device, fragmentShaderSourceCode);

		auto geometry = new KID::Geometry::Mesh; {
			geometry->CreateSphere(KID::Math::vec3f{1.0f, 1.0f, 1.0f}, KID::Math::vec2f{1.0f, 1.0f}, KID::Math::vec2u{128, 128});
		}

		auto vertexBuffer = new KID::Vulkan::Buffer(
			device,
			geometry->GetVertexSize(
				KID::Geometry::Mesh::VertexFormat::Float32,
				KID::Geometry::Mesh::VertexFormat::Float32,
				KID::Geometry::Mesh::VertexFormat::Float32,
				KID::Geometry::Mesh::VertexFormat::Float32,
				KID::Geometry::Mesh::VertexFormat::Float32
			) * geometry->GetVertexCount(),
			KID::Vulkan::Buffer::Usages::VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
		);
		{
			auto memory = new KID::Vulkan::DeviceMemory(vertexBuffer);
			{
				auto data = (float*)memory->Map(0, vertexBuffer->GetSize());

				auto vData = geometry->PackVertex(
					KID::Geometry::Mesh::VertexFormat::Float32,
					KID::Geometry::Mesh::VertexFormat::Float32,
					KID::Geometry::Mesh::VertexFormat::Float32,
					KID::Geometry::Mesh::VertexFormat::Float32,
					KID::Geometry::Mesh::VertexFormat::Float32
				);
				memcpy(data, vData, vertexBuffer->GetSize());

				/*std::vector<float> vArr = {
					-0.5f, +0.5f,
					+0.5f, +0.5f,
					-0.5f, -0.5f,
					+0.5f, -0.5f,
				};
				memcpy(data, vArr.data(), vertexBuffer->GetSize());*/
				memory->Unmap();
			}
			vertexBuffer->SetDeviceMemory(memory);
		}
		auto indexBuffer = new KID::Vulkan::Buffer(
			device,
			geometry->GetIndexSize(KID::Geometry::Mesh::IndexFormat::Uint32) * geometry->GetIndexCount(),//sizeof(uint32_t)*6,
			KID::Vulkan::Buffer::Usages::VK_BUFFER_USAGE_INDEX_BUFFER_BIT
		);
		{
			auto memory = new KID::Vulkan::DeviceMemory(indexBuffer);
			{
				auto data = (float*)memory->Map(0, indexBuffer->GetSize());

				auto iData = geometry->PackIndex(
					KID::Geometry::Mesh::IndexFormat::Uint32
				);
				memcpy(data, iData, indexBuffer->GetSize());

				/*std::vector<uint32_t> iArr = {
					0, 1, 2,
					1, 3, 2
				};
				memcpy(data, iArr.data(), indexBuffer->GetSize());*/
				memory->Unmap();
			}
			indexBuffer->SetDeviceMemory(memory);
		}
		auto uniformBuffer = new KID::Vulkan::Buffer(device, sizeof(float)*4, KID::Vulkan::Buffer::Usages::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
		{
			auto memory = new KID::Vulkan::DeviceMemory(uniformBuffer);
			{
				auto data = (float*)memory->Map(0, uniformBuffer->GetSize());
				std::vector<float> uniformData = {
					1.0f, 0.0f, 1.0f, 1.0f
				};
				memcpy(data, uniformData.data(), uniformBuffer->GetSize());
				memory->Unmap();
			}
			uniformBuffer->SetDeviceMemory(memory);
		}
		auto uniformBuffer2 = new KID::Vulkan::Buffer(device, sizeof(float)*16, KID::Vulkan::Buffer::Usages::VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
		{
			auto memory = new KID::Vulkan::DeviceMemory(uniformBuffer2);
			{
				auto data = (float*)memory->Map(0, uniformBuffer2->GetSize());
				auto tm = KID::Math::mat4f::model(KID::Math::vec3f{0.0f, 0.0f, 2.0f}, KID::Math::vec3f{0.0f, 0.0f, 0.0f}, KID::Math::vec3f{1.0f, 1.0f, 1.0f}) *
					KID::Math::mat4f::scale(KID::Math::vec3f{1.0f, 1.0f, -1.0f}) *
					KID::Math::mat4f::perspective(60.0f, 1.0f, 0.1f, 1000.0f);

				//memcpy(data, uniformData.data(), uniformBuffer2->GetSize());
				memcpy(data, tm.value, sizeof(float)*16);
				memory->Unmap();
			}
			uniformBuffer2->SetDeviceMemory(memory);
		}

		auto textureSourceInfo = KID::DevIL::Load(KID_PATHA("Media/Images/Brick1_D.png"));
		auto textureImage = new KID::Vulkan::Image(
			device,
			{textureSourceInfo.Width, textureSourceInfo.Height, textureSourceInfo.Depth}, //{512, 512, 1},
			KID::Vulkan::Image::Type::VK_IMAGE_TYPE_2D,
			KID::Vulkan::Image::Format::VK_FORMAT_R8G8B8A8_UNORM,
			KID::Vulkan::Image::Tiling::VK_IMAGE_TILING_LINEAR,
			KID::Vulkan::Image::UsageBits::VK_IMAGE_USAGE_SAMPLED_BIT,
			KID::Vulkan::Image::Layout::VK_IMAGE_LAYOUT_PREINITIALIZED
		);
		{
			auto memory = new KID::Vulkan::DeviceMemory(textureImage, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			{
				std::vector<uint32_t> data(textureImage->Vk_GetSize().width*textureImage->Vk_GetSize().height*textureImage->Vk_GetSize().depth, 0xFF00FFFF);
				for(size_t x = 0; x < textureSourceInfo.Width; ++x)
				for(size_t y = 0; y < textureSourceInfo.Height; ++y)
				{
					auto id = y * textureSourceInfo.Width + x;
					auto &color = data[id];

					auto source = textureSourceInfo.Data + id * 3;

					color = 0xFF000000 |
						(((KID::uint32)source[0]) << 0) | 
						(((KID::uint32)source[1]) << 8) | 
						(((KID::uint32)source[2]) << 16);

					//color = 0xFF000000 | (((KID::uint32)(x * 255 / 512)) << 8) | (((KID::uint32)(y * 255 / 512)) << 16);
				}
				auto p = memory->Map(0, data.size());
				memcpy(p, data.data(), data.size() * sizeof(uint32_t));
				memory->Unmap();
			}
			textureImage->SetDeviceMemory(memory);
		}
		auto textureImageView = new KID::Vulkan::ImageView(
			textureImage,
			KID::Vulkan::ImageView::Type::VK_IMAGE_VIEW_TYPE_2D,
			KID::Vulkan::ImageView::AspectBits::VK_IMAGE_ASPECT_COLOR_BIT,
			KID::Vulkan::ImageView::Components{
				KID::Vulkan::ImageView::Swizzle::VK_COMPONENT_SWIZZLE_R,
				KID::Vulkan::ImageView::Swizzle::VK_COMPONENT_SWIZZLE_G,
				KID::Vulkan::ImageView::Swizzle::VK_COMPONENT_SWIZZLE_B,
				KID::Vulkan::ImageView::Swizzle::VK_COMPONENT_SWIZZLE_A
			}
		);
		auto textureSampler = new KID::Vulkan::Sampler(
			device,
			KID::Vulkan::Sampler::Filter::VK_FILTER_LINEAR, KID::Vulkan::Sampler::Filter::VK_FILTER_LINEAR,
			KID::Vulkan::Sampler::MipmapMode::VK_SAMPLER_MIPMAP_MODE_NEAREST,
			KID::Vulkan::Sampler::AddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT, KID::Vulkan::Sampler::AddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT, KID::Vulkan::Sampler::AddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT
		);

		auto descriptorSetLayout = new KID::Vulkan::DescriptorSetLayout(device, {
			{0, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT, nullptr},
			{1, VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT, nullptr},
			{2, VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1, VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT, nullptr},
		});
		auto descriptorSet = new KID::Vulkan::DescriptorSet(descriptorPool, descriptorSetLayout);
		{
			std::vector<VkDescriptorBufferInfo> vk_descriptorBufferInfos(1);
			{
				vk_descriptorBufferInfos[0].buffer = uniformBuffer->Vk_GetBuffer();
				vk_descriptorBufferInfos[0].offset = 0;
				vk_descriptorBufferInfos[0].range = VK_WHOLE_SIZE;
			}
			std::vector<VkDescriptorBufferInfo> vk_descriptorBufferInfos2(1);
			{
				vk_descriptorBufferInfos2[0].buffer = uniformBuffer2->Vk_GetBuffer();
				vk_descriptorBufferInfos2[0].offset = 0;
				vk_descriptorBufferInfos2[0].range = VK_WHOLE_SIZE;
			}
			std::vector<VkDescriptorImageInfo> vk_descriptorImageInfos(1);
			{
				vk_descriptorImageInfos[0].sampler = textureSampler->Vk_GetSampler();
				vk_descriptorImageInfos[0].imageView = textureImageView->Vk_GetImageView();
				vk_descriptorImageInfos[0].imageLayout = VkImageLayout::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			}

			std::vector<VkWriteDescriptorSet> vk_writeDescriptorSet(3);
			{
				vk_writeDescriptorSet[0];
				{
					vk_writeDescriptorSet[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					vk_writeDescriptorSet[0].pNext = nullptr;
					vk_writeDescriptorSet[0].dstSet = descriptorSet->Vk_GetDescriptorSet();
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
					vk_writeDescriptorSet[1].dstSet = descriptorSet->Vk_GetDescriptorSet();
					vk_writeDescriptorSet[1].dstBinding = 1;
					vk_writeDescriptorSet[1].dstArrayElement = 0;
					vk_writeDescriptorSet[1].descriptorCount = vk_descriptorImageInfos.size();
					vk_writeDescriptorSet[1].descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
					vk_writeDescriptorSet[1].pImageInfo = vk_descriptorImageInfos.data();
					vk_writeDescriptorSet[1].pBufferInfo = nullptr;
					vk_writeDescriptorSet[1].pTexelBufferView = nullptr;
				}
				vk_writeDescriptorSet[2];
				{
					vk_writeDescriptorSet[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
					vk_writeDescriptorSet[2].pNext = nullptr;
					vk_writeDescriptorSet[2].dstSet = descriptorSet->Vk_GetDescriptorSet();
					vk_writeDescriptorSet[2].dstBinding = 2;
					vk_writeDescriptorSet[2].dstArrayElement = 0;
					vk_writeDescriptorSet[2].descriptorCount = vk_descriptorBufferInfos2.size();
					vk_writeDescriptorSet[2].descriptorType = VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
					vk_writeDescriptorSet[2].pImageInfo = nullptr;
					vk_writeDescriptorSet[2].pBufferInfo = vk_descriptorBufferInfos2.data();
					vk_writeDescriptorSet[2].pTexelBufferView = nullptr;
				}
			}
			vkUpdateDescriptorSets(device->Vk_GetDevice(), vk_writeDescriptorSet.size(), vk_writeDescriptorSet.data(), 0, nullptr);
		}
		auto pipelineLayout = new KID::Vulkan::PipelineLayout(device, {descriptorSetLayout});
		auto pipelineCache = new KID::Vulkan::PipelineCache(device);

		auto fence = new KID::Vulkan::Fence(device);

		std::vector<KID::Vulkan::ImageView*> swapchainImageViews(swapchain->GetImages().size());
		std::vector<KID::Vulkan::Framebuffer*> framebuffers(swapchain->GetImages().size());
		std::vector<KID::Vulkan::CommandBuffer*> commandBuffers(swapchain->GetImages().size());
		std::vector<KID::Vulkan::Pipeline*> pipelines(swapchain->GetImages().size());
		{
			for(size_t i = 0; i < framebuffers.size(); ++i)
			{
				auto &swapchainImage = swapchain->GetImages()[i];
				KID::Vulkan::ChangeImageLayout(
					queue,
					commandPool,
					swapchainImage,
					0,
					VkAccessFlagBits::VK_ACCESS_MEMORY_READ_BIT | VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
					VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED,
					VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
					VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT
				);

				auto &swapchainImageView = swapchainImageViews[i];
				swapchainImageView = new KID::Vulkan::ImageView(
					swapchainImage,
					KID::Vulkan::ImageView::Type::VK_IMAGE_VIEW_TYPE_2D,
					KID::Vulkan::ImageView::AspectBits::VK_IMAGE_ASPECT_COLOR_BIT,
					KID::Vulkan::ImageView::Components{
						KID::Vulkan::ImageView::Swizzle::VK_COMPONENT_SWIZZLE_R,
						KID::Vulkan::ImageView::Swizzle::VK_COMPONENT_SWIZZLE_G,
						KID::Vulkan::ImageView::Swizzle::VK_COMPONENT_SWIZZLE_B,
						KID::Vulkan::ImageView::Swizzle::VK_COMPONENT_SWIZZLE_A
					}
				);

				auto &framebuffer = framebuffers[i];
				framebuffer = new KID::Vulkan::Framebuffer(renderPass, {swapchainImageView}, depthImageView);

				auto pipelineShaderModules = KID::Vulkan::Pipeline::ShaderModules{
					{vertexShaderModule, {}},
					{nullptr, {}},
					{nullptr, {}},
					{nullptr, {}},
					{fragmentShaderModule, {{uniformBuffer, 0, 0, VK_WHOLE_SIZE}}}
				};
				auto pipelineVertexBindings = KID::Vulkan::Pipeline::VerticesBindings{
					{0, sizeof(float)*14, VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX},
					{1, sizeof(float)*14, VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX},
					{2, sizeof(float)* 14, VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX},
					{3, sizeof(float)* 14, VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX},
					{4, sizeof(float)* 14, VkVertexInputRate::VK_VERTEX_INPUT_RATE_VERTEX}
				};
				auto pipelineVertexAttributes = KID::Vulkan::Pipeline::VerticesAttributes{
					{0, 0, VkFormat::VK_FORMAT_R32G32B32_SFLOAT, 0},
					{1, 0, VkFormat::VK_FORMAT_R32G32_SFLOAT, sizeof(float)*3},
					{2, 0, VkFormat::VK_FORMAT_R32G32B32_SFLOAT, sizeof(float)* 5},
					{3, 0, VkFormat::VK_FORMAT_R32G32B32_SFLOAT, sizeof(float)* 8},
					{4, 0, VkFormat::VK_FORMAT_R32G32B32_SFLOAT, sizeof(float)* 11}
				};
				auto pipelineViewports = KID::Vulkan::Pipeline::Viewports{
					{0, 0, (float)width, (float)height}
				};
				auto pipelineScissors = KID::Vulkan::Pipeline::Scissors{
					{{0, 0}, {width, height}}
				};

				auto &pipeline = pipelines[i];
				pipeline = new KID::Vulkan::Pipeline(
					device, renderPass, pipelineLayout, pipelineCache,
					pipelineShaderModules,
					pipelineVertexBindings,
					pipelineVertexAttributes,
					KID::Vulkan::Pipeline::Topology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, false,
					pipelineViewports, pipelineScissors,
					KID::Vulkan::Pipeline::Fill::VK_POLYGON_MODE_FILL,
					KID::Vulkan::Pipeline::Culls::VK_CULL_MODE_FRONT_BIT, KID::Vulkan::Pipeline::Front::VK_FRONT_FACE_COUNTER_CLOCKWISE
				);

				auto &commandBuffer = commandBuffers[i];
				commandBuffer = new KID::Vulkan::CommandBuffer(commandPool, KID::Vulkan::CommandBuffer::Level::VK_COMMAND_BUFFER_LEVEL_PRIMARY);
				{
					commandBuffer->Reset();
					commandBuffer->Begin(nullptr, 0, framebuffer, false);
					{
						renderPass->Begin(commandBuffer,
							framebuffer,
							{{0, 0}, {width, height}},
							{KID::Vulkan::ClearColorf(0.32f, 0.32f, 0.32f, 1.0f),KID::Vulkan::ClearDepthStencil(1.0f, 0)},
							VkSubpassContents::VK_SUBPASS_CONTENTS_INLINE
						);
						{
							vkCmdBindDescriptorSets(
								commandBuffer->Vk_GetCommandBuffer(),
								VK_PIPELINE_BIND_POINT_GRAPHICS,
								pipelineLayout->Vk_GetPipelineLayout(),
								0,
								1,
								&descriptorSet->Vk_GetDescriptorSet(),
								0,
								nullptr
							);

							vkCmdBindPipeline(
								commandBuffer->Vk_GetCommandBuffer(),
								VK_PIPELINE_BIND_POINT_GRAPHICS,
								pipeline->Vk_GetPipeline()
							);

							vkCmdSetViewport(
								commandBuffer->Vk_GetCommandBuffer(),
								0,
								pipeline->Vk_GetViewports().size(),
								pipeline->Vk_GetViewports().data()
							);

							vkCmdSetScissor(
								commandBuffer->Vk_GetCommandBuffer(),
								0,
								pipeline->Vk_GetScissors().size(),
								pipeline->Vk_GetScissors().data()
							);

							std::vector<VkBuffer> vk_vertexBuffers = {
								vertexBuffer->Vk_GetBuffer(),
								vertexBuffer->Vk_GetBuffer(),
								vertexBuffer->Vk_GetBuffer(),
								vertexBuffer->Vk_GetBuffer(),
								vertexBuffer->Vk_GetBuffer()
							};
							std::vector<VkDeviceSize> vk_vertexOffsets = {0, 1, 2, 3, 4};
							vkCmdBindVertexBuffers(
								commandBuffer->Vk_GetCommandBuffer(),
								0,
								vk_vertexBuffers.size(),
								vk_vertexBuffers.data(),
								vk_vertexOffsets.data()
							);

							/*vkCmdDraw(
								commandBuffer->Vk_GetCommandBuffer(),
								3,
								1,
								0,
								0
							);*/

							vkCmdBindIndexBuffer(
								commandBuffer->Vk_GetCommandBuffer(),
								indexBuffer->Vk_GetBuffer(),
								0,
								VkIndexType::VK_INDEX_TYPE_UINT32
							);

							vkCmdDrawIndexed(
								commandBuffer->Vk_GetCommandBuffer(),
								geometry->GetIndexCount(),
								1,
								0,
								0,
								0
							);
						}
						renderPass->End(commandBuffer);
					}
					commandBuffer->End();
				}
			}
		}

		KID::Math::vec3f ang{0.0f, 0.0f, 0.0f};

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

			if(GetAsyncKeyState(VK_UP)) done = true;

			auto currentImage = swapchain->GetNextImage(nullptr, fence);

			queue->WaitIdle();
			device->WaitIdle();

			auto memory = uniformBuffer2->GetDeviceMemory();
			{
				ang.y += 5.0f;
				auto data = memory->Map(0, sizeof(KID::float32)*16);
				{
					auto tm = KID::Math::mat4f::model(KID::Math::vec3f{0.0f, 0.0f, 2.0f}, ang, KID::Math::vec3f{1.0f, 1.0f, 1.0f}) *
						KID::Math::mat4f::scale(KID::Math::vec3f{1.0f, 1.0f, -1.0f}) *
						KID::Math::mat4f::perspective(60.0f, 1.0f, 0.1f, 1000.0f);
					memcpy(data, tm.value, sizeof(KID::float32)* 16);
				}
				memory->Unmap();
			}

			KID::Vulkan::ChangeImageLayout(
				queue,
				commandPool,
				swapchain->GetImages()[currentImage],
				0,
				VkAccessFlagBits::VK_ACCESS_MEMORY_READ_BIT | VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
				VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED, //VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
				VkImageLayout::VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
				VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT
			);

			queue->Submit({commandBuffers[currentImage]}, VkPipelineStageFlagBits::VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT);

			queue->WaitIdle();
			device->WaitIdle();

			KID::Vulkan::ChangeImageLayout(
				queue,
				commandPool,
				swapchain->GetImages()[currentImage],
				0,
				VkAccessFlagBits::VK_ACCESS_MEMORY_READ_BIT | VkAccessFlagBits::VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
				VkImageLayout::VK_IMAGE_LAYOUT_UNDEFINED, //VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
				VkImageLayout::VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT
			);

			queue->Present({swapchain}, {currentImage});

			queue->WaitIdle();
			device->WaitIdle();
		}

		delete fence;

		for(auto &swapchainImageView : swapchainImageViews) delete swapchainImageView;
		for(auto &framebuffer : framebuffers) delete framebuffer;
		for(auto &pipeline : pipelines) delete pipeline;
		for(auto &commandBuffer : commandBuffers) delete commandBuffer;

		delete vertexShaderModule;
		delete fragmentShaderModule;

		delete vertexBuffer->GetDeviceMemory();
		delete vertexBuffer;
		delete indexBuffer->GetDeviceMemory();
		delete indexBuffer;
		delete uniformBuffer->GetDeviceMemory();
		delete uniformBuffer;
		delete uniformBuffer2->GetDeviceMemory();
		delete uniformBuffer2;

		delete textureImageView;
		delete textureImage->GetDeviceMemory();
		delete textureImage;
		delete textureSampler;

		delete descriptorSet;
		delete pipelineLayout;
		delete descriptorSetLayout;
		delete pipelineCache;

		delete renderPass;

		delete depthImageView;
		delete depthImage->GetDeviceMemory();
		delete depthImage;

		delete queue;
		delete commandPool;
		delete descriptorPool;

		delete swapchain;
		delete surface;

		delete device;
	}
	delete instance;
}

void main()
{
	KID::Vulkan::Log::Clear();

	//try
	{
		KID::Vulkan::Log::Write("Start vulkan shit...");
		vulkan();
		//vulkan2();
	}
	//catch(KID::Vulkan::Exception *e)
	//{
	//	KID::Vulkan::Log::Write("Some shit happen...");
	//	std::cout << "Some shit happen..." << std::endl;
	//}

	std::system("pause");
}