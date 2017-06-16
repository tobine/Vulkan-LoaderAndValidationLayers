#ifndef __vkaid_dispatch_h_
#define __vkaid_dispatch_h_ 1

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
*/

#include "vulkaid_layer_data.h"

namespace vkaid {

void PostCallCreateDevice(device_layer_data* device_data, const VkDeviceCreateInfo* pCreateInfo,
                          const VkAllocationCallbacks* pAllocator, VkDevice* pDevice);

void PostCallDestroyDevice(device_layer_data* device_data, const VkAllocationCallbacks* pAllocator);

void PreCallQueuePresentKHR(device_layer_data* device_data, VkQueue queue, const VkPresentInfoKHR* pPresentInfo);

void PostCallAllocateCommandBuffers(
    device_layer_data*                          device_data,
    const VkCommandBufferAllocateInfo*          pAllocateInfo,
    VkCommandBuffer*                            pCommandBuffers);

void PostCallCmdBindPipeline(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkPipelineBindPoint pipelineBindPoint,
                             VkPipeline pipeline);
void PostCallCmdSetViewport(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t firstViewport,
                            uint32_t viewportCount, const VkViewport* pViewports);
void PostCallCmdSetScissor(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t firstScissor,
                           uint32_t scissorCount, const VkRect2D* pScissors);
void PostCallCmdSetLineWidth(device_layer_data* device_data, VkCommandBuffer commandBuffer, float lineWidth);
void PostCallCmdSetDepthBias(device_layer_data* device_data, VkCommandBuffer commandBuffer, float depthBiasConstantFactor,
                             float depthBiasClamp, float depthBiasSlopeFactor);
void PostCallCmdSetBlendConstants(device_layer_data* device_data, VkCommandBuffer commandBuffer, const float blendConstants[4]);
void PostCallCmdSetDepthBounds(device_layer_data* device_data, VkCommandBuffer commandBuffer, float minDepthBounds,
                               float maxDepthBounds);
void PostCallCmdSetStencilCompareMask(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask,
                                      uint32_t compareMask);
void PostCallCmdSetStencilWriteMask(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask,
                                    uint32_t writeMask);
void PostCallCmdSetStencilReference(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkStencilFaceFlags faceMask,
                                    uint32_t reference);
void PostCallCmdBindDescriptorSets(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                   VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t firstSet,
                                   uint32_t descriptorSetCount, const VkDescriptorSet* pDescriptorSets, uint32_t dynamicOffsetCount,
                                   const uint32_t* pDynamicOffsets);
void PostCallCmdBindIndexBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset,
                                VkIndexType indexType);
void PostCallCmdBindVertexBuffers(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t firstBinding,
                                  uint32_t bindingCount, const VkBuffer* pBuffers, const VkDeviceSize* pOffsets);
void PostCallCmdDraw(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t vertexCount, uint32_t instanceCount,
                     uint32_t firstVertex, uint32_t firstInstance);
void PostCallCmdDrawIndexed(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t indexCount,
                            uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance);
void PostCallCmdDrawIndirect(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer, VkDeviceSize offset,
                             uint32_t drawCount, uint32_t stride);
void PostCallCmdDrawIndexedIndirect(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer,
                                    VkDeviceSize offset, uint32_t drawCount, uint32_t stride);
void PostCallCmdDispatch(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t groupCountX, uint32_t groupCountY,
                         uint32_t groupCountZ);
void PostCallCmdDispatchIndirect(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer,
                                 VkDeviceSize offset);
void PostCallCmdCopyBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer srcBuffer, VkBuffer dstBuffer,
                           uint32_t regionCount, const VkBufferCopy* pRegions);
void PostCallCmdCopyImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage srcImage,
                          VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount,
                          const VkImageCopy* pRegions);
void PostCallCmdBlitImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage srcImage,
                          VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount,
                          const VkImageBlit* pRegions, VkFilter filter);
void PostCallCmdCopyBufferToImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer srcBuffer,
                                  VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount,
                                  const VkBufferImageCopy* pRegions);
void PostCallCmdCopyImageToBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage srcImage,
                                  VkImageLayout srcImageLayout, VkBuffer dstBuffer, uint32_t regionCount,
                                  const VkBufferImageCopy* pRegions);
void PostCallCmdUpdateBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer dstBuffer,
                             VkDeviceSize dstOffset, VkDeviceSize dataSize, const void* pData);
void PostCallCmdFillBuffer(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer dstBuffer,
                           VkDeviceSize dstOffset, VkDeviceSize size, uint32_t data);
void PostCallCmdClearColorImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage image,
                                VkImageLayout imageLayout, const VkClearColorValue* pColor, uint32_t rangeCount,
                                const VkImageSubresourceRange* pRanges);
void PostCallCmdClearDepthStencilImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage image,
                                       VkImageLayout imageLayout, const VkClearDepthStencilValue* pDepthStencil,
                                       uint32_t rangeCount, const VkImageSubresourceRange* pRanges);
void PostCallCmdClearAttachments(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t attachmentCount,
                                 const VkClearAttachment* pAttachments, uint32_t rectCount, const VkClearRect* pRects);
void PostCallCmdResolveImage(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkImage srcImage,
                             VkImageLayout srcImageLayout, VkImage dstImage, VkImageLayout dstImageLayout, uint32_t regionCount,
                             const VkImageResolve* pRegions);
void PostCallCmdSetEvent(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkEvent event,
                         VkPipelineStageFlags stageMask);
void PostCallCmdResetEvent(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkEvent event,
                           VkPipelineStageFlags stageMask);
void PostCallCmdWaitEvents(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t eventCount,
                           const VkEvent* pEvents, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask,
                           uint32_t memoryBarrierCount, const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount,
                           const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount,
                           const VkImageMemoryBarrier* pImageMemoryBarriers);
void PostCallCmdPipelineBarrier(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkPipelineStageFlags srcStageMask,
                                VkPipelineStageFlags dstStageMask, VkDependencyFlags dependencyFlags, uint32_t memoryBarrierCount,
                                const VkMemoryBarrier* pMemoryBarriers, uint32_t bufferMemoryBarrierCount,
                                const VkBufferMemoryBarrier* pBufferMemoryBarriers, uint32_t imageMemoryBarrierCount,
                                const VkImageMemoryBarrier* pImageMemoryBarriers);
void PostCallCmdBeginQuery(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query,
                           VkQueryControlFlags flags);
void PostCallCmdEndQuery(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkQueryPool queryPool, uint32_t query);
void PostCallCmdResetQueryPool(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkQueryPool queryPool,
                               uint32_t firstQuery, uint32_t queryCount);
void PostCallCmdWriteTimestamp(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkPipelineStageFlagBits pipelineStage,
                               VkQueryPool queryPool, uint32_t query);
void PostCallCmdCopyQueryPoolResults(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkQueryPool queryPool,
                                     uint32_t firstQuery, uint32_t queryCount, VkBuffer dstBuffer, VkDeviceSize dstOffset,
                                     VkDeviceSize stride, VkQueryResultFlags flags);
void PostCallCmdPushConstants(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkPipelineLayout layout,
                              VkShaderStageFlags stageFlags, uint32_t offset, uint32_t size, const void* pValues);
void PostCallCmdBeginRenderPass(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                const VkRenderPassBeginInfo* pRenderPassBegin, VkSubpassContents contents);
void PostCallCmdNextSubpass(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkSubpassContents contents);
void PostCallCmdEndRenderPass(device_layer_data* device_data, VkCommandBuffer commandBuffer);
void PostCallCmdExecuteCommands(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t commandBufferCount,
                                const VkCommandBuffer* pCommandBuffers);
void PostCallCmdPushDescriptorSetKHR(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                     VkPipelineBindPoint pipelineBindPoint, VkPipelineLayout layout, uint32_t set,
                                     uint32_t descriptorWriteCount, const VkWriteDescriptorSet* pDescriptorWrites);
void PostCallCmdPushDescriptorSetWithTemplateKHR(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                                 VkDescriptorUpdateTemplateKHR descriptorUpdateTemplate, VkPipelineLayout layout,
                                                 uint32_t set, const void* pData);
void PostCallCmdDebugMarkerBeginEXT(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                    VkDebugMarkerMarkerInfoEXT* pMarkerInfo);
void PostCallCmdDebugMarkerEndEXT(device_layer_data* device_data, VkCommandBuffer commandBuffer);
void PostCallCmdDebugMarkerInsertEXT(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                     VkDebugMarkerMarkerInfoEXT* pMarkerInfo);
void PostCallCmdDrawIndirectCountAMD(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer,
                                     VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset,
                                     uint32_t maxDrawCount, uint32_t stride);
void PostCallCmdDrawIndexedIndirectCountAMD(device_layer_data* device_data, VkCommandBuffer commandBuffer, VkBuffer buffer,
                                            VkDeviceSize offset, VkBuffer countBuffer, VkDeviceSize countBufferOffset,
                                            uint32_t maxDrawCount, uint32_t stride);
void PostCallCmdSetDeviceMaskKHX(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t deviceMask);
void PostCallCmdDispatchBaseKHX(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t baseGroupX,
                                uint32_t baseGroupY, uint32_t baseGroupZ, uint32_t groupCountX, uint32_t groupCountY,
                                uint32_t groupCountZ);
void PostCallCmdProcessCommandsNVX(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                   const VkCmdProcessCommandsInfoNVX* pProcessCommandsInfo);
void PostCallCmdReserveSpaceForCommandsNVX(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                           const VkCmdReserveSpaceForCommandsInfoNVX* pReserveSpaceInfo);
void PostCallCmdSetViewportWScalingNV(device_layer_data* device_data, VkCommandBuffer commandBuffer, uint32_t firstViewport,
                                      uint32_t viewportCount, const VkViewportWScalingNV* pViewportWScalings);
void PostCallCmdSetDiscardRectangleEXT(device_layer_data* device_data, VkCommandBuffer commandBuffer,
                                       uint32_t firstDiscardRectangle, uint32_t discardRectangleCount,
                                       const VkRect2D* pDiscardRectangles);

}  // namespace vkaid
#endif  // #define __vkaid_dispatch_h_ 1