#pragma once
#include "wx/wxprec.h"
#include <wx/timer.h>
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <string>
#include <set>
#include <memory>
#include <chrono>
#include "glm.hpp"
#include <wx/clrpicker.h>

struct QueueFamilyIndices {
    int graphicsFamily = -1;
    int presentFamily = -1;

    bool IsComplete() {
        return graphicsFamily >= 0 && presentFamily >= 0;
    }
};

struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

class VulkanCanvas :
    public wxWindow
{
public:
    VulkanCanvas(wxWindow *pParent,
        wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = 0,
        const wxString& name = "VulkanCanvasName");

    virtual ~VulkanCanvas() noexcept;
	wxWindow *m_pParent;
private:
    void InitializeVulkan(std::vector<const char*> extensions);
    void CreateInstance(const VkInstanceCreateInfo& createInfo);
    void CreateWindowSurface();
    void PickPhysicalDevice();
    void CreateLogicalDevice();
    void CreateSwapChain(const wxSize& size);
    void CreateImageViews();
    void CreateRenderPass();
    void CreateGraphicsPipeline(const std::string& vertexShaderFile, const std::string& fragmentShaderFile);
    void CreateFrameBuffers();
    void CreateCommandPool();
    void CreateCommandBuffers();
    void CreateSemaphores();
    void RecreateSwapchain();
    VkWin32SurfaceCreateInfoKHR VulkanCanvas::CreateWin32SurfaceCreateInfo() const noexcept;
    VkDeviceQueueCreateInfo CreateDeviceQueueCreateInfo(int queueFamily) const noexcept;
    VkApplicationInfo CreateApplicationInfo(const std::string& appName,
        const int32_t appVersion = VK_MAKE_VERSION(1, 0, 0),
        const std::string& engineName = "YEngine",
        const int32_t engineVersion = VK_MAKE_VERSION(1, 0, 0),
        const int32_t apiVersion = VK_API_VERSION_1_0) const noexcept;
    VkInstanceCreateInfo CreateInstanceCreateInfo(const VkApplicationInfo& appInfo,
        const std::vector<const char*>& extensionNames,
        const std::vector<const char*>& layerNames) const noexcept;
    std::vector<VkDeviceQueueCreateInfo> VulkanCanvas::CreateQueueCreateInfos(
        const std::set<int>& uniqueQueueFamilies) const noexcept;
    VkDeviceCreateInfo CreateDeviceCreateInfo(
        const std::vector<VkDeviceQueueCreateInfo>& queueCreateInfos,
        const VkPhysicalDeviceFeatures& deviceFeatures) const noexcept;
    VkSwapchainCreateInfoKHR CreateSwapchainCreateInfo(
        const SwapChainSupportDetails& swapChainSupport,
        const VkSurfaceFormatKHR& surfaceFormat,
        uint32_t imageCount,
        const VkExtent2D& extent);
    VkImageViewCreateInfo CreateImageViewCreateInfo(uint32_t swapchainImage) const noexcept;
    VkAttachmentDescription CreateAttachmentDescription() const noexcept;
    VkAttachmentReference CreateAttachmentReference() const noexcept;
    VkSubpassDescription CreateSubpassDescription(const VkAttachmentReference& attachmentRef) const noexcept;
    VkSubpassDependency CreateSubpassDependency() const noexcept;
    VkRenderPassCreateInfo CreateRenderPassCreateInfo(
        const VkAttachmentDescription& colorAttachment,
        const VkSubpassDescription& subPass,
        const VkSubpassDependency& dependency) const noexcept;
    VkPipelineShaderStageCreateInfo CreatePipelineShaderStageCreateInfo(
        VkShaderStageFlagBits stage, VkShaderModule& module, const char* entryName) const noexcept;
    VkPipelineVertexInputStateCreateInfo CreatePipelineVertexInputStateCreateInfo() const noexcept;
    VkPipelineInputAssemblyStateCreateInfo CreatePipelineInputAssemblyStateCreateInfo(
        const VkPrimitiveTopology& topology, uint32_t restartEnable) const noexcept;
    VkViewport CreateViewport() const noexcept;
    VkRect2D CreateScissor() const noexcept;
    VkPipelineViewportStateCreateInfo CreatePipelineViewportStateCreateInfo(
        const VkViewport& viewport, const VkRect2D& scissor) const noexcept;
    VkPipelineRasterizationStateCreateInfo CreatePipelineRasterizationStateCreateInfo() const noexcept;
    VkPipelineMultisampleStateCreateInfo CreatePipelineMultisampleStateCreateInfo() const noexcept;
    VkPipelineColorBlendAttachmentState CreatePipelineColorBlendAttachmentState() const noexcept;
    VkPipelineColorBlendStateCreateInfo CreatePipelineColorBlendStateCreateInfo(
        const VkPipelineColorBlendAttachmentState& colorBlendAttachment) const noexcept;
    VkPipelineLayoutCreateInfo CreatePipelineLayoutCreateInfo() const noexcept;
    VkGraphicsPipelineCreateInfo CreateGraphicsPipelineCreateInfo(
        const VkPipelineShaderStageCreateInfo shaderStages[],
        const VkPipelineVertexInputStateCreateInfo& vertexInputInfo,
        const VkPipelineInputAssemblyStateCreateInfo& inputAssembly,
        const VkPipelineViewportStateCreateInfo& viewportState,
        const VkPipelineRasterizationStateCreateInfo& rasterizer,
        const VkPipelineMultisampleStateCreateInfo& multisampling,
        const VkPipelineColorBlendStateCreateInfo& colorBlending) const noexcept;
    VkShaderModuleCreateInfo CreateShaderModuleCreateInfo(
        const std::vector<char>& code) const noexcept;
    VkFramebufferCreateInfo CreateFramebufferCreateInfo(
        const VkImageView& attachments) const noexcept;
    VkCommandPoolCreateInfo CreateCommandPoolCreateInfo(QueueFamilyIndices& queueFamilyIndices) const noexcept;
    VkCommandBufferAllocateInfo CreateCommandBufferAllocateInfo() const noexcept;
    VkCommandBufferBeginInfo CreateCommandBufferBeginInfo() const noexcept;
    VkRenderPassBeginInfo CreateRenderPassBeginInfo(size_t swapchainBufferNumber) const noexcept;
    VkSemaphoreCreateInfo CreateSemaphoreCreateInfo() const noexcept;
	VkSubmitInfo CreateSubmitInfo(uint32_t imageIndex, VkPipelineStageFlags* pipelineStageFlags) const noexcept;
	VkPresentInfoKHR CreatePresentInfoKHR(uint32_t& imageIndex) const noexcept;
    bool IsDeviceSuitable(const VkPhysicalDevice& device) const;
    QueueFamilyIndices FindQueueFamilies(const VkPhysicalDevice& device) const;
    bool CheckDeviceExtensionSupport(const VkPhysicalDevice& device) const;
    SwapChainSupportDetails QuerySwapChainSupport(const VkPhysicalDevice& device) const;
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) const noexcept;
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) const noexcept;
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, const wxSize& size) const noexcept;
    static std::vector<char> ReadFile(const std::string& filename);
    void CreateShaderModule(const std::vector<char>& code, VkShaderModule& shaderModule) const;
    virtual void OnPaint(wxPaintEvent& event);
    virtual void OnResize(wxSizeEvent& event);
	virtual void onTimer(wxTimerEvent& event);
    void OnPaintException(const std::string& msg);

	void VulkanCanvas::CreateVertexBuffer(VkBuffer &, VkDeviceMemory &);

	void CreateBuffer(VkBuffer &, VkBufferUsageFlags, uint32_t size, VkMemoryPropertyFlags properties, VkDeviceMemory &deviceMemorie);
	void CreateUniformBuffer(VkBuffer &, uint32_t size, VkDeviceMemory &deviceMemorie);
	VkBufferCreateInfo CreateBufferCreateInfo(uint64_t, VkBufferUsageFlags);
	void AllocateMemory(VkDeviceMemory &deviceMemorie, VkBuffer &buffer, VkMemoryPropertyFlags properties);
	VkMemoryAllocateInfo CreateMemoryAllocateInfo(VkBuffer &buffer, VkMemoryPropertyFlags properties);
	void MapMemory(VkDeviceMemory, uint64_t, uint64_t, void **);
	VkDescriptorSetLayoutBinding CreateDescriptorSetLayoutBinding(uint32_t, VkDescriptorType, uint32_t, uint32_t);
	void CreateDescriptorSetLayout(uint32_t, uint32_t, VkDescriptorType, uint32_t, uint32_t);
	VkDescriptorSetLayoutCreateInfo CreateDescriptorSetLayoutInfo(uint32_t, uint32_t, VkDescriptorType, uint32_t, uint32_t);

	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

	VkDeviceMemory m_uniformMemorie, m_vertexMemory;
	std::vector<glm::vec2> m_vertices;
	VkBuffer m_uniformBuffer, m_vertexBuffer;
	VkDescriptorSet m_descriptorSet;
	VkDescriptorSetLayout m_descriptorSetLayout;
	VkDescriptorPoolSize m_descriptorPoolSize;

	VkVertexInputBindingDescription m_bindingDescription;
	std::vector<VkVertexInputAttributeDescription> m_attributeDescriptions;

	VkDescriptorSetLayoutBinding m_descriptorSetLayoutBinding;
	VkDescriptorPool m_descriptorPool;
	std::vector<VkPushConstantRange> m_ranges;
	VkDescriptorSetAllocateInfo m_descriptorSetAllocateInfo;
	VkDescriptorPoolCreateInfo descriptorPoolCreateInfo;

	VkDescriptorBufferInfo bufferInfo;
	VkWriteDescriptorSet descriptorWrite;

    VkInstance m_instance;
    VkSurfaceKHR m_surface;
    VkPhysicalDevice m_physicalDevice;
    VkDevice m_logicalDevice;
    VkQueue m_graphicsQueue;
    VkQueue m_presentQueue;
    VkSwapchainKHR m_swapchain;
    std::vector<VkImage> m_swapchainImages;
    VkFormat m_swapchainImageFormat;
    VkExtent2D m_swapchainExtent;
    std::vector<VkImageView> m_swapchainImageViews;
    VkRenderPass m_renderPass;
    VkPipelineLayout m_pipelineLayout;
    VkPipeline m_graphicsPipeline;
    std::vector<VkFramebuffer> m_swapchainFramebuffers;
    VkCommandPool m_commandPool;
    std::vector<VkCommandBuffer> m_commandBuffers;
    VkSemaphore m_imageAvailableSemaphore;
    VkSemaphore m_renderFinishedSemaphore;
    bool m_vulkanInitialized;
	std::unique_ptr<wxTimer> m_timer;
	typedef std::chrono::time_point<std::chrono::high_resolution_clock> sclock;
	sclock m_startTime;
	
};

