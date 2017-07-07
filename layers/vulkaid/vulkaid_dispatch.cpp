/*
** Copyright (c) 2015-2017 The Khronos Group Inc.
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
 *
 * Author: Tobin Ehlis <tobine@google.com>
 *
*/

#include "vulkaid_dispatch.h"
// Include vulkaid modules here
#include "vulkaid_state_tracker.h"
#include "vulkaid_state_view.h"
#include "vulkaid_state_controller.h"
#include <memory>

namespace vkaid {

// TODO : Where should these guys live?
static unique_ptr<StateController> state_controller;
static unique_ptr<StateView> state_view;

void PreCallCreateInstance(instance_layer_data* instance_data, const VkInstanceCreateInfo* pCreateInfo,
                           const VkAllocationCallbacks* pAllocator, VkInstance* pInstance) {}
void PostCallCreateInstance(instance_layer_data* instance_data, const VkInstanceCreateInfo* pCreateInfo,
                            const VkAllocationCallbacks* pAllocator, VkInstance* pInstance) {}

void PreCallDestroyInstance(instance_layer_data* instance_data, VkInstance instance, const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyInstance(instance_layer_data* instance_data, VkInstance instance, const VkAllocationCallbacks* pAllocator) {}

void PreCallEnumeratePhysicalDevices(instance_layer_data* instance_data, VkInstance instance, uint32_t* pPhysicalDeviceCount,
                                     VkPhysicalDevice* pPhysicalDevices) {}
void PostCallEnumeratePhysicalDevices(instance_layer_data* instance_data, VkInstance instance, uint32_t* pPhysicalDeviceCount,
                                      VkPhysicalDevice* pPhysicalDevices) {}

void PreCallGetPhysicalDeviceFeatures(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                      VkPhysicalDeviceFeatures* pFeatures) {}
void PostCallGetPhysicalDeviceFeatures(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                       VkPhysicalDeviceFeatures* pFeatures) {}

void PreCallGetPhysicalDeviceFormatProperties(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice, VkFormat format,
                                              VkFormatProperties* pFormatProperties) {}
void PostCallGetPhysicalDeviceFormatProperties(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice, VkFormat format,
                                               VkFormatProperties* pFormatProperties) {}

void PreCallGetPhysicalDeviceImageFormatProperties(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                   VkFormat format, VkImageType type, VkImageTiling tiling, VkImageUsageFlags usage,
                                                   VkImageCreateFlags flags, VkImageFormatProperties* pImageFormatProperties) {}
void PostCallGetPhysicalDeviceImageFormatProperties(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                    VkFormat format, VkImageType type, VkImageTiling tiling,
                                                    VkImageUsageFlags usage, VkImageCreateFlags flags,
                                                    VkImageFormatProperties* pImageFormatProperties) {}

void PreCallGetPhysicalDeviceProperties(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                        VkPhysicalDeviceProperties* pProperties) {}
void PostCallGetPhysicalDeviceProperties(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                         VkPhysicalDeviceProperties* pProperties) {}

void PreCallGetPhysicalDeviceQueueFamilyProperties(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                   uint32_t* pQueueFamilyPropertyCount,
                                                   VkQueueFamilyProperties* pQueueFamilyProperties) {}
void PostCallGetPhysicalDeviceQueueFamilyProperties(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                    uint32_t* pQueueFamilyPropertyCount,
                                                    VkQueueFamilyProperties* pQueueFamilyProperties) {}

void PreCallGetPhysicalDeviceMemoryProperties(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                              VkPhysicalDeviceMemoryProperties* pMemoryProperties) {}
void PostCallGetPhysicalDeviceMemoryProperties(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                               VkPhysicalDeviceMemoryProperties* pMemoryProperties) {}

void PreCallCreateDevice(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo,
                         const VkAllocationCallbacks* pAllocator, VkDevice* pDevice) {}
void PostCallCreateDevice(device_layer_data* device_data, const VkDeviceCreateInfo* pCreateInfo,
                          const VkAllocationCallbacks* pAllocator, VkDevice* pDevice) {
    // Initialize device state
    auto device_state = new device_state_struct();
    device_data->state_ptr = static_cast<void*>(device_state);
    // Initialize view and controller
    state_view = std::unique_ptr<StateView>(new StateView(device_state));
    state_controller = std::unique_ptr<StateController>(new StateController(device_state, state_view.get()));
}

void PreCallDestroyDevice(device_layer_data* device_data, VkDevice device, const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyDevice(device_layer_data* device_data, const VkAllocationCallbacks* pAllocator) {
    delete static_cast<device_state_struct*>(device_data->state_ptr);
}

void PreCallCmdBindPipeline(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint,
                            VkPipeline pipeline) {}
void PostCallCmdBindPipeline(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint,
                             VkPipeline pipeline) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_BIND_PIPELINE);
}
void PreCallCmdSetViewport(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t firstViewport,
                           uint32_t viewportCount, const VkViewport* pViewports) {}
void PostCallCmdSetViewport(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t firstViewport,
                            uint32_t viewportCount, const VkViewport* pViewports) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_VIEWPORT);
}
void PreCallCmdSetScissor(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t firstScissor,
                          uint32_t scissorCount, const VkRect2D* pScissors) {}
void PostCallCmdSetScissor(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t firstScissor,
                           uint32_t scissorCount, const VkRect2D* pScissors) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_SCISSOR);
}
void PreCallCmdSetLineWidth(device_layer_data* device_data, VkCommandBuffer commandBuffer, float lineWidth) {}
void PostCallCmdSetLineWidth(device_layer_data* device_data, VkCommandBuffer commandBuffer, float lineWidth) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_LINE_WIDTH);
}
void PreCallCmdSetDepthBias(device_layer_data* device_data, VkCommandBuffer commandBuffer, float depthBiasConstantFactor,
                            float depthBiasClamp, float depthBiasSlopeFactor) {}
void PostCallCmdSetDepthBias(device_layer_data* device_data, VkCommandBuffer commandBuffer, float depthBiasConstantFactor,
                             float depthBiasClamp, float depthBiasSlopeFactor) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_DEPTH_BIAS);
}
void PreCallCmdSetBlendConstants(device_layer_data* device_data, VkCommandBuffer commandBuffer, const float blendConstants[4]) {}
void PostCallCmdSetBlendConstants(device_layer_data* device_data, VkCommandBuffer commandBuffer, const float blendConstants[4]) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_BLEND_CONSTANTS);
}
void PreCallCmdSetDepthBounds(device_layer_data* device_data, VkCommandBuffer commandBuffer, float minDepthBounds,
                              float maxDepthBounds) {}
void PostCallCmdSetDepthBounds(device_layer_data* device_data, VkCommandBuffer commandBuffer, float minDepthBounds,
                               float maxDepthBounds) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_DEPTH_BOUNDS);
}
void PreCallCmdSetStencilCompareMask(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask,
                                     uint32_t compareMask) {}
void PostCallCmdSetStencilCompareMask(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask,
                                      uint32_t compareMask) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_STENCIL_COMPARE_MASK);
}
void PreCallCmdSetStencilWriteMask(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask,
                                   uint32_t writeMask) {}
void PostCallCmdSetStencilWriteMask(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask,
                                    uint32_t writeMask) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_STENCIL_WRITE_MASK);
}
void PreCallCmdSetStencilReference(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask,
                                   uint32_t reference) {}
void PostCallCmdSetStencilReference(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask,
                                    uint32_t reference) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_STENCIL_REFERENCE);
}
void PreCallCmdBindDescriptorSets(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                  VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet,
                                  uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount,
                                  const uint32_t* pDynamicOffsets) {}
void PostCallCmdBindDescriptorSets(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                   VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet,
                                   uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount,
                                   const uint32_t* pDynamicOffsets) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_BIND_DESCRIPTOR_SETS);
}
void PreCallCmdBindIndexBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset,
                               VkIndexType indexType) {}
void PostCallCmdBindIndexBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset,
                                VkIndexType indexType) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_BIND_INDEX_BUFFER);
}
void PreCallCmdBindVertexBuffers(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t firstBinding,
                                 uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) {}
void PostCallCmdBindVertexBuffers(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t firstBinding,
                                  uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_BIND_VERTEX_BUFFERS);
}
void PreCallCmdDraw(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount,
                    uint32_t firstVertex, uint32_t firstInstance) {}
void PostCallCmdDraw(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount,
                     uint32_t firstVertex, uint32_t firstInstance) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DRAW);
}
void PreCallCmdDrawIndexed(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t indexCount,
                           uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {}
void PostCallCmdDrawIndexed(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t indexCount,
                            uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DRAW_INDEXED);
}
void PreCallCmdDrawIndirect(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset,
                            uint32_t drawCount, uint32_t stride) {}
void PostCallCmdDrawIndirect(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset,
                             uint32_t drawCount, uint32_t stride) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DRAW_INDIRECT);
}
void PreCallCmdDrawIndexedIndirect(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer,
                                   VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {}
void PostCallCmdDrawIndexedIndirect(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer,
                                    VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DRAW_INDEXED_INDIRECT);
}
void PreCallCmdDispatch(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY,
                        uint32_t groupCountZ) {}
void PostCallCmdDispatch(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY,
                         uint32_t groupCountZ) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DISPATCH);
}
void PreCallCmdDispatchIndirect(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer,
                                VkDeviceSize offset) {}
void PostCallCmdDispatchIndirect(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer,
                                 VkDeviceSize offset) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DISPATCH_INDIRECT);
}
void PreCallCmdCopyBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer,
                          uint32_t regionCount, const VkBufferCopy* pRegions) {}
void PostCallCmdCopyBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer,
                           uint32_t regionCount, const VkBufferCopy* pRegions) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_COPY_BUFFER);
}
void PreCallCmdCopyImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage srcImage,
                         VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount,
                         const VkImageCopy* pRegions) {}
void PostCallCmdCopyImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage srcImage,
                          VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount,
                          const VkImageCopy* pRegions) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_COPY_IMAGE);
}
void PreCallCmdBlitImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage srcImage,
                         VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount,
                         const VkImageBlit* pRegions, VkFilter filter) {}
void PostCallCmdBlitImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage srcImage,
                          VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount,
                          const VkImageBlit* pRegions, VkFilter filter) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_BLIT_IMAGE);
}
void PreCallCmdCopyBufferToImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer srcBuffer,
                                 VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount,
                                 const VkBufferImageCopy* pRegions) {}
void PostCallCmdCopyBufferToImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer srcBuffer,
                                  VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount,
                                  const VkBufferImageCopy* pRegions) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_COPY_BUFFER_TO_IMAGE);
}
void PreCallCmdCopyImageToBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage srcImage,
                                 VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount,
                                 const VkBufferImageCopy* pRegions) {}
void PostCallCmdCopyImageToBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage srcImage,
                                  VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount,
                                  const VkBufferImageCopy* pRegions) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_COPY_IMAGE_TO_BUFFER);
}
void PreCallCmdUpdateBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer dstBuffer,
                            VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) {}
void PostCallCmdUpdateBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer dstBuffer,
                             VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_UPDATE_BUFFER);
}
void PreCallCmdFillBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer dstBuffer, VkDeviceSize dstOffset,
                          VkDeviceSize size, uint32_t data) {}
void PostCallCmdFillBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer dstBuffer,
                           VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_FILL_BUFFER);
}
void PreCallCmdClearColorImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage image,
                               VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount,
                               const VkImageSubresourceRange* pRanges) {}
void PostCallCmdClearColorImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage image,
                                VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount,
                                const VkImageSubresourceRange* pRanges) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_CLEAR_COLOR_IMAGE);
}
void PreCallCmdClearDepthStencilImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage image,
                                      VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil, uint32_t rangeCount,
                                      const VkImageSubresourceRange* pRanges) {}
void PostCallCmdClearDepthStencilImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage image,
                                       VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil,
                                       uint32_t rangeCount, const VkImageSubresourceRange* pRanges) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_CLEAR_DEPTH_STENCIL_IMAGE);
}
void PreCallCmdClearAttachments(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t attachmentCount,
                                const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) {}
void PostCallCmdClearAttachments(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t attachmentCount,
                                 const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_CLEAR_ATTACHMENTS);
}
void PreCallCmdResolveImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage srcImage,
                            VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount,
                            const VkImageResolve* pRegions) {}
void PostCallCmdResolveImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage srcImage,
                             VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount,
                             const VkImageResolve* pRegions) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_RESOLVE_IMAGE);
}
void PreCallCmdSetEvent(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkEvent event,
                        VkPipelineStageFlags stageMask) {}
void PostCallCmdSetEvent(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkEvent event,
                         VkPipelineStageFlags stageMask) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_EVENT);
}
void PreCallCmdResetEvent(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkEvent event,
                          VkPipelineStageFlags stageMask) {}
void PostCallCmdResetEvent(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkEvent event,
                           VkPipelineStageFlags stageMask) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_RESET_EVENT);
}
void PreCallCmdWaitEvents(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t eventCount,
                          const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
                          uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount,
                          const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount,
                          const VkImageMemoryBarrier* pImageMemoryBarriers) {}
void PostCallCmdWaitEvents(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t eventCount,
                           const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
                           uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount,
                           const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount,
                           const VkImageMemoryBarrier* pImageMemoryBarriers) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_WAIT_EVENTS);
}
void PreCallCmdPipelineBarrier(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask,
                               VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount,
                               const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount,
                               const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount,
                               const VkImageMemoryBarrier* pImageMemoryBarriers) {}
void PostCallCmdPipelineBarrier(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask,
                                VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount,
                                const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount,
                                const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount,
                                const VkImageMemoryBarrier* pImageMemoryBarriers) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_PIPELINE_BARRIER);
}
void PreCallCmdBeginQuery(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query,
                          VkQueryControlFlags flags) {}
void PostCallCmdBeginQuery(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query,
                           VkQueryControlFlags flags) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_BEGIN_QUERY);
}
void PreCallCmdEndQuery(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) {}
void PostCallCmdEndQuery(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_END_QUERY);
}
void PreCallCmdResetQueryPool(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkQueryPool queryPool,
                              uint32_t firstQuery, uint32_t queryCount) {}
void PostCallCmdResetQueryPool(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkQueryPool queryPool,
                               uint32_t firstQuery, uint32_t queryCount) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_RESET_QUERY_POOL);
}
void PreCallCmdWriteTimestamp(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage,
                              VkQueryPool queryPool, uint32_t query) {}
void PostCallCmdWriteTimestamp(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage,
                               VkQueryPool queryPool, uint32_t query) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_WRITE_TIMESTAMP);
}
void PreCallCmdCopyQueryPoolResults(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkQueryPool queryPool,
                                    uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset,
                                    VkDeviceSize stride, VkQueryResultFlags flags) {}
void PostCallCmdCopyQueryPoolResults(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkQueryPool queryPool,
                                     uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset,
                                     VkDeviceSize stride, VkQueryResultFlags flags) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_COPY_QUERY_POOL_RESULTS);
}
void PreCallCmdPushConstants(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkPipelineLayout layout,
                             VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) {}
void PostCallCmdPushConstants(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkPipelineLayout layout,
                              VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_PUSH_CONSTANTS);
}
void PreCallCmdBeginRenderPass(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                               const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) {}
void PostCallCmdBeginRenderPass(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_BEGIN_RENDER_PASS);
}
void PreCallCmdNextSubpass(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkSubpassContents contents) {}
void PostCallCmdNextSubpass(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkSubpassContents contents) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_NEXT_SUBPASS);
}
void PreCallCmdEndRenderPass(device_layer_data* device_data, VkCommandBuffer commandBuffer) {}
void PostCallCmdEndRenderPass(device_layer_data* device_data, VkCommandBuffer commandBuffer) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_END_RENDER_PASS);
}
void PreCallCmdExecuteCommands(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t commandBufferCount,
                               const VkCommandBuffer* pCommandBuffers) {}
void PostCallCmdExecuteCommands(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t commandBufferCount,
                                const VkCommandBuffer* pCommandBuffers) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_EXECUTE_COMMANDS);
}
void PreCallCmdPushDescriptorSetKHR(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                    VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set,
                                    uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) {}
void PostCallCmdPushDescriptorSetKHR(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                     VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set,
                                     uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_PUSH_DESCRIPTOR_SET_KHR);
}
void PreCallCmdPushDescriptorSetWithTemplateKHR(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                                VkDescriptorUpdateTemplateKHR descriptorUpdateTemplate, VkPipelineLayout layout,
                                                uint32_t set, const void* pData) {}
void PostCallCmdPushDescriptorSetWithTemplateKHR(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                                 VkDescriptorUpdateTemplateKHR descriptorUpdateTemplate, VkPipelineLayout layout,
                                                 uint32_t set, const void* pData) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_PUSH_DESCRIPTOR_SET_WITH_TEMPLATE_KHR);
}
void PreCallCmdDebugMarkerBeginEXT(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                   VkDebugMarkerMarkerInfoEXT* pMarkerInfo) {}
void PostCallCmdDebugMarkerBeginEXT(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                    VkDebugMarkerMarkerInfoEXT* pMarkerInfo) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DEBUG_MARKER_BEGIN_EXT);
}
void PreCallCmdDebugMarkerEndEXT(device_layer_data* device_data, VkCommandBuffer commandBuffer) {}
void PostCallCmdDebugMarkerEndEXT(device_layer_data* device_data, VkCommandBuffer commandBuffer) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DEBUG_MARKER_END_EXT);
}
void PreCallCmdDebugMarkerInsertEXT(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                    VkDebugMarkerMarkerInfoEXT* pMarkerInfo) {}
void PostCallCmdDebugMarkerInsertEXT(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                     VkDebugMarkerMarkerInfoEXT* pMarkerInfo) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DEBUG_MARKER_INSERT_EXT);
}
void PreCallCmdDrawIndirectCountAMD(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer,
                                    VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset,
                                    uint32_t maxDrawCount, uint32_t stride) {}
void PostCallCmdDrawIndirectCountAMD(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer,
                                     VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset,
                                     uint32_t maxDrawCount, uint32_t stride) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DRAW_INDIRECT_COUNT_AMD);
}
void PreCallCmdDrawIndexedIndirectCountAMD(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer,
                                           VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset,
                                           uint32_t maxDrawCount, uint32_t stride) {}
void PostCallCmdDrawIndexedIndirectCountAMD(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer,
                                            VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset,
                                            uint32_t maxDrawCount, uint32_t stride) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DRAW_INDEXED_INDIRECT_COUNT_AMD);
}
void PreCallCmdSetDeviceMaskKHX(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t deviceMask) {}
void PostCallCmdSetDeviceMaskKHX(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t deviceMask) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_DEVICE_MASK_KHX);
}
void PreCallCmdDispatchBaseKHX(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t baseGroupX,
                               uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY,
                               uint32_t groupCountZ) {}
void PostCallCmdDispatchBaseKHX(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t baseGroupX,
                                uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY,
                                uint32_t groupCountZ) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DISPATCH_BASE_KHX);
}
void PreCallCmdProcessCommandsNVX(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                  const VkCmdProcessCommandsInfoNVX* pProcessCommandsInfo) {}
void PostCallCmdProcessCommandsNVX(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                   const VkCmdProcessCommandsInfoNVX* pProcessCommandsInfo) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_PROCESS_COMMANDS_NVX);
}
void PreCallCmdReserveSpaceForCommandsNVX(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                          const VkCmdReserveSpaceForCommandsInfoNVX* pReserveSpaceInfo) {}
void PostCallCmdReserveSpaceForCommandsNVX(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                           const VkCmdReserveSpaceForCommandsInfoNVX* pReserveSpaceInfo) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_RESERVE_SPACE_FOR_COMMANDS_NVX);
}
void PreCallCmdSetViewportWScalingNV(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t firstViewport,
                                     uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) {}
void PostCallCmdSetViewportWScalingNV(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t firstViewport,
                                      uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_VIEWPORT_W_SCALING_NV);
}

void PreCallCmdSetDiscardRectangleEXT(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t firstDiscardRectangle,
                                      uint32_t discardRectangleCount, const VkRect2D* pDiscardRectangles) {}
void PostCallCmdSetDiscardRectangleEXT(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                       uint32_t firstDiscardRectangle, uint32_t discardRectangleCount,
                                       const VkRect2D* pDiscardRectangles) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_DISCARD_RECTANGLE_EXT);
}
void PreCallEnumerateInstanceExtensionProperties(device_layer_data* device_data, const char* pLayerName, uint32_t* pPropertyCount,
                                                 VkExtensionProperties* pProperties) {}
void PostCallEnumerateInstanceExtensionProperties(device_layer_data* device_data, const char* pLayerName, uint32_t* pPropertyCount,
                                                  VkExtensionProperties* pProperties) {}

void PreCallEnumerateDeviceExtensionProperties(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                               const char* pLayerName, uint32_t* pPropertyCount,
                                               VkExtensionProperties* pProperties) {}
void PostCallEnumerateDeviceExtensionProperties(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                const char* pLayerName, uint32_t* pPropertyCount,
                                                VkExtensionProperties* pProperties) {}

void PreCallEnumerateInstanceLayerProperties(device_layer_data* device_data, uint32_t* pPropertyCount,
                                             VkLayerProperties* pProperties) {}
void PostCallEnumerateInstanceLayerProperties(device_layer_data* device_data, uint32_t* pPropertyCount,
                                              VkLayerProperties* pProperties) {}

void PreCallEnumerateDeviceLayerProperties(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                           uint32_t* pPropertyCount, VkLayerProperties* pProperties) {}
void PostCallEnumerateDeviceLayerProperties(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                            uint32_t* pPropertyCount, VkLayerProperties* pProperties) {}

void PreCallGetDeviceQueue(device_layer_data* device_data, VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex,
                           VkQueue* pQueue) {}
void PostCallGetDeviceQueue(device_layer_data* device_data, VkDevice device, uint32_t queueFamilyIndex, uint32_t queueIndex,
                            VkQueue* pQueue) {}

void PreCallQueueSubmit(device_layer_data* device_data, VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits,
                        VkFence fence) {}
void PostCallQueueSubmit(device_layer_data* device_data, VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits,
                         VkFence fence) {}

void PreCallQueueWaitIdle(device_layer_data* device_data, VkQueue queue) {}
void PostCallQueueWaitIdle(device_layer_data* device_data, VkQueue queue) {}

void PreCallDeviceWaitIdle(device_layer_data* device_data, VkDevice device) {}
void PostCallDeviceWaitIdle(device_layer_data* device_data, VkDevice device) {}

void PreCallAllocateMemory(device_layer_data* device_data, VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo,
                           const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) {}
void PostCallAllocateMemory(device_layer_data* device_data, VkDevice device, const VkMemoryAllocateInfo* pAllocateInfo,
                            const VkAllocationCallbacks* pAllocator, VkDeviceMemory* pMemory) {}

void PreCallFreeMemory(device_layer_data* device_data, VkDevice device, VkDeviceMemory memory,
                       const VkAllocationCallbacks* pAllocator) {}
void PostCallFreeMemory(device_layer_data* device_data, VkDevice device, VkDeviceMemory memory,
                        const VkAllocationCallbacks* pAllocator) {}

void PreCallMapMemory(device_layer_data* device_data, VkDevice device, VkDeviceMemory memory, VkDeviceSize offset,
                      VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) {}
void PostCallMapMemory(device_layer_data* device_data, VkDevice device, VkDeviceMemory memory, VkDeviceSize offset,
                       VkDeviceSize size, VkMemoryMapFlags flags, void** ppData) {}

void PreCallUnmapMemory(device_layer_data* device_data, VkDevice device, VkDeviceMemory memory) {}
void PostCallUnmapMemory(device_layer_data* device_data, VkDevice device, VkDeviceMemory memory) {}

void PreCallFlushMappedMemoryRanges(device_layer_data* device_data, VkDevice device, uint32_t memoryRangeCount,
                                    const VkMappedMemoryRange* pMemoryRanges) {}
void PostCallFlushMappedMemoryRanges(device_layer_data* device_data, VkDevice device, uint32_t memoryRangeCount,
                                     const VkMappedMemoryRange* pMemoryRanges) {}

void PreCallInvalidateMappedMemoryRanges(device_layer_data* device_data, VkDevice device, uint32_t memoryRangeCount,
                                         const VkMappedMemoryRange* pMemoryRanges) {}
void PostCallInvalidateMappedMemoryRanges(device_layer_data* device_data, VkDevice device, uint32_t memoryRangeCount,
                                          const VkMappedMemoryRange* pMemoryRanges) {}

void PreCallGetDeviceMemoryCommitment(device_layer_data* device_data, VkDevice device, VkDeviceMemory memory,
                                      VkDeviceSize* pCommittedMemoryInBytes) {}
void PostCallGetDeviceMemoryCommitment(device_layer_data* device_data, VkDevice device, VkDeviceMemory memory,
                                       VkDeviceSize* pCommittedMemoryInBytes) {}

void PreCallBindBufferMemory(device_layer_data* device_data, VkDevice device, VkBuffer buffer, VkDeviceMemory memory,
                             VkDeviceSize memoryOffset) {}
void PostCallBindBufferMemory(device_layer_data* device_data, VkDevice device, VkBuffer buffer, VkDeviceMemory memory,
                              VkDeviceSize memoryOffset) {}

void PreCallBindImageMemory(device_layer_data* device_data, VkDevice device, VkImage image, VkDeviceMemory memory,
                            VkDeviceSize memoryOffset) {}
void PostCallBindImageMemory(device_layer_data* device_data, VkDevice device, VkImage image, VkDeviceMemory memory,
                             VkDeviceSize memoryOffset) {}

void PreCallGetBufferMemoryRequirements(device_layer_data* device_data, VkDevice device, VkBuffer buffer,
                                        VkMemoryRequirements* pMemoryRequirements) {}
void PostCallGetBufferMemoryRequirements(device_layer_data* device_data, VkDevice device, VkBuffer buffer,
                                         VkMemoryRequirements* pMemoryRequirements) {}

void PreCallGetImageMemoryRequirements(device_layer_data* device_data, VkDevice device, VkImage image,
                                       VkMemoryRequirements* pMemoryRequirements) {}
void PostCallGetImageMemoryRequirements(device_layer_data* device_data, VkDevice device, VkImage image,
                                        VkMemoryRequirements* pMemoryRequirements) {}

void PreCallGetImageSparseMemoryRequirements(device_layer_data* device_data, VkDevice device, VkImage image,
                                             uint32_t* pSparseMemoryRequirementCount,
                                             VkSparseImageMemoryRequirements* pSparseMemoryRequirements) {}
void PostCallGetImageSparseMemoryRequirements(device_layer_data* device_data, VkDevice device, VkImage image,
                                              uint32_t* pSparseMemoryRequirementCount,
                                              VkSparseImageMemoryRequirements* pSparseMemoryRequirements) {}

void PreCallGetPhysicalDeviceSparseImageFormatProperties(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                         VkFormat format, VkImageType type, VkSampleCountFlagBits samples,
                                                         VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount,
                                                         VkSparseImageFormatProperties* pProperties) {}
void PostCallGetPhysicalDeviceSparseImageFormatProperties(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                          VkFormat format, VkImageType type, VkSampleCountFlagBits samples,
                                                          VkImageUsageFlags usage, VkImageTiling tiling, uint32_t* pPropertyCount,
                                                          VkSparseImageFormatProperties* pProperties) {}

void PreCallQueueBindSparse(device_layer_data* device_data, VkQueue queue, uint32_t bindInfoCount,
                            const VkBindSparseInfo* pBindInfo, VkFence fence) {}
void PostCallQueueBindSparse(device_layer_data* device_data, VkQueue queue, uint32_t bindInfoCount,
                             const VkBindSparseInfo* pBindInfo, VkFence fence) {}

void PreCallCreateFence(device_layer_data* device_data, VkDevice device, const VkFenceCreateInfo* pCreateInfo,
                        const VkAllocationCallbacks* pAllocator, VkFence* pFence) {}
void PostCallCreateFence(device_layer_data* device_data, VkDevice device, const VkFenceCreateInfo* pCreateInfo,
                         const VkAllocationCallbacks* pAllocator, VkFence* pFence) {}

void PreCallDestroyFence(device_layer_data* device_data, VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyFence(device_layer_data* device_data, VkDevice device, VkFence fence, const VkAllocationCallbacks* pAllocator) {
}

void PreCallResetFences(device_layer_data* device_data, VkDevice device, uint32_t fenceCount, const VkFence* pFences) {}
void PostCallResetFences(device_layer_data* device_data, VkDevice device, uint32_t fenceCount, const VkFence* pFences) {}

void PreCallGetFenceStatus(device_layer_data* device_data, VkDevice device, VkFence fence) {}
void PostCallGetFenceStatus(device_layer_data* device_data, VkDevice device, VkFence fence) {}

void PreCallWaitForFences(device_layer_data* device_data, VkDevice device, uint32_t fenceCount, const VkFence* pFences,
                          VkBool32 waitAll, uint64_t timeout) {}
void PostCallWaitForFences(device_layer_data* device_data, VkDevice device, uint32_t fenceCount, const VkFence* pFences,
                           VkBool32 waitAll, uint64_t timeout) {}

void PreCallCreateSemaphore(device_layer_data* device_data, VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo,
                            const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) {}
void PostCallCreateSemaphore(device_layer_data* device_data, VkDevice device, const VkSemaphoreCreateInfo* pCreateInfo,
                             const VkAllocationCallbacks* pAllocator, VkSemaphore* pSemaphore) {}

void PreCallDestroySemaphore(device_layer_data* device_data, VkDevice device, VkSemaphore semaphore,
                             const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroySemaphore(device_layer_data* device_data, VkDevice device, VkSemaphore semaphore,
                              const VkAllocationCallbacks* pAllocator) {}

void PreCallCreateEvent(device_layer_data* device_data, VkDevice device, const VkEventCreateInfo* pCreateInfo,
                        const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) {}
void PostCallCreateEvent(device_layer_data* device_data, VkDevice device, const VkEventCreateInfo* pCreateInfo,
                         const VkAllocationCallbacks* pAllocator, VkEvent* pEvent) {}

void PreCallDestroyEvent(device_layer_data* device_data, VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyEvent(device_layer_data* device_data, VkDevice device, VkEvent event, const VkAllocationCallbacks* pAllocator) {
}

void PreCallGetEventStatus(device_layer_data* device_data, VkDevice device, VkEvent event) {}
void PostCallGetEventStatus(device_layer_data* device_data, VkDevice device, VkEvent event) {}

void PreCallSetEvent(device_layer_data* device_data, VkDevice device, VkEvent event) {}
void PostCallSetEvent(device_layer_data* device_data, VkDevice device, VkEvent event) {}

void PreCallResetEvent(device_layer_data* device_data, VkDevice device, VkEvent event) {}
void PostCallResetEvent(device_layer_data* device_data, VkDevice device, VkEvent event) {}

void PreCallCreateQueryPool(device_layer_data* device_data, VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo,
                            const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) {}
void PostCallCreateQueryPool(device_layer_data* device_data, VkDevice device, const VkQueryPoolCreateInfo* pCreateInfo,
                             const VkAllocationCallbacks* pAllocator, VkQueryPool* pQueryPool) {}

void PreCallDestroyQueryPool(device_layer_data* device_data, VkDevice device, VkQueryPool queryPool,
                             const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyQueryPool(device_layer_data* device_data, VkDevice device, VkQueryPool queryPool,
                              const VkAllocationCallbacks* pAllocator) {}

void PreCallGetQueryPoolResults(device_layer_data* device_data, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery,
                                uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) {}
void PostCallGetQueryPoolResults(device_layer_data* device_data, VkDevice device, VkQueryPool queryPool, uint32_t firstQuery,
                                 uint32_t queryCount, size_t dataSize, void* pData, VkDeviceSize stride, VkQueryResultFlags flags) {
}

void PreCallCreateBuffer(device_layer_data* device_data, VkDevice device, const VkBufferCreateInfo* pCreateInfo,
                         const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) {}
void PostCallCreateBuffer(device_layer_data* device_data, VkDevice device, const VkBufferCreateInfo* pCreateInfo,
                          const VkAllocationCallbacks* pAllocator, VkBuffer* pBuffer) {}

void PreCallDestroyBuffer(device_layer_data* device_data, VkDevice device, VkBuffer buffer,
                          const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyBuffer(device_layer_data* device_data, VkDevice device, VkBuffer buffer,
                           const VkAllocationCallbacks* pAllocator) {}

void PreCallCreateBufferView(device_layer_data* device_data, VkDevice device, const VkBufferViewCreateInfo* pCreateInfo,
                             const VkAllocationCallbacks* pAllocator, VkBufferView* pView) {}
void PostCallCreateBufferView(device_layer_data* device_data, VkDevice device, const VkBufferViewCreateInfo* pCreateInfo,
                              const VkAllocationCallbacks* pAllocator, VkBufferView* pView) {}

void PreCallDestroyBufferView(device_layer_data* device_data, VkDevice device, VkBufferView bufferView,
                              const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyBufferView(device_layer_data* device_data, VkDevice device, VkBufferView bufferView,
                               const VkAllocationCallbacks* pAllocator) {}

void PreCallCreateImage(device_layer_data* device_data, VkDevice device, const VkImageCreateInfo* pCreateInfo,
                        const VkAllocationCallbacks* pAllocator, VkImage* pImage) {}
void PostCallCreateImage(device_layer_data* device_data, VkDevice device, const VkImageCreateInfo* pCreateInfo,
                         const VkAllocationCallbacks* pAllocator, VkImage* pImage) {}

void PreCallDestroyImage(device_layer_data* device_data, VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyImage(device_layer_data* device_data, VkDevice device, VkImage image, const VkAllocationCallbacks* pAllocator) {
}

void PreCallGetImageSubresourceLayout(device_layer_data* device_data, VkDevice device, VkImage image,
                                      const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) {}
void PostCallGetImageSubresourceLayout(device_layer_data* device_data, VkDevice device, VkImage image,
                                       const VkImageSubresource* pSubresource, VkSubresourceLayout* pLayout) {}

void PreCallCreateImageView(device_layer_data* device_data, VkDevice device, const VkImageViewCreateInfo* pCreateInfo,
                            const VkAllocationCallbacks* pAllocator, VkImageView* pView) {}
void PostCallCreateImageView(device_layer_data* device_data, VkDevice device, const VkImageViewCreateInfo* pCreateInfo,
                             const VkAllocationCallbacks* pAllocator, VkImageView* pView) {}

void PreCallDestroyImageView(device_layer_data* device_data, VkDevice device, VkImageView imageView,
                             const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyImageView(device_layer_data* device_data, VkDevice device, VkImageView imageView,
                              const VkAllocationCallbacks* pAllocator) {}

void PreCallCreateShaderModule(device_layer_data* device_data, VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo,
                               const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) {}
void PostCallCreateShaderModule(device_layer_data* device_data, VkDevice device, const VkShaderModuleCreateInfo* pCreateInfo,
                                const VkAllocationCallbacks* pAllocator, VkShaderModule* pShaderModule) {}

void PreCallDestroyShaderModule(device_layer_data* device_data, VkDevice device, VkShaderModule shaderModule,
                                const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyShaderModule(device_layer_data* device_data, VkDevice device, VkShaderModule shaderModule,
                                 const VkAllocationCallbacks* pAllocator) {}

void PreCallCreatePipelineCache(device_layer_data* device_data, VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo,
                                const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) {}
void PostCallCreatePipelineCache(device_layer_data* device_data, VkDevice device, const VkPipelineCacheCreateInfo* pCreateInfo,
                                 const VkAllocationCallbacks* pAllocator, VkPipelineCache* pPipelineCache) {}

void PreCallDestroyPipelineCache(device_layer_data* device_data, VkDevice device, VkPipelineCache pipelineCache,
                                 const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyPipelineCache(device_layer_data* device_data, VkDevice device, VkPipelineCache pipelineCache,
                                  const VkAllocationCallbacks* pAllocator) {}

void PreCallGetPipelineCacheData(device_layer_data* device_data, VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize,
                                 void* pData) {}
void PostCallGetPipelineCacheData(device_layer_data* device_data, VkDevice device, VkPipelineCache pipelineCache, size_t* pDataSize,
                                  void* pData) {}

void PreCallMergePipelineCaches(device_layer_data* device_data, VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount,
                                const VkPipelineCache* pSrcCaches) {}
void PostCallMergePipelineCaches(device_layer_data* device_data, VkDevice device, VkPipelineCache dstCache, uint32_t srcCacheCount,
                                 const VkPipelineCache* pSrcCaches) {}

void PreCallCreateGraphicsPipelines(device_layer_data* device_data, VkDevice device, VkPipelineCache pipelineCache,
                                    uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos,
                                    const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {}
void PostCallCreateGraphicsPipelines(device_layer_data* device_data, VkDevice device, VkPipelineCache pipelineCache,
                                     uint32_t createInfoCount, const VkGraphicsPipelineCreateInfo* pCreateInfos,
                                     const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {}

void PreCallCreateComputePipelines(device_layer_data* device_data, VkDevice device, VkPipelineCache pipelineCache,
                                   uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos,
                                   const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {}
void PostCallCreateComputePipelines(device_layer_data* device_data, VkDevice device, VkPipelineCache pipelineCache,
                                    uint32_t createInfoCount, const VkComputePipelineCreateInfo* pCreateInfos,
                                    const VkAllocationCallbacks* pAllocator, VkPipeline* pPipelines) {}

void PreCallDestroyPipeline(device_layer_data* device_data, VkDevice device, VkPipeline pipeline,
                            const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyPipeline(device_layer_data* device_data, VkDevice device, VkPipeline pipeline,
                             const VkAllocationCallbacks* pAllocator) {}

void PreCallCreatePipelineLayout(device_layer_data* device_data, VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo,
                                 const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) {}
void PostCallCreatePipelineLayout(device_layer_data* device_data, VkDevice device, const VkPipelineLayoutCreateInfo* pCreateInfo,
                                  const VkAllocationCallbacks* pAllocator, VkPipelineLayout* pPipelineLayout) {}

void PreCallDestroyPipelineLayout(device_layer_data* device_data, VkDevice device, VkPipelineLayout pipelineLayout,
                                  const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyPipelineLayout(device_layer_data* device_data, VkDevice device, VkPipelineLayout pipelineLayout,
                                   const VkAllocationCallbacks* pAllocator) {}

void PreCallCreateSampler(device_layer_data* device_data, VkDevice device, const VkSamplerCreateInfo* pCreateInfo,
                          const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) {}
void PostCallCreateSampler(device_layer_data* device_data, VkDevice device, const VkSamplerCreateInfo* pCreateInfo,
                           const VkAllocationCallbacks* pAllocator, VkSampler* pSampler) {}

void PreCallDestroySampler(device_layer_data* device_data, VkDevice device, VkSampler sampler,
                           const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroySampler(device_layer_data* device_data, VkDevice device, VkSampler sampler,
                            const VkAllocationCallbacks* pAllocator) {}

void PreCallCreateDescriptorSetLayout(device_layer_data* device_data, VkDevice device,
                                      const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                      VkDescriptorSetLayout* pSetLayout) {}
void PostCallCreateDescriptorSetLayout(device_layer_data* device_data, VkDevice device,
                                       const VkDescriptorSetLayoutCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                       VkDescriptorSetLayout* pSetLayout) {}

void PreCallDestroyDescriptorSetLayout(device_layer_data* device_data, VkDevice device, VkDescriptorSetLayout descriptorSetLayout,
                                       const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyDescriptorSetLayout(device_layer_data* device_data, VkDevice device, VkDescriptorSetLayout descriptorSetLayout,
                                        const VkAllocationCallbacks* pAllocator) {}

void PreCallCreateDescriptorPool(device_layer_data* device_data, VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo,
                                 const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) {}
void PostCallCreateDescriptorPool(device_layer_data* device_data, VkDevice device, const VkDescriptorPoolCreateInfo* pCreateInfo,
                                  const VkAllocationCallbacks* pAllocator, VkDescriptorPool* pDescriptorPool) {}

void PreCallDestroyDescriptorPool(device_layer_data* device_data, VkDevice device, VkDescriptorPool descriptorPool,
                                  const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyDescriptorPool(device_layer_data* device_data, VkDevice device, VkDescriptorPool descriptorPool,
                                   const VkAllocationCallbacks* pAllocator) {}

void PreCallResetDescriptorPool(device_layer_data* device_data, VkDevice device, VkDescriptorPool descriptorPool,
                                VkDescriptorPoolResetFlags flags) {}
void PostCallResetDescriptorPool(device_layer_data* device_data, VkDevice device, VkDescriptorPool descriptorPool,
                                 VkDescriptorPoolResetFlags flags) {}

void PreCallAllocateDescriptorSets(device_layer_data* device_data, VkDevice device,
                                   const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) {}
void PostCallAllocateDescriptorSets(device_layer_data* device_data, VkDevice device,
                                    const VkDescriptorSetAllocateInfo* pAllocateInfo, VkDescriptorSet* pDescriptorSets) {}

void PreCallFreeDescriptorSets(device_layer_data* device_data, VkDevice device, VkDescriptorPool descriptorPool,
                               uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) {}
void PostCallFreeDescriptorSets(device_layer_data* device_data, VkDevice device, VkDescriptorPool descriptorPool,
                                uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets) {}

void PreCallUpdateDescriptorSets(device_layer_data* device_data, VkDevice device, uint32_t descriptorWriteCount,
                                 const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount,
                                 const VkCopyDescriptorSet* pDescriptorCopies) {}
void PostCallUpdateDescriptorSets(device_layer_data* device_data, VkDevice device, uint32_t descriptorWriteCount,
                                  const VkWriteDescriptorSet* pDescriptorWrites, uint32_t descriptorCopyCount,
                                  const VkCopyDescriptorSet* pDescriptorCopies) {}

void PreCallCreateFramebuffer(device_layer_data* device_data, VkDevice device, const VkFramebufferCreateInfo* pCreateInfo,
                              const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) {}
void PostCallCreateFramebuffer(device_layer_data* device_data, VkDevice device, const VkFramebufferCreateInfo* pCreateInfo,
                               const VkAllocationCallbacks* pAllocator, VkFramebuffer* pFramebuffer) {}

void PreCallDestroyFramebuffer(device_layer_data* device_data, VkDevice device, VkFramebuffer framebuffer,
                               const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyFramebuffer(device_layer_data* device_data, VkDevice device, VkFramebuffer framebuffer,
                                const VkAllocationCallbacks* pAllocator) {}

void PreCallCreateRenderPass(device_layer_data* device_data, VkDevice device, const VkRenderPassCreateInfo* pCreateInfo,
                             const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) {}
void PostCallCreateRenderPass(device_layer_data* device_data, VkDevice device, const VkRenderPassCreateInfo* pCreateInfo,
                              const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass) {}

void PreCallDestroyRenderPass(device_layer_data* device_data, VkDevice device, VkRenderPass renderPass,
                              const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyRenderPass(device_layer_data* device_data, VkDevice device, VkRenderPass renderPass,
                               const VkAllocationCallbacks* pAllocator) {}

void PreCallGetRenderAreaGranularity(device_layer_data* device_data, VkDevice device, VkRenderPass renderPass,
                                     VkExtent2D* pGranularity) {}
void PostCallGetRenderAreaGranularity(device_layer_data* device_data, VkDevice device, VkRenderPass renderPass,
                                      VkExtent2D* pGranularity) {}

void PreCallCreateCommandPool(device_layer_data* device_data, VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo,
                              const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) {}
void PostCallCreateCommandPool(device_layer_data* device_data, VkDevice device, const VkCommandPoolCreateInfo* pCreateInfo,
                               const VkAllocationCallbacks* pAllocator, VkCommandPool* pCommandPool) {}

void PreCallDestroyCommandPool(device_layer_data* device_data, VkDevice device, VkCommandPool commandPool,
                               const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyCommandPool(device_layer_data* device_data, VkDevice device, VkCommandPool commandPool,
                                const VkAllocationCallbacks* pAllocator) {}

void PreCallResetCommandPool(device_layer_data* device_data, VkDevice device, VkCommandPool commandPool,
                             VkCommandPoolResetFlags flags) {}
void PostCallResetCommandPool(device_layer_data* device_data, VkDevice device, VkCommandPool commandPool,
                              VkCommandPoolResetFlags flags) {}

void PreCallAllocateCommandBuffers(device_layer_data* device_data, VkDevice device,
                                   const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) {}
void PostCallAllocateCommandBuffers(device_layer_data* device_data, VkDevice device,
                                    const VkCommandBufferAllocateInfo* pAllocateInfo, VkCommandBuffer* pCommandBuffers) {
    state_tracker::AllocateCommandBuffers(device_data, pAllocateInfo, pCommandBuffers);
}

void PreCallFreeCommandBuffers(device_layer_data* device_data, VkDevice device, VkCommandPool commandPool,
                               uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) {}
void PostCallFreeCommandBuffers(device_layer_data* device_data, VkDevice device, VkCommandPool commandPool,
                                uint32_t commandBufferCount, const VkCommandBuffer* pCommandBuffers) {}

void PreCallBeginCommandBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                               const VkCommandBufferBeginInfo* pBeginInfo) {}
void PostCallBeginCommandBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                const VkCommandBufferBeginInfo* pBeginInfo) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_BEGIN);
}

void PreCallEndCommandBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer) {}
void PostCallEndCommandBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_END);
}

void PreCallResetCommandBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) {}
void PostCallResetCommandBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkCommandBufferResetFlags flags) {}

void PreCallDestroySurfaceKHR(instance_layer_data* instance_data, VkInstance instance, VkSurfaceKHR surface,
                              const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroySurfaceKHR(instance_layer_data* instance_data, VkInstance instance, VkSurfaceKHR surface,
                               const VkAllocationCallbacks* pAllocator) {}

void PreCallGetPhysicalDeviceSurfaceSupportKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                               uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported) {}
void PostCallGetPhysicalDeviceSurfaceSupportKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                uint32_t queueFamilyIndex, VkSurfaceKHR surface, VkBool32* pSupported) {}

void PreCallGetPhysicalDeviceSurfaceCapabilitiesKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                    VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities) {}
void PostCallGetPhysicalDeviceSurfaceCapabilitiesKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                     VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR* pSurfaceCapabilities) {}

void PreCallGetPhysicalDeviceSurfaceFormatsKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                               VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount,
                                               VkSurfaceFormatKHR* pSurfaceFormats) {}
void PostCallGetPhysicalDeviceSurfaceFormatsKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                VkSurfaceKHR surface, uint32_t* pSurfaceFormatCount,
                                                VkSurfaceFormatKHR* pSurfaceFormats) {}

void PreCallGetPhysicalDeviceSurfacePresentModesKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                    VkSurfaceKHR surface, uint32_t* pPresentModeCount,
                                                    VkPresentModeKHR* pPresentModes) {}
void PostCallGetPhysicalDeviceSurfacePresentModesKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                     VkSurfaceKHR surface, uint32_t* pPresentModeCount,
                                                     VkPresentModeKHR* pPresentModes) {}

void PreCallCreateSwapchainKHR(device_layer_data* device_data, VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo,
                               const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) {}
void PostCallCreateSwapchainKHR(device_layer_data* device_data, VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo,
                                const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) {}

void PreCallDestroySwapchainKHR(device_layer_data* device_data, VkDevice device, VkSwapchainKHR swapchain,
                                const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroySwapchainKHR(device_layer_data* device_data, VkDevice device, VkSwapchainKHR swapchain,
                                 const VkAllocationCallbacks* pAllocator) {}

void PreCallGetSwapchainImagesKHR(device_layer_data* device_data, VkDevice device, VkSwapchainKHR swapchain,
                                  uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) {}
void PostCallGetSwapchainImagesKHR(device_layer_data* device_data, VkDevice device, VkSwapchainKHR swapchain,
                                   uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) {}

void PreCallAcquireNextImageKHR(device_layer_data* device_data, VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout,
                                VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) {}
void PostCallAcquireNextImageKHR(device_layer_data* device_data, VkDevice device, VkSwapchainKHR swapchain, uint64_t timeout,
                                 VkSemaphore semaphore, VkFence fence, uint32_t* pImageIndex) {}

void PreCallQueuePresentKHR(device_layer_data* device_data, VkQueue queue, const VkPresentInfoKHR* pPresentInfo) {
    // Demo case to show how controller triggers display of state
    state_controller->HandlePresent();
}
void PostCallQueuePresentKHR(device_layer_data* device_data, VkQueue queue, const VkPresentInfoKHR* pPresentInfo) {}

void PreCallGetPhysicalDeviceDisplayPropertiesKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                  uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties) {}
void PostCallGetPhysicalDeviceDisplayPropertiesKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                   uint32_t* pPropertyCount, VkDisplayPropertiesKHR* pProperties) {}

void PreCallGetPhysicalDeviceDisplayPlanePropertiesKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                       uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties) {}
void PostCallGetPhysicalDeviceDisplayPlanePropertiesKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                        uint32_t* pPropertyCount, VkDisplayPlanePropertiesKHR* pProperties) {}

void PreCallGetDisplayPlaneSupportedDisplaysKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays) {}
void PostCallGetDisplayPlaneSupportedDisplaysKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                 uint32_t planeIndex, uint32_t* pDisplayCount, VkDisplayKHR* pDisplays) {}

void PreCallGetDisplayModePropertiesKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice, VkDisplayKHR display,
                                        uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties) {}
void PostCallGetDisplayModePropertiesKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice, VkDisplayKHR display,
                                         uint32_t* pPropertyCount, VkDisplayModePropertiesKHR* pProperties) {}

void PreCallCreateDisplayModeKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice, VkDisplayKHR display,
                                 const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                 VkDisplayModeKHR* pMode) {}
void PostCallCreateDisplayModeKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice, VkDisplayKHR display,
                                  const VkDisplayModeCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                  VkDisplayModeKHR* pMode) {}

void PreCallGetDisplayPlaneCapabilitiesKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                           VkDisplayModeKHR mode, uint32_t planeIndex,
                                           VkDisplayPlaneCapabilitiesKHR* pCapabilities) {}
void PostCallGetDisplayPlaneCapabilitiesKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                            VkDisplayModeKHR mode, uint32_t planeIndex,
                                            VkDisplayPlaneCapabilitiesKHR* pCapabilities) {}

void PreCallCreateDisplayPlaneSurfaceKHR(instance_layer_data* instance_data, VkInstance instance,
                                         const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                         VkSurfaceKHR* pSurface) {}
void PostCallCreateDisplayPlaneSurfaceKHR(instance_layer_data* instance_data, VkInstance instance,
                                          const VkDisplaySurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                          VkSurfaceKHR* pSurface) {}

void PreCallCreateSharedSwapchainsKHR(device_layer_data* device_data, VkDevice device, uint32_t swapchainCount,
                                      const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator,
                                      VkSwapchainKHR* pSwapchains) {}
void PostCallCreateSharedSwapchainsKHR(device_layer_data* device_data, VkDevice device, uint32_t swapchainCount,
                                       const VkSwapchainCreateInfoKHR* pCreateInfos, const VkAllocationCallbacks* pAllocator,
                                       VkSwapchainKHR* pSwapchains) {}

#ifdef VK_USE_PLATFORM_XLIB_KHR

void PreCallCreateXlibSurfaceKHR(instance_layer_data* instance_data, VkInstance instance,
                                 const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                 VkSurfaceKHR* pSurface) {}
void PostCallCreateXlibSurfaceKHR(instance_layer_data* instance_data, VkInstance instance,
                                  const VkXlibSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                  VkSurfaceKHR* pSurface) {}

void PreCallGetPhysicalDeviceXlibPresentationSupportKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                        uint32_t queueFamilyIndex, Display* dpy, VisualID visualID) {}
void PostCallGetPhysicalDeviceXlibPresentationSupportKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                         uint32_t queueFamilyIndex, Display* dpy, VisualID visualID) {}

#endif /* VK_USE_PLATFORM_XLIB_KHR */

#ifdef VK_USE_PLATFORM_XCB_KHR

void PreCallCreateXcbSurfaceKHR(instance_layer_data* instance_data, VkInstance instance,
                                const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                VkSurfaceKHR* pSurface) {}
void PostCallCreateXcbSurfaceKHR(instance_layer_data* instance_data, VkInstance instance,
                                 const VkXcbSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                 VkSurfaceKHR* pSurface) {}

void PreCallGetPhysicalDeviceXcbPresentationSupportKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                       uint32_t queueFamilyIndex, xcb_connection_t* connection,
                                                       xcb_visualid_t visual_id) {}
void PostCallGetPhysicalDeviceXcbPresentationSupportKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                        uint32_t queueFamilyIndex, xcb_connection_t* connection,
                                                        xcb_visualid_t visual_id) {}

#endif /* VK_USE_PLATFORM_XCB_KHR */

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

void PreCallCreateWaylandSurfaceKHR(instance_layer_data* instance_data, VkInstance instance,
                                    const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                    VkSurfaceKHR* pSurface) {}
void PostCallCreateWaylandSurfaceKHR(instance_layer_data* instance_data, VkInstance instance,
                                     const VkWaylandSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                     VkSurfaceKHR* pSurface) {}

void PreCallGetPhysicalDeviceWaylandPresentationSupportKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                           uint32_t queueFamilyIndex, struct wl_display* display) {}
void PostCallGetPhysicalDeviceWaylandPresentationSupportKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                            uint32_t queueFamilyIndex, struct wl_display* display) {}

#endif /* VK_USE_PLATFORM_WAYLAND_KHR */

#ifdef VK_USE_PLATFORM_MIR_KHR

void PreCallCreateMirSurfaceKHR(instance_layer_data* instance_data, VkInstance instance,
                                const VkMirSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                VkSurfaceKHR* pSurface) {}
void PostCallCreateMirSurfaceKHR(instance_layer_data* instance_data, VkInstance instance,
                                 const VkMirSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                 VkSurfaceKHR* pSurface) {}

void PreCallGetPhysicalDeviceMirPresentationSupportKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                       uint32_t queueFamilyIndex, MirConnection* connection) {}
void PostCallGetPhysicalDeviceMirPresentationSupportKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                        uint32_t queueFamilyIndex, MirConnection* connection) {}

#endif /* VK_USE_PLATFORM_MIR_KHR */

#ifdef VK_USE_PLATFORM_ANDROID_KHR

void PreCallCreateAndroidSurfaceKHR(instance_layer_data* instance_data, VkInstance instance,
                                    const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                    VkSurfaceKHR* pSurface) {}
void PostCallCreateAndroidSurfaceKHR(instance_layer_data* instance_data, VkInstance instance,
                                     const VkAndroidSurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                     VkSurfaceKHR* pSurface) {}

#endif /* VK_USE_PLATFORM_ANDROID_KHR */

#ifdef VK_USE_PLATFORM_WIN32_KHR

void PreCallCreateWin32SurfaceKHR(instance_layer_data* instance_data, VkInstance instance,
                                  const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                  VkSurfaceKHR* pSurface) {}
void PostCallCreateWin32SurfaceKHR(instance_layer_data* instance_data, VkInstance instance,
                                   const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                   VkSurfaceKHR* pSurface) {}

void PreCallGetPhysicalDeviceWin32PresentationSupportKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                         uint32_t queueFamilyIndex) {}
void PostCallGetPhysicalDeviceWin32PresentationSupportKHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                          uint32_t queueFamilyIndex) {}

#endif /* VK_USE_PLATFORM_WIN32_KHR */

void PreCallGetPhysicalDeviceFeatures2KHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                          VkPhysicalDeviceFeatures2KHR* pFeatures) {}
void PostCallGetPhysicalDeviceFeatures2KHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                           VkPhysicalDeviceFeatures2KHR* pFeatures) {}

void PreCallGetPhysicalDeviceProperties2KHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                            VkPhysicalDeviceProperties2KHR* pProperties) {}
void PostCallGetPhysicalDeviceProperties2KHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                             VkPhysicalDeviceProperties2KHR* pProperties) {}

void PreCallGetPhysicalDeviceFormatProperties2KHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                  VkFormat format, VkFormatProperties2KHR* pFormatProperties) {}
void PostCallGetPhysicalDeviceFormatProperties2KHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                   VkFormat format, VkFormatProperties2KHR* pFormatProperties) {}

void PreCallGetPhysicalDeviceImageFormatProperties2KHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                       const VkPhysicalDeviceImageFormatInfo2KHR* pImageFormatInfo,
                                                       VkImageFormatProperties2KHR* pImageFormatProperties) {}
void PostCallGetPhysicalDeviceImageFormatProperties2KHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                        const VkPhysicalDeviceImageFormatInfo2KHR* pImageFormatInfo,
                                                        VkImageFormatProperties2KHR* pImageFormatProperties) {}

void PreCallGetPhysicalDeviceQueueFamilyProperties2KHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                       uint32_t* pQueueFamilyPropertyCount,
                                                       VkQueueFamilyProperties2KHR* pQueueFamilyProperties) {}
void PostCallGetPhysicalDeviceQueueFamilyProperties2KHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                        uint32_t* pQueueFamilyPropertyCount,
                                                        VkQueueFamilyProperties2KHR* pQueueFamilyProperties) {}

void PreCallGetPhysicalDeviceMemoryProperties2KHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                  VkPhysicalDeviceMemoryProperties2KHR* pMemoryProperties) {}
void PostCallGetPhysicalDeviceMemoryProperties2KHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                   VkPhysicalDeviceMemoryProperties2KHR* pMemoryProperties) {}

void PreCallGetPhysicalDeviceSparseImageFormatProperties2KHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                             const VkPhysicalDeviceSparseImageFormatInfo2KHR* pFormatInfo,
                                                             uint32_t* pPropertyCount,
                                                             VkSparseImageFormatProperties2KHR* pProperties) {}
void PostCallGetPhysicalDeviceSparseImageFormatProperties2KHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                              const VkPhysicalDeviceSparseImageFormatInfo2KHR* pFormatInfo,
                                                              uint32_t* pPropertyCount,
                                                              VkSparseImageFormatProperties2KHR* pProperties) {}

void PreCallTrimCommandPoolKHR(device_layer_data* device_data, VkDevice device, VkCommandPool commandPool,
                               VkCommandPoolTrimFlagsKHR flags) {}
void PostCallTrimCommandPoolKHR(device_layer_data* device_data, VkDevice device, VkCommandPool commandPool,
                                VkCommandPoolTrimFlagsKHR flags) {}

void PreCallCreateDescriptorUpdateTemplateKHR(device_layer_data* device_data, VkDevice device,
                                              const VkDescriptorUpdateTemplateCreateInfoKHR* pCreateInfo,
                                              const VkAllocationCallbacks* pAllocator,
                                              VkDescriptorUpdateTemplateKHR* pDescriptorUpdateTemplate) {}
void PostCallCreateDescriptorUpdateTemplateKHR(device_layer_data* device_data, VkDevice device,
                                               const VkDescriptorUpdateTemplateCreateInfoKHR* pCreateInfo,
                                               const VkAllocationCallbacks* pAllocator,
                                               VkDescriptorUpdateTemplateKHR* pDescriptorUpdateTemplate) {}

void PreCallDestroyDescriptorUpdateTemplateKHR(device_layer_data* device_data, VkDevice device,
                                               VkDescriptorUpdateTemplateKHR descriptorUpdateTemplate,
                                               const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyDescriptorUpdateTemplateKHR(device_layer_data* device_data, VkDevice device,
                                                VkDescriptorUpdateTemplateKHR descriptorUpdateTemplate,
                                                const VkAllocationCallbacks* pAllocator) {}

void PreCallUpdateDescriptorSetWithTemplateKHR(device_layer_data* device_data, VkDevice device, VkDescriptorSet descriptorSet,
                                               VkDescriptorUpdateTemplateKHR descriptorUpdateTemplate, const void* pData) {}
void PostCallUpdateDescriptorSetWithTemplateKHR(device_layer_data* device_data, VkDevice device, VkDescriptorSet descriptorSet,
                                                VkDescriptorUpdateTemplateKHR descriptorUpdateTemplate, const void* pData) {}

void PreCallGetSwapchainStatusKHR(device_layer_data* device_data, VkDevice device, VkSwapchainKHR swapchain) {}
void PostCallGetSwapchainStatusKHR(device_layer_data* device_data, VkDevice device, VkSwapchainKHR swapchain) {}

void PreCallGetPhysicalDeviceSurfaceCapabilities2KHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                     const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo,
                                                     VkSurfaceCapabilities2KHR* pSurfaceCapabilities) {}
void PostCallGetPhysicalDeviceSurfaceCapabilities2KHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                      const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo,
                                                      VkSurfaceCapabilities2KHR* pSurfaceCapabilities) {}

void PreCallGetPhysicalDeviceSurfaceFormats2KHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount,
                                                VkSurfaceFormat2KHR* pSurfaceFormats) {}
void PostCallGetPhysicalDeviceSurfaceFormats2KHR(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                 const VkPhysicalDeviceSurfaceInfo2KHR* pSurfaceInfo, uint32_t* pSurfaceFormatCount,
                                                 VkSurfaceFormat2KHR* pSurfaceFormats) {}

void PreCallDebugReportCallbackEXT(device_layer_data* device_data) {}
void PostCallDebugReportCallbackEXT(device_layer_data* device_data) {}

// Skipping line:    PFN_vkDebugReportCallbackEXT    pfnCallback;

void PreCallCreateDebugReportCallbackEXT(instance_layer_data* instance_data, VkInstance instance,
                                         const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
                                         const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {}
void PostCallCreateDebugReportCallbackEXT(instance_layer_data* instance_data, VkInstance instance,
                                          const VkDebugReportCallbackCreateInfoEXT* pCreateInfo,
                                          const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {}

void PreCallDestroyDebugReportCallbackEXT(instance_layer_data* instance_data, VkInstance instance,
                                          VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyDebugReportCallbackEXT(instance_layer_data* instance_data, VkInstance instance,
                                           VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {}

void PreCallDebugReportMessageEXT(instance_layer_data* instance_data, VkInstance instance, VkDebugReportFlagsEXT flags,
                                  VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode,
                                  const char* pLayerPrefix, const char* pMessage) {}
void PostCallDebugReportMessageEXT(instance_layer_data* instance_data, VkInstance instance, VkDebugReportFlagsEXT flags,
                                   VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode,
                                   const char* pLayerPrefix, const char* pMessage) {}

void PreCallDebugMarkerSetObjectTagEXT(device_layer_data* device_data, VkDevice device, VkDebugMarkerObjectTagInfoEXT* pTagInfo) {}
void PostCallDebugMarkerSetObjectTagEXT(device_layer_data* device_data, VkDevice device, VkDebugMarkerObjectTagInfoEXT* pTagInfo) {}

void PreCallDebugMarkerSetObjectNameEXT(device_layer_data* device_data, VkDevice device,
                                        VkDebugMarkerObjectNameInfoEXT* pNameInfo) {}
void PostCallDebugMarkerSetObjectNameEXT(device_layer_data* device_data, VkDevice device,
                                         VkDebugMarkerObjectNameInfoEXT* pNameInfo) {}

void PreCallGetPhysicalDeviceExternalImageFormatPropertiesNV(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                             VkFormat format, VkImageType type, VkImageTiling tiling,
                                                             VkImageUsageFlags usage, VkImageCreateFlags flags,
                                                             VkExternalMemoryHandleTypeFlagsNV externalHandleType,
                                                             VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties) {}
void PostCallGetPhysicalDeviceExternalImageFormatPropertiesNV(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                              VkFormat format, VkImageType type, VkImageTiling tiling,
                                                              VkImageUsageFlags usage, VkImageCreateFlags flags,
                                                              VkExternalMemoryHandleTypeFlagsNV externalHandleType,
                                                              VkExternalImageFormatPropertiesNV* pExternalImageFormatProperties) {}

#ifdef VK_USE_PLATFORM_WIN32_KHR

void PreCallGetMemoryWin32HandleNV(device_layer_data* device_data, VkDevice device, VkDeviceMemory memory,
                                   VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle) {}
void PostCallGetMemoryWin32HandleNV(device_layer_data* device_data, VkDevice device, VkDeviceMemory memory,
                                    VkExternalMemoryHandleTypeFlagsNV handleType, HANDLE* pHandle) {}

#endif /* VK_USE_PLATFORM_WIN32_KHR */

#ifdef VK_USE_PLATFORM_WIN32_KHR

#endif /* VK_USE_PLATFORM_WIN32_KHR */

void PreCallGetDeviceGroupPeerMemoryFeaturesKHX(device_layer_data* device_data, VkDevice device, uint32_t heapIndex,
                                                uint32_t localDeviceIndex, uint32_t remoteDeviceIndex,
                                                VkPeerMemoryFeatureFlagsKHX* pPeerMemoryFeatures) {}
void PostCallGetDeviceGroupPeerMemoryFeaturesKHX(device_layer_data* device_data, VkDevice device, uint32_t heapIndex,
                                                 uint32_t localDeviceIndex, uint32_t remoteDeviceIndex,
                                                 VkPeerMemoryFeatureFlagsKHX* pPeerMemoryFeatures) {}

void PreCallBindBufferMemory2KHX(device_layer_data* device_data, VkDevice device, uint32_t bindInfoCount,
                                 const VkBindBufferMemoryInfoKHX* pBindInfos) {}
void PostCallBindBufferMemory2KHX(device_layer_data* device_data, VkDevice device, uint32_t bindInfoCount,
                                  const VkBindBufferMemoryInfoKHX* pBindInfos) {}

void PreCallBindImageMemory2KHX(device_layer_data* device_data, VkDevice device, uint32_t bindInfoCount,
                                const VkBindImageMemoryInfoKHX* pBindInfos) {}
void PostCallBindImageMemory2KHX(device_layer_data* device_data, VkDevice device, uint32_t bindInfoCount,
                                 const VkBindImageMemoryInfoKHX* pBindInfos) {}

void PreCallGetDeviceGroupPresentCapabilitiesKHX(device_layer_data* device_data, VkDevice device,
                                                 VkDeviceGroupPresentCapabilitiesKHX* pDeviceGroupPresentCapabilities) {}
void PostCallGetDeviceGroupPresentCapabilitiesKHX(device_layer_data* device_data, VkDevice device,
                                                  VkDeviceGroupPresentCapabilitiesKHX* pDeviceGroupPresentCapabilities) {}

void PreCallGetDeviceGroupSurfacePresentModesKHX(device_layer_data* device_data, VkDevice device, VkSurfaceKHR surface,
                                                 VkDeviceGroupPresentModeFlagsKHX* pModes) {}
void PostCallGetDeviceGroupSurfacePresentModesKHX(device_layer_data* device_data, VkDevice device, VkSurfaceKHR surface,
                                                  VkDeviceGroupPresentModeFlagsKHX* pModes) {}

void PreCallAcquireNextImage2KHX(device_layer_data* device_data, VkDevice device, const VkAcquireNextImageInfoKHX* pAcquireInfo,
                                 uint32_t* pImageIndex) {}
void PostCallAcquireNextImage2KHX(device_layer_data* device_data, VkDevice device, const VkAcquireNextImageInfoKHX* pAcquireInfo,
                                  uint32_t* pImageIndex) {}

void PreCallGetPhysicalDevicePresentRectanglesKHX(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                  VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects) {}
void PostCallGetPhysicalDevicePresentRectanglesKHX(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                   VkSurfaceKHR surface, uint32_t* pRectCount, VkRect2D* pRects) {}

#ifdef VK_USE_PLATFORM_VI_NN

void PreCallCreateViSurfaceNN(instance_layer_data* instance_data, VkInstance instance, const VkViSurfaceCreateInfoNN* pCreateInfo,
                              const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {}
void PostCallCreateViSurfaceNN(instance_layer_data* instance_data, VkInstance instance, const VkViSurfaceCreateInfoNN* pCreateInfo,
                               const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface) {}

#endif /* VK_USE_PLATFORM_VI_NN */

void PreCallEnumeratePhysicalDeviceGroupsKHX(instance_layer_data* instance_data, VkInstance instance,
                                             uint32_t* pPhysicalDeviceGroupCount,
                                             VkPhysicalDeviceGroupPropertiesKHX* pPhysicalDeviceGroupProperties) {}
void PostCallEnumeratePhysicalDeviceGroupsKHX(instance_layer_data* instance_data, VkInstance instance,
                                              uint32_t* pPhysicalDeviceGroupCount,
                                              VkPhysicalDeviceGroupPropertiesKHX* pPhysicalDeviceGroupProperties) {}

void PreCallGetPhysicalDeviceExternalBufferPropertiesKHX(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                         const VkPhysicalDeviceExternalBufferInfoKHX* pExternalBufferInfo,
                                                         VkExternalBufferPropertiesKHX* pExternalBufferProperties) {}
void PostCallGetPhysicalDeviceExternalBufferPropertiesKHX(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                          const VkPhysicalDeviceExternalBufferInfoKHX* pExternalBufferInfo,
                                                          VkExternalBufferPropertiesKHX* pExternalBufferProperties) {}

#ifdef VK_USE_PLATFORM_WIN32_KHX

void PreCallGetMemoryWin32HandleKHX(device_layer_data* device_data, VkDevice device, VkDeviceMemory memory,
                                    VkExternalMemoryHandleTypeFlagBitsKHX handleType, HANDLE* pHandle) {}
void PostCallGetMemoryWin32HandleKHX(device_layer_data* device_data, VkDevice device, VkDeviceMemory memory,
                                     VkExternalMemoryHandleTypeFlagBitsKHX handleType, HANDLE* pHandle) {}

void PreCallGetMemoryWin32HandlePropertiesKHX(device_layer_data* device_data, VkDevice device,
                                              VkExternalMemoryHandleTypeFlagBitsKHX handleType, HANDLE handle,
                                              VkMemoryWin32HandlePropertiesKHX* pMemoryWin32HandleProperties) {}
void PostCallGetMemoryWin32HandlePropertiesKHX(device_layer_data* device_data, VkDevice device,
                                               VkExternalMemoryHandleTypeFlagBitsKHX handleType, HANDLE handle,
                                               VkMemoryWin32HandlePropertiesKHX* pMemoryWin32HandleProperties) {}

#endif /* VK_USE_PLATFORM_WIN32_KHX */

void PreCallGetMemoryFdKHX(device_layer_data* device_data, VkDevice device, VkDeviceMemory memory,
                           VkExternalMemoryHandleTypeFlagBitsKHX handleType, int* pFd) {}
void PostCallGetMemoryFdKHX(device_layer_data* device_data, VkDevice device, VkDeviceMemory memory,
                            VkExternalMemoryHandleTypeFlagBitsKHX handleType, int* pFd) {}

void PreCallGetMemoryFdPropertiesKHX(device_layer_data* device_data, VkDevice device,
                                     VkExternalMemoryHandleTypeFlagBitsKHX handleType, int fd,
                                     VkMemoryFdPropertiesKHX* pMemoryFdProperties) {}
void PostCallGetMemoryFdPropertiesKHX(device_layer_data* device_data, VkDevice device,
                                      VkExternalMemoryHandleTypeFlagBitsKHX handleType, int fd,
                                      VkMemoryFdPropertiesKHX* pMemoryFdProperties) {}

#ifdef VK_USE_PLATFORM_WIN32_KHR

#endif /* VK_USE_PLATFORM_WIN32_KHR */

void PreCallGetPhysicalDeviceExternalSemaphorePropertiesKHX(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                            const VkPhysicalDeviceExternalSemaphoreInfoKHX* pExternalSemaphoreInfo,
                                                            VkExternalSemaphorePropertiesKHX* pExternalSemaphoreProperties) {}
void PostCallGetPhysicalDeviceExternalSemaphorePropertiesKHX(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                             const VkPhysicalDeviceExternalSemaphoreInfoKHX* pExternalSemaphoreInfo,
                                                             VkExternalSemaphorePropertiesKHX* pExternalSemaphoreProperties) {}

#ifdef VK_USE_PLATFORM_WIN32_KHX

void PreCallImportSemaphoreWin32HandleKHX(device_layer_data* device_data, VkDevice device,
                                          const VkImportSemaphoreWin32HandleInfoKHX* pImportSemaphoreWin32HandleInfo) {}
void PostCallImportSemaphoreWin32HandleKHX(device_layer_data* device_data, VkDevice device,
                                           const VkImportSemaphoreWin32HandleInfoKHX* pImportSemaphoreWin32HandleInfo) {}

void PreCallGetSemaphoreWin32HandleKHX(device_layer_data* device_data, VkDevice device, VkSemaphore semaphore,
                                       VkExternalSemaphoreHandleTypeFlagBitsKHX handleType, HANDLE* pHandle) {}
void PostCallGetSemaphoreWin32HandleKHX(device_layer_data* device_data, VkDevice device, VkSemaphore semaphore,
                                        VkExternalSemaphoreHandleTypeFlagBitsKHX handleType, HANDLE* pHandle) {}

#endif /* VK_USE_PLATFORM_WIN32_KHX */

void PreCallImportSemaphoreFdKHX(device_layer_data* device_data, VkDevice device,
                                 const VkImportSemaphoreFdInfoKHX* pImportSemaphoreFdInfo) {}
void PostCallImportSemaphoreFdKHX(device_layer_data* device_data, VkDevice device,
                                  const VkImportSemaphoreFdInfoKHX* pImportSemaphoreFdInfo) {}

void PreCallGetSemaphoreFdKHX(device_layer_data* device_data, VkDevice device, VkSemaphore semaphore,
                              VkExternalSemaphoreHandleTypeFlagBitsKHX handleType, int* pFd) {}
void PostCallGetSemaphoreFdKHX(device_layer_data* device_data, VkDevice device, VkSemaphore semaphore,
                               VkExternalSemaphoreHandleTypeFlagBitsKHX handleType, int* pFd) {}

void PreCallCreateIndirectCommandsLayoutNVX(device_layer_data* device_data, VkDevice device,
                                            const VkIndirectCommandsLayoutCreateInfoNVX* pCreateInfo,
                                            const VkAllocationCallbacks* pAllocator,
                                            VkIndirectCommandsLayoutNVX* pIndirectCommandsLayout) {}
void PostCallCreateIndirectCommandsLayoutNVX(device_layer_data* device_data, VkDevice device,
                                             const VkIndirectCommandsLayoutCreateInfoNVX* pCreateInfo,
                                             const VkAllocationCallbacks* pAllocator,
                                             VkIndirectCommandsLayoutNVX* pIndirectCommandsLayout) {}

void PreCallDestroyIndirectCommandsLayoutNVX(device_layer_data* device_data, VkDevice device,
                                             VkIndirectCommandsLayoutNVX indirectCommandsLayout,
                                             const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyIndirectCommandsLayoutNVX(device_layer_data* device_data, VkDevice device,
                                              VkIndirectCommandsLayoutNVX indirectCommandsLayout,
                                              const VkAllocationCallbacks* pAllocator) {}

void PreCallCreateObjectTableNVX(device_layer_data* device_data, VkDevice device, const VkObjectTableCreateInfoNVX* pCreateInfo,
                                 const VkAllocationCallbacks* pAllocator, VkObjectTableNVX* pObjectTable) {}
void PostCallCreateObjectTableNVX(device_layer_data* device_data, VkDevice device, const VkObjectTableCreateInfoNVX* pCreateInfo,
                                  const VkAllocationCallbacks* pAllocator, VkObjectTableNVX* pObjectTable) {}

void PreCallDestroyObjectTableNVX(device_layer_data* device_data, VkDevice device, VkObjectTableNVX objectTable,
                                  const VkAllocationCallbacks* pAllocator) {}
void PostCallDestroyObjectTableNVX(device_layer_data* device_data, VkDevice device, VkObjectTableNVX objectTable,
                                   const VkAllocationCallbacks* pAllocator) {}

void PreCallRegisterObjectsNVX(device_layer_data* device_data, VkDevice device, VkObjectTableNVX objectTable, uint32_t objectCount,
                               const VkObjectTableEntryNVX* const* ppObjectTableEntries, const uint32_t* pObjectIndices) {}
void PostCallRegisterObjectsNVX(device_layer_data* device_data, VkDevice device, VkObjectTableNVX objectTable, uint32_t objectCount,
                                const VkObjectTableEntryNVX* const* ppObjectTableEntries, const uint32_t* pObjectIndices) {}

void PreCallUnregisterObjectsNVX(device_layer_data* device_data, VkDevice device, VkObjectTableNVX objectTable,
                                 uint32_t objectCount, const VkObjectEntryTypeNVX* pObjectEntryTypes,
                                 const uint32_t* pObjectIndices) {}
void PostCallUnregisterObjectsNVX(device_layer_data* device_data, VkDevice device, VkObjectTableNVX objectTable,
                                  uint32_t objectCount, const VkObjectEntryTypeNVX* pObjectEntryTypes,
                                  const uint32_t* pObjectIndices) {}

void PreCallGetPhysicalDeviceGeneratedCommandsPropertiesNVX(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                            VkDeviceGeneratedCommandsFeaturesNVX* pFeatures,
                                                            VkDeviceGeneratedCommandsLimitsNVX* pLimits) {}
void PostCallGetPhysicalDeviceGeneratedCommandsPropertiesNVX(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                             VkDeviceGeneratedCommandsFeaturesNVX* pFeatures,
                                                             VkDeviceGeneratedCommandsLimitsNVX* pLimits) {}

void PreCallReleaseDisplayEXT(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice, VkDisplayKHR display) {}
void PostCallReleaseDisplayEXT(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice, VkDisplayKHR display) {}

#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT

void PreCallAcquireXlibDisplayEXT(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice, Display* dpy,
                                  VkDisplayKHR display) {}
void PostCallAcquireXlibDisplayEXT(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice, Display* dpy,
                                   VkDisplayKHR display) {}

void PreCallGetRandROutputDisplayEXT(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice, Display* dpy,
                                     RROutput rrOutput, VkDisplayKHR* pDisplay) {}
void PostCallGetRandROutputDisplayEXT(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice, Display* dpy,
                                      RROutput rrOutput, VkDisplayKHR* pDisplay) {}

#endif /* VK_USE_PLATFORM_XLIB_XRANDR_EXT */

void PreCallGetPhysicalDeviceSurfaceCapabilities2EXT(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                     VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities) {}
void PostCallGetPhysicalDeviceSurfaceCapabilities2EXT(instance_layer_data* instance_data, VkPhysicalDevice physicalDevice,
                                                      VkSurfaceKHR surface, VkSurfaceCapabilities2EXT* pSurfaceCapabilities) {}

void PreCallDisplayPowerControlEXT(device_layer_data* device_data, VkDevice device, VkDisplayKHR display,
                                   const VkDisplayPowerInfoEXT* pDisplayPowerInfo) {}
void PostCallDisplayPowerControlEXT(device_layer_data* device_data, VkDevice device, VkDisplayKHR display,
                                    const VkDisplayPowerInfoEXT* pDisplayPowerInfo) {}

void PreCallRegisterDeviceEventEXT(device_layer_data* device_data, VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo,
                                   const VkAllocationCallbacks* pAllocator, VkFence* pFence) {}
void PostCallRegisterDeviceEventEXT(device_layer_data* device_data, VkDevice device, const VkDeviceEventInfoEXT* pDeviceEventInfo,
                                    const VkAllocationCallbacks* pAllocator, VkFence* pFence) {}

void PreCallRegisterDisplayEventEXT(device_layer_data* device_data, VkDevice device, VkDisplayKHR display,
                                    const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator,
                                    VkFence* pFence) {}
void PostCallRegisterDisplayEventEXT(device_layer_data* device_data, VkDevice device, VkDisplayKHR display,
                                     const VkDisplayEventInfoEXT* pDisplayEventInfo, const VkAllocationCallbacks* pAllocator,
                                     VkFence* pFence) {}

void PreCallGetSwapchainCounterEXT(device_layer_data* device_data, VkDevice device, VkSwapchainKHR swapchain,
                                   VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) {}
void PostCallGetSwapchainCounterEXT(device_layer_data* device_data, VkDevice device, VkSwapchainKHR swapchain,
                                    VkSurfaceCounterFlagBitsEXT counter, uint64_t* pCounterValue) {}

void PreCallGetRefreshCycleDurationGOOGLE(device_layer_data* device_data, VkDevice device, VkSwapchainKHR swapchain,
                                          VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties) {}
void PostCallGetRefreshCycleDurationGOOGLE(device_layer_data* device_data, VkDevice device, VkSwapchainKHR swapchain,
                                           VkRefreshCycleDurationGOOGLE* pDisplayTimingProperties) {}

void PreCallGetPastPresentationTimingGOOGLE(device_layer_data* device_data, VkDevice device, VkSwapchainKHR swapchain,
                                            uint32_t* pPresentationTimingCount,
                                            VkPastPresentationTimingGOOGLE* pPresentationTimings) {}
void PostCallGetPastPresentationTimingGOOGLE(device_layer_data* device_data, VkDevice device, VkSwapchainKHR swapchain,
                                             uint32_t* pPresentationTimingCount,
                                             VkPastPresentationTimingGOOGLE* pPresentationTimings) {}

void PreCallSetHdrMetadataEXT(device_layer_data* device_data, VkDevice device, uint32_t swapchainCount,
                              const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata) {}
void PostCallSetHdrMetadataEXT(device_layer_data* device_data, VkDevice device, uint32_t swapchainCount,
                               const VkSwapchainKHR* pSwapchains, const VkHdrMetadataEXT* pMetadata) {}

#ifdef VK_USE_PLATFORM_IOS_MVK

void PreCallCreateIOSSurfaceMVK(instance_layer_data* instance_data, VkInstance instance,
                                const VkIOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                VkSurfaceKHR* pSurface) {}
void PostCallCreateIOSSurfaceMVK(instance_layer_data* instance_data, VkInstance instance,
                                 const VkIOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                 VkSurfaceKHR* pSurface) {}

#endif /* VK_USE_PLATFORM_IOS_MVK */

#ifdef VK_USE_PLATFORM_MACOS_MVK

void PreCallCreateMacOSSurfaceMVK(instance_layer_data* instance_data, VkInstance instance,
                                  const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                  VkSurfaceKHR* pSurface) {}
void PostCallCreateMacOSSurfaceMVK(instance_layer_data* instance_data, VkInstance instance,
                                   const VkMacOSSurfaceCreateInfoMVK* pCreateInfo, const VkAllocationCallbacks* pAllocator,
                                   VkSurfaceKHR* pSurface) {}

#endif /* VK_USE_PLATFORM_MACOS_MVK */

} // namespace vkaid
