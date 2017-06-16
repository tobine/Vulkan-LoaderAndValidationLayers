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
#include <memory>

namespace vkaid {

void PostCallCreateDevice(device_layer_data* device_data, const VkDeviceCreateInfo* pCreateInfo,
                          const VkAllocationCallbacks* pAllocator, VkDevice* pDevice) {
    device_data->state_ptr = static_cast<void*>(new device_state_struct());
}

void PostCallDestroyDevice(device_layer_data* device_data, const VkAllocationCallbacks* pAllocator) {
    delete static_cast<device_state_struct*>(device_data->state_ptr);
}

void PreCallQueuePresentKHR(device_layer_data* device_data, VkQueue queue, const VkPresentInfoKHR* pPresentInfo) {
    // TODO : Ideally this will be in a separate module/namespace that is decoupled from state_tracker
    state_tracker::DisplayCommandBuffers(device_data);
}

void PostCallAllocateCommandBuffers(
    device_layer_data*                          device_data,
    const VkCommandBufferAllocateInfo*          pAllocateInfo,
    VkCommandBuffer*                            pCommandBuffers)
{
    state_tracker::AllocateCommandBuffers(device_data, pAllocateInfo, pCommandBuffers);
}

void PostCallCmdBindPipeline(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint,
                             VkPipeline pipeline) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_BIND_PIPELINE);
}
void PostCallCmdSetViewport(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t firstViewport,
                            uint32_t viewportCount, const VkViewport* pViewports) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_VIEWPORT);
}
void PostCallCmdSetScissor(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t firstScissor,
                           uint32_t scissorCount, const VkRect2D* pScissors) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_SCISSOR);
}
void PostCallCmdSetLineWidth(device_layer_data* device_data, VkCommandBuffer commandBuffer, float lineWidth) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_LINE_WIDTH);
}
void PostCallCmdSetDepthBias(device_layer_data* device_data, VkCommandBuffer commandBuffer, float depthBiasConstantFactor,
                             float depthBiasClamp, float depthBiasSlopeFactor) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_DEPTH_BIAS);
}
void PostCallCmdSetBlendConstants(device_layer_data* device_data, VkCommandBuffer commandBuffer, const float blendConstants[4]) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_BLEND_CONSTANTS);
}
void PostCallCmdSetDepthBounds(device_layer_data* device_data, VkCommandBuffer commandBuffer, float minDepthBounds,
                               float maxDepthBounds) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_DEPTH_BOUNDS);
}
void PostCallCmdSetStencilCompareMask(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask,
                                      uint32_t compareMask) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_STENCIL_COMPARE_MASK);
}
void PostCallCmdSetStencilWriteMask(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask,
                                    uint32_t writeMask) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_STENCIL_WRITE_MASK);
}
void PostCallCmdSetStencilReference(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask,
                                    uint32_t reference) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_STENCIL_REFERENCE);
}
void PostCallCmdBindDescriptorSets(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                   VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet,
                                   uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount,
                                   const uint32_t* pDynamicOffsets) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_BIND_DESCRIPTOR_SETS);
}
void PostCallCmdBindIndexBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset,
                                VkIndexType indexType) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_BIND_INDEX_BUFFER);
}
void PostCallCmdBindVertexBuffers(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t firstBinding,
                                  uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_BIND_VERTEX_BUFFERS);
}
void PostCallCmdDraw(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount,
                     uint32_t firstVertex, uint32_t firstInstance) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DRAW);
}
void PostCallCmdDrawIndexed(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t indexCount,
                            uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DRAW_INDEXED);
}
void PostCallCmdDrawIndirect(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset,
                             uint32_t drawCount, uint32_t stride) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DRAW_INDIRECT);
}
void PostCallCmdDrawIndexedIndirect(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer,
                                    VkDeviceSize offset, uint32_t drawCount, uint32_t stride) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DRAW_INDEXED_INDIRECT);
}
void PostCallCmdDispatch(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY,
                         uint32_t groupCountZ) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DISPATCH);
}
void PostCallCmdDispatchIndirect(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer,
                                 VkDeviceSize offset) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DISPATCH_INDIRECT);
}
void PostCallCmdCopyBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer,
                           uint32_t regionCount, const VkBufferCopy* pRegions) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_COPY_BUFFER);
}
void PostCallCmdCopyImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage srcImage,
                          VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount,
                          const VkImageCopy* pRegions) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_COPY_IMAGE);
}
void PostCallCmdBlitImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage srcImage,
                          VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount,
                          const VkImageBlit* pRegions, VkFilter filter) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_BLIT_IMAGE);
}
void PostCallCmdCopyBufferToImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer srcBuffer,
                                  VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount,
                                  const VkBufferImageCopy* pRegions) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_COPY_BUFFER_TO_IMAGE);
}
void PostCallCmdCopyImageToBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage srcImage,
                                  VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount,
                                  const VkBufferImageCopy* pRegions) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_COPY_IMAGE_TO_BUFFER);
}
void PostCallCmdUpdateBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer dstBuffer,
                             VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_UPDATE_BUFFER);
}
void PostCallCmdFillBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer dstBuffer,
                           VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_FILL_BUFFER);
}
void PostCallCmdClearColorImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage image,
                                VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount,
                                const VkImageSubresourceRange* pRanges) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_CLEAR_COLOR_IMAGE);
}
void PostCallCmdClearDepthStencilImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage image,
                                       VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil,
                                       uint32_t rangeCount, const VkImageSubresourceRange* pRanges) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_CLEAR_DEPTH_STENCIL_IMAGE);
}
void PostCallCmdClearAttachments(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t attachmentCount,
                                 const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_CLEAR_ATTACHMENTS);
}
void PostCallCmdResolveImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage srcImage,
                             VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount,
                             const VkImageResolve* pRegions) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_RESOLVE_IMAGE);
}
void PostCallCmdSetEvent(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkEvent event,
                         VkPipelineStageFlags stageMask) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_EVENT);
}
void PostCallCmdResetEvent(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkEvent event,
                           VkPipelineStageFlags stageMask) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_RESET_EVENT);
}
void PostCallCmdWaitEvents(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t eventCount,
                           const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
                           uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount,
                           const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount,
                           const VkImageMemoryBarrier* pImageMemoryBarriers) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_WAIT_EVENTS);
}
void PostCallCmdPipelineBarrier(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask,
                                VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount,
                                const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount,
                                const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount,
                                const VkImageMemoryBarrier* pImageMemoryBarriers) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_PIPELINE_BARRIER);
}
void PostCallCmdBeginQuery(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query,
                           VkQueryControlFlags flags) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_BEGIN_QUERY);
}
void PostCallCmdEndQuery(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_END_QUERY);
}
void PostCallCmdResetQueryPool(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkQueryPool queryPool,
                               uint32_t firstQuery, uint32_t queryCount) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_RESET_QUERY_POOL);
}
void PostCallCmdWriteTimestamp(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage,
                               VkQueryPool queryPool, uint32_t query) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_WRITE_TIMESTAMP);
}
void PostCallCmdCopyQueryPoolResults(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkQueryPool queryPool,
                                     uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset,
                                     VkDeviceSize stride, VkQueryResultFlags flags) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_COPY_QUERY_POOL_RESULTS);
}
void PostCallCmdPushConstants(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkPipelineLayout layout,
                              VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_PUSH_CONSTANTS);
}
void PostCallCmdBeginRenderPass(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_BEGIN_RENDER_PASS);
}
void PostCallCmdNextSubpass(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkSubpassContents contents) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_NEXT_SUBPASS);
}
void PostCallCmdEndRenderPass(device_layer_data* device_data, VkCommandBuffer commandBuffer) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_END_RENDER_PASS);
}
void PostCallCmdExecuteCommands(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t commandBufferCount,
                                const VkCommandBuffer* pCommandBuffers) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_EXECUTE_COMMANDS);
}
void PostCallCmdPushDescriptorSetKHR(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                     VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set,
                                     uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_PUSH_DESCRIPTOR_SET_KHR);
}
void PostCallCmdPushDescriptorSetWithTemplateKHR(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                                 VkDescriptorUpdateTemplateKHR descriptorUpdateTemplate, VkPipelineLayout layout,
                                                 uint32_t set, const void* pData) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_PUSH_DESCRIPTOR_SET_WITH_TEMPLATE_KHR);
}
void PostCallCmdDebugMarkerBeginEXT(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                    VkDebugMarkerMarkerInfoEXT* pMarkerInfo) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DEBUG_MARKER_BEGIN_EXT);
}
void PostCallCmdDebugMarkerEndEXT(device_layer_data* device_data, VkCommandBuffer commandBuffer) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DEBUG_MARKER_END_EXT);
}
void PostCallCmdDebugMarkerInsertEXT(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                     VkDebugMarkerMarkerInfoEXT* pMarkerInfo) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DEBUG_MARKER_INSERT_EXT);
}
void PostCallCmdDrawIndirectCountAMD(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer,
                                     VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset,
                                     uint32_t maxDrawCount, uint32_t stride) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DRAW_INDIRECT_COUNT_AMD);
}
void PostCallCmdDrawIndexedIndirectCountAMD(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer,
                                            VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset,
                                            uint32_t maxDrawCount, uint32_t stride) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DRAW_INDEXED_INDIRECT_COUNT_AMD);
}
void PostCallCmdSetDeviceMaskKHX(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t deviceMask) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_DEVICE_MASK_KHX);
}
void PostCallCmdDispatchBaseKHX(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t baseGroupX,
                                uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY,
                                uint32_t groupCountZ) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_DISPATCH_BASE_KHX);
}
void PostCallCmdProcessCommandsNVX(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                   const VkCmdProcessCommandsInfoNVX* pProcessCommandsInfo) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_PROCESS_COMMANDS_NVX);
}
void PostCallCmdReserveSpaceForCommandsNVX(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                           const VkCmdReserveSpaceForCommandsInfoNVX* pReserveSpaceInfo) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_RESERVE_SPACE_FOR_COMMANDS_NVX);
}
void PostCallCmdSetViewportWScalingNV(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t firstViewport,
                                      uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_VIEWPORT_W_SCALING_NV);
}
void PostCallCmdSetDiscardRectangleEXT(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                       uint32_t firstDiscardRectangle, uint32_t discardRectangleCount,
                                       const VkRect2D* pDiscardRectangles) {
    state_tracker::AddCommand(device_data, commandBuffer, COMMAND_SET_DISCARD_RECTANGLE_EXT);
}
} // namespace vkaid
