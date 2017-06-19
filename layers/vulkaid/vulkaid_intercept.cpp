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

#include "vulkaid_intercept.h"
#include "vulkaid_dispatch.h"
#include "vk_dispatch_table_helper.h"
#include "vk_layer_extension_utils.h"
#include "vk_extension_helper.h"
#include "vk_layer_data.h"
#include "vk_layer_utils.h"
#include <vector>

namespace vkaid {

static uint32_t loader_layer_if_version = CURRENT_LOADER_LAYER_INTERFACE_VERSION;

VKAPI_ATTR VkResult VKAPI_CALL CreateInstance(
    const VkInstanceCreateInfo*                 pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkInstance*                                 pInstance)
{
    VkLayerInstanceCreateInfo *chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);

    assert(chain_info->u.pLayerInfo);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    PFN_vkCreateInstance fpCreateInstance = (PFN_vkCreateInstance)fpGetInstanceProcAddr(NULL, "vkCreateInstance");
    if (fpCreateInstance == NULL) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    // Advance the link info for the next element on the chain
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;

    VkResult result = fpCreateInstance(pCreateInfo, pAllocator, pInstance);
    if (result != VK_SUCCESS) return result;

    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(*pInstance), instance_layer_data_map);
    instance_data->instance = *pInstance;
    layer_init_instance_dispatch_table(*pInstance, &instance_data->dispatch_table, fpGetInstanceProcAddr);

    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyInstance(
    VkInstance                                  instance,
    const VkAllocationCallbacks*                pAllocator)
{
    auto key = get_dispatch_key(instance);
    instance_layer_data *instance_data = GetLayerDataPtr(key, instance_layer_data_map);
    instance_data->dispatch_table.DestroyInstance(instance, pAllocator);

    instance_layer_data_map.erase(key);
}

VKAPI_ATTR VkResult VKAPI_CALL EnumeratePhysicalDevices(
    VkInstance                                  instance,
    uint32_t*                                   pPhysicalDeviceCount,
    VkPhysicalDevice*                           pPhysicalDevices)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    PreCallEnumeratePhysicalDevices(instance_data, instance, pPhysicalDeviceCount, pPhysicalDevices);
    VkResult result = instance_data->dispatch_table.EnumeratePhysicalDevices(instance, pPhysicalDeviceCount, pPhysicalDevices);
    PostCallEnumeratePhysicalDevices(instance_data, instance, pPhysicalDeviceCount, pPhysicalDevices);
    return result;
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceFeatures(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceFeatures*                   pFeatures)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceFeatures(instance_data, physicalDevice, pFeatures);
    instance_data->dispatch_table.GetPhysicalDeviceFeatures(physicalDevice, pFeatures);
    PostCallGetPhysicalDeviceFeatures(instance_data, physicalDevice, pFeatures);
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceFormatProperties(
    VkPhysicalDevice                            physicalDevice,
    VkFormat                                    format,
    VkFormatProperties*                         pFormatProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceFormatProperties(instance_data, physicalDevice, format, pFormatProperties);
    instance_data->dispatch_table.GetPhysicalDeviceFormatProperties(physicalDevice, format, pFormatProperties);
    PostCallGetPhysicalDeviceFormatProperties(instance_data, physicalDevice, format, pFormatProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceImageFormatProperties(
    VkPhysicalDevice                            physicalDevice,
    VkFormat                                    format,
    VkImageType                                 type,
    VkImageTiling                               tiling,
    VkImageUsageFlags                           usage,
    VkImageCreateFlags                          flags,
    VkImageFormatProperties*                    pImageFormatProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceImageFormatProperties(instance_data, physicalDevice, format, type, tiling, usage, flags,
                                                  pImageFormatProperties);
    VkResult result = instance_data->dispatch_table.GetPhysicalDeviceImageFormatProperties(physicalDevice, format, type, tiling,
                                                                                           usage, flags, pImageFormatProperties);
    PostCallGetPhysicalDeviceImageFormatProperties(instance_data, physicalDevice, format, type, tiling, usage, flags,
                                                   pImageFormatProperties);
    return result;
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceProperties(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceProperties*                 pProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceProperties(instance_data, physicalDevice, pProperties);
    instance_data->dispatch_table.GetPhysicalDeviceProperties(physicalDevice, pProperties);
    PostCallGetPhysicalDeviceProperties(instance_data, physicalDevice, pProperties);
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceQueueFamilyProperties(
    VkPhysicalDevice                            physicalDevice,
    uint32_t*                                   pQueueFamilyPropertyCount,
    VkQueueFamilyProperties*                    pQueueFamilyProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceQueueFamilyProperties(instance_data, physicalDevice, pQueueFamilyPropertyCount, pQueueFamilyProperties);
    instance_data->dispatch_table.GetPhysicalDeviceQueueFamilyProperties(physicalDevice, pQueueFamilyPropertyCount,
                                                                         pQueueFamilyProperties);
    PostCallGetPhysicalDeviceQueueFamilyProperties(instance_data, physicalDevice, pQueueFamilyPropertyCount,
                                                   pQueueFamilyProperties);
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceMemoryProperties(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceMemoryProperties*           pMemoryProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceMemoryProperties(instance_data, physicalDevice, pMemoryProperties);
    instance_data->dispatch_table.GetPhysicalDeviceMemoryProperties(physicalDevice, pMemoryProperties);
    PostCallGetPhysicalDeviceMemoryProperties(instance_data, physicalDevice, pMemoryProperties);
}

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL GetDeviceProcAddr(
    VkDevice                                    device,
    const char*                                 pName)
{
    const auto item = name_to_funcptr_map.find(pName);
    if (item != name_to_funcptr_map.end()) {
        return reinterpret_cast<PFN_vkVoidFunction>(item->second);
    }

    auto device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    const auto &table = device_data->dispatch_table;
    if (!table.GetDeviceProcAddr)
        return nullptr;
    return table.GetDeviceProcAddr(device, pName);
}

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL GetInstanceProcAddr(
    VkInstance                                  instance,
    const char*                                 pName)
{
    const auto item = name_to_funcptr_map.find(pName);
    if (item != name_to_funcptr_map.end()) {
        return reinterpret_cast<PFN_vkVoidFunction>(item->second);
    }
    auto instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    const auto &table = instance_data->dispatch_table;
    if (!table.GetInstanceProcAddr)
        return nullptr;
    return table.GetInstanceProcAddr(instance, pName);
}

VKAPI_ATTR VkResult VKAPI_CALL CreateDevice(
    VkPhysicalDevice                            physicalDevice,
    const VkDeviceCreateInfo*                   pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDevice*                                   pDevice)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);

    VkLayerDeviceCreateInfo *chain_info = get_chain_info(pCreateInfo, VK_LAYER_LINK_INFO);

    assert(chain_info->u.pLayerInfo);
    PFN_vkGetInstanceProcAddr fpGetInstanceProcAddr = chain_info->u.pLayerInfo->pfnNextGetInstanceProcAddr;
    PFN_vkGetDeviceProcAddr fpGetDeviceProcAddr = chain_info->u.pLayerInfo->pfnNextGetDeviceProcAddr;
    PFN_vkCreateDevice fpCreateDevice = (PFN_vkCreateDevice)fpGetInstanceProcAddr(instance_data->instance, "vkCreateDevice");
    if (fpCreateDevice == NULL) {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    // Advance the link info for the next element on the chain
    chain_info->u.pLayerInfo = chain_info->u.pLayerInfo->pNext;

    VkResult result = fpCreateDevice(physicalDevice, pCreateInfo, pAllocator, pDevice);
    if (result != VK_SUCCESS) {
        return result;
    }

    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(*pDevice), device_layer_data_map);

    device_data->instance_data = instance_data;
    // Setup device dispatch table
    layer_init_device_dispatch_table(*pDevice, &device_data->dispatch_table, fpGetDeviceProcAddr);
    device_data->device = *pDevice;
    // Save PhysicalDevice handle
    device_data->physical_device = physicalDevice;

    device_data->extensions.InitFromDeviceCreateInfo(&instance_data->extensions, pCreateInfo);

    // Get physical device limits for this device
    instance_data->dispatch_table.GetPhysicalDeviceProperties(physicalDevice, &(device_data->physical_device_properties));
    uint32_t count;
    instance_data->dispatch_table.GetPhysicalDeviceQueueFamilyProperties(physicalDevice, &count, nullptr);
    device_data->queue_family_properties.resize(count);
    instance_data->dispatch_table.GetPhysicalDeviceQueueFamilyProperties(
        physicalDevice, &count, &device_data->queue_family_properties[0]);
    if (pCreateInfo->pEnabledFeatures) {
        device_data->enabled_features = *pCreateInfo->pEnabledFeatures;
    }
    // Store physical device properties and physical device mem limits into device layer_data structs
    instance_data->dispatch_table.GetPhysicalDeviceMemoryProperties(physicalDevice, &device_data->physical_device_mem_properties);
    instance_data->dispatch_table.GetPhysicalDeviceProperties(physicalDevice, &device_data->physical_device_properties);
    // Call dispatch layer for device-level setup
    PostCallCreateDevice(device_data, pCreateInfo, pAllocator, pDevice);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyDevice(
    VkDevice                                    device,
    const VkAllocationCallbacks*                pAllocator)
{
    dispatch_key key = get_dispatch_key(device);
    device_layer_data *device_data = GetLayerDataPtr(key, device_layer_data_map);

    device_data->dispatch_table.DestroyDevice(device, pAllocator);
    device_layer_data_map.erase(key);
}

VKAPI_ATTR VkResult VKAPI_CALL EnumerateInstanceExtensionProperties(
    const char*                                 pLayerName,
    uint32_t*                                   pPropertyCount,
    VkExtensionProperties*                      pProperties)
{
    // TODO: Just hacking in success for now
    return VK_SUCCESS;
}

static const VkLayerProperties vulkaid_layer_properties = {
    "VK_LAYER_GOOGLE_vulkaid", VK_LAYER_API_VERSION, 1, "Vulkan Aid (VulkAid) API profiling/characterization labels",
};

VKAPI_ATTR VkResult VKAPI_CALL EnumerateInstanceLayerProperties(uint32_t *pCount, VkLayerProperties *pProperties) {
    return util_GetLayerProperties(1, &vulkaid_layer_properties, pCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL EnumerateDeviceLayerProperties(VkPhysicalDevice physicalDevice, uint32_t *pCount,
                                                              VkLayerProperties *pProperties) {
    return util_GetLayerProperties(1, &vulkaid_layer_properties, pCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL EnumerateDeviceExtensionProperties(
    VkPhysicalDevice                            physicalDevice,
    const char*                                 pLayerName,
    uint32_t*                                   pPropertyCount,
    VkExtensionProperties*                      pProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkEnumerateDeviceExtensionProperties();
    VkResult result = instance_data->dispatch_table.EnumerateDeviceExtensionProperties(physicalDevice,pLayerName,pPropertyCount,pProperties);
    //PostCallvkEnumerateDeviceExtensionProperties();
    return result;
}

VKAPI_ATTR void VKAPI_CALL GetDeviceQueue(
    VkDevice                                    device,
    uint32_t                                    queueFamilyIndex,
    uint32_t                                    queueIndex,
    VkQueue*                                    pQueue)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetDeviceQueue(device_data, device, queueFamilyIndex, queueIndex, pQueue);
    device_data->dispatch_table.GetDeviceQueue(device, queueFamilyIndex, queueIndex, pQueue);
    PostCallGetDeviceQueue(device_data, device, queueFamilyIndex, queueIndex, pQueue);
}

VKAPI_ATTR VkResult VKAPI_CALL QueueSubmit(
    VkQueue                                     queue,
    uint32_t                                    submitCount,
    const VkSubmitInfo*                         pSubmits,
    VkFence                                     fence)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(queue), device_layer_data_map);
    PreCallQueueSubmit(device_data, queue, submitCount, pSubmits, fence);
    VkResult result = device_data->dispatch_table.QueueSubmit(queue, submitCount, pSubmits, fence);
    PostCallQueueSubmit(device_data, queue, submitCount, pSubmits, fence);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL QueueWaitIdle(
    VkQueue                                     queue)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(queue), device_layer_data_map);
    PreCallQueueWaitIdle(device_data, queue);
    VkResult result = device_data->dispatch_table.QueueWaitIdle(queue);
    PostCallQueueWaitIdle(device_data, queue);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL DeviceWaitIdle(
    VkDevice                                    device)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDeviceWaitIdle(device_data, device);
    VkResult result = device_data->dispatch_table.DeviceWaitIdle(device);
    PostCallDeviceWaitIdle(device_data, device);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL AllocateMemory(
    VkDevice                                    device,
    const VkMemoryAllocateInfo*                 pAllocateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDeviceMemory*                             pMemory)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallAllocateMemory(device_data, device, pAllocateInfo, pAllocator, pMemory);
    VkResult result = device_data->dispatch_table.AllocateMemory(device, pAllocateInfo, pAllocator, pMemory);
    PostCallAllocateMemory(device_data, device, pAllocateInfo, pAllocator, pMemory);
    return result;
}

VKAPI_ATTR void VKAPI_CALL FreeMemory(
    VkDevice                                    device,
    VkDeviceMemory                              memory,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallFreeMemory(device_data, device, memory, pAllocator);
    device_data->dispatch_table.FreeMemory(device, memory, pAllocator);
    PostCallFreeMemory(device_data, device, memory, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL MapMemory(
    VkDevice                                    device,
    VkDeviceMemory                              memory,
    VkDeviceSize                                offset,
    VkDeviceSize                                size,
    VkMemoryMapFlags                            flags,
    void**                                      ppData)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallMapMemory(device_data, device, memory, offset, size, flags, ppData);
    VkResult result = device_data->dispatch_table.MapMemory(device, memory, offset, size, flags, ppData);
    PostCallMapMemory(device_data, device, memory, offset, size, flags, ppData);
    return result;
}

VKAPI_ATTR void VKAPI_CALL UnmapMemory(
    VkDevice                                    device,
    VkDeviceMemory                              memory)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallUnmapMemory(device_data, device, memory);
    device_data->dispatch_table.UnmapMemory(device, memory);
    PostCallUnmapMemory(device_data, device, memory);
}

VKAPI_ATTR VkResult VKAPI_CALL FlushMappedMemoryRanges(
    VkDevice                                    device,
    uint32_t                                    memoryRangeCount,
    const VkMappedMemoryRange*                  pMemoryRanges)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallFlushMappedMemoryRanges(device_data, device, memoryRangeCount, pMemoryRanges);
    VkResult result = device_data->dispatch_table.FlushMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
    PostCallFlushMappedMemoryRanges(device_data, device, memoryRangeCount, pMemoryRanges);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL InvalidateMappedMemoryRanges(
    VkDevice                                    device,
    uint32_t                                    memoryRangeCount,
    const VkMappedMemoryRange*                  pMemoryRanges)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallInvalidateMappedMemoryRanges(device_data, device, memoryRangeCount, pMemoryRanges);
    VkResult result = device_data->dispatch_table.InvalidateMappedMemoryRanges(device, memoryRangeCount, pMemoryRanges);
    PostCallInvalidateMappedMemoryRanges(device_data, device, memoryRangeCount, pMemoryRanges);
    return result;
}

VKAPI_ATTR void VKAPI_CALL GetDeviceMemoryCommitment(
    VkDevice                                    device,
    VkDeviceMemory                              memory,
    VkDeviceSize*                               pCommittedMemoryInBytes)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetDeviceMemoryCommitment(device_data, device, memory, pCommittedMemoryInBytes);
    device_data->dispatch_table.GetDeviceMemoryCommitment(device, memory, pCommittedMemoryInBytes);
    PostCallGetDeviceMemoryCommitment(device_data, device, memory, pCommittedMemoryInBytes);
}

VKAPI_ATTR VkResult VKAPI_CALL BindBufferMemory(
    VkDevice                                    device,
    VkBuffer                                    buffer,
    VkDeviceMemory                              memory,
    VkDeviceSize                                memoryOffset)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallBindBufferMemory(device_data, device, buffer, memory, memoryOffset);
    VkResult result = device_data->dispatch_table.BindBufferMemory(device, buffer, memory, memoryOffset);
    PostCallBindBufferMemory(device_data, device, buffer, memory, memoryOffset);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL BindImageMemory(
    VkDevice                                    device,
    VkImage                                     image,
    VkDeviceMemory                              memory,
    VkDeviceSize                                memoryOffset)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallBindImageMemory(device_data, device, image, memory, memoryOffset);
    VkResult result = device_data->dispatch_table.BindImageMemory(device, image, memory, memoryOffset);
    PostCallBindImageMemory(device_data, device, image, memory, memoryOffset);
    return result;
}

VKAPI_ATTR void VKAPI_CALL GetBufferMemoryRequirements(
    VkDevice                                    device,
    VkBuffer                                    buffer,
    VkMemoryRequirements*                       pMemoryRequirements)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetBufferMemoryRequirements(device_data, device, buffer, pMemoryRequirements);
    device_data->dispatch_table.GetBufferMemoryRequirements(device, buffer, pMemoryRequirements);
    PostCallGetBufferMemoryRequirements(device_data, device, buffer, pMemoryRequirements);
}

VKAPI_ATTR void VKAPI_CALL GetImageMemoryRequirements(
    VkDevice                                    device,
    VkImage                                     image,
    VkMemoryRequirements*                       pMemoryRequirements)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetImageMemoryRequirements(device_data, device, image, pMemoryRequirements);
    device_data->dispatch_table.GetImageMemoryRequirements(device, image, pMemoryRequirements);
    PostCallGetImageMemoryRequirements(device_data, device, image, pMemoryRequirements);
}

VKAPI_ATTR void VKAPI_CALL GetImageSparseMemoryRequirements(
    VkDevice                                    device,
    VkImage                                     image,
    uint32_t*                                   pSparseMemoryRequirementCount,
    VkSparseImageMemoryRequirements*            pSparseMemoryRequirements)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetImageSparseMemoryRequirements(device_data, device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
    device_data->dispatch_table.GetImageSparseMemoryRequirements(device, image, pSparseMemoryRequirementCount,
                                                                 pSparseMemoryRequirements);
    PostCallGetImageSparseMemoryRequirements(device_data, device, image, pSparseMemoryRequirementCount, pSparseMemoryRequirements);
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceSparseImageFormatProperties(
    VkPhysicalDevice                            physicalDevice,
    VkFormat                                    format,
    VkImageType                                 type,
    VkSampleCountFlagBits                       samples,
    VkImageUsageFlags                           usage,
    VkImageTiling                               tiling,
    uint32_t*                                   pPropertyCount,
    VkSparseImageFormatProperties*              pProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceSparseImageFormatProperties(instance_data, physicalDevice, format, type, samples, usage, tiling,
                                                        pPropertyCount, pProperties);
    instance_data->dispatch_table.GetPhysicalDeviceSparseImageFormatProperties(physicalDevice, format, type, samples, usage, tiling,
                                                                               pPropertyCount, pProperties);
    PostCallGetPhysicalDeviceSparseImageFormatProperties(instance_data, physicalDevice, format, type, samples, usage, tiling,
                                                         pPropertyCount, pProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL QueueBindSparse(
    VkQueue                                     queue,
    uint32_t                                    bindInfoCount,
    const VkBindSparseInfo*                     pBindInfo,
    VkFence                                     fence)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(queue), device_layer_data_map);
    PreCallQueueBindSparse(device_data, queue, bindInfoCount, pBindInfo, fence);
    VkResult result = device_data->dispatch_table.QueueBindSparse(queue, bindInfoCount, pBindInfo, fence);
    PostCallQueueBindSparse(device_data, queue, bindInfoCount, pBindInfo, fence);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL CreateFence(
    VkDevice                                    device,
    const VkFenceCreateInfo*                    pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkFence*                                    pFence)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateFence(device_data, device, pCreateInfo, pAllocator, pFence);
    VkResult result = device_data->dispatch_table.CreateFence(device, pCreateInfo, pAllocator, pFence);
    PostCallCreateFence(device_data, device, pCreateInfo, pAllocator, pFence);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyFence(
    VkDevice                                    device,
    VkFence                                     fence,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroyFence(device_data, device, fence, pAllocator);
    device_data->dispatch_table.DestroyFence(device, fence, pAllocator);
    PostCallDestroyFence(device_data, device, fence, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL ResetFences(
    VkDevice                                    device,
    uint32_t                                    fenceCount,
    const VkFence*                              pFences)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallResetFences(device_data, device, fenceCount, pFences);
    VkResult result = device_data->dispatch_table.ResetFences(device, fenceCount, pFences);
    PostCallResetFences(device_data, device, fenceCount, pFences);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetFenceStatus(
    VkDevice                                    device,
    VkFence                                     fence)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetFenceStatus(device_data, device, fence);
    VkResult result = device_data->dispatch_table.GetFenceStatus(device, fence);
    PostCallGetFenceStatus(device_data, device, fence);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL WaitForFences(
    VkDevice                                    device,
    uint32_t                                    fenceCount,
    const VkFence*                              pFences,
    VkBool32                                    waitAll,
    uint64_t                                    timeout)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallWaitForFences(device_data, device, fenceCount, pFences, waitAll, timeout);
    VkResult result = device_data->dispatch_table.WaitForFences(device, fenceCount, pFences, waitAll, timeout);
    PostCallWaitForFences(device_data, device, fenceCount, pFences, waitAll, timeout);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL CreateSemaphore(
    VkDevice                                    device,
    const VkSemaphoreCreateInfo*                pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSemaphore*                                pSemaphore)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateSemaphore(device_data, device, pCreateInfo, pAllocator, pSemaphore);
    VkResult result = device_data->dispatch_table.CreateSemaphore(device, pCreateInfo, pAllocator, pSemaphore);
    PostCallCreateSemaphore(device_data, device, pCreateInfo, pAllocator, pSemaphore);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroySemaphore(
    VkDevice                                    device,
    VkSemaphore                                 semaphore,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroySemaphore(device_data, device, semaphore, pAllocator);
    device_data->dispatch_table.DestroySemaphore(device, semaphore, pAllocator);
    PostCallDestroySemaphore(device_data, device, semaphore, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL CreateEvent(
    VkDevice                                    device,
    const VkEventCreateInfo*                    pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkEvent*                                    pEvent)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateEvent(device_data, device, pCreateInfo, pAllocator, pEvent);
    VkResult result = device_data->dispatch_table.CreateEvent(device, pCreateInfo, pAllocator, pEvent);
    PostCallCreateEvent(device_data, device, pCreateInfo, pAllocator, pEvent);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyEvent(
    VkDevice                                    device,
    VkEvent                                     event,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroyEvent(device_data, device, event, pAllocator);
    device_data->dispatch_table.DestroyEvent(device, event, pAllocator);
    PostCallDestroyEvent(device_data, device, event, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL GetEventStatus(
    VkDevice                                    device,
    VkEvent                                     event)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetEventStatus(device_data, device, event);
    VkResult result = device_data->dispatch_table.GetEventStatus(device, event);
    PostCallGetEventStatus(device_data, device, event);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL SetEvent(
    VkDevice                                    device,
    VkEvent                                     event)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallSetEvent(device_data, device, event);
    VkResult result = device_data->dispatch_table.SetEvent(device, event);
    PostCallSetEvent(device_data, device, event);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL ResetEvent(
    VkDevice                                    device,
    VkEvent                                     event)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallResetEvent(device_data, device, event);
    VkResult result = device_data->dispatch_table.ResetEvent(device, event);
    PostCallResetEvent(device_data, device, event);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL CreateQueryPool(
    VkDevice                                    device,
    const VkQueryPoolCreateInfo*                pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkQueryPool*                                pQueryPool)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateQueryPool(device_data, device, pCreateInfo, pAllocator, pQueryPool);
    VkResult result = device_data->dispatch_table.CreateQueryPool(device, pCreateInfo, pAllocator, pQueryPool);
    PostCallCreateQueryPool(device_data, device, pCreateInfo, pAllocator, pQueryPool);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyQueryPool(
    VkDevice                                    device,
    VkQueryPool                                 queryPool,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroyQueryPool(device_data, device, queryPool, pAllocator);
    device_data->dispatch_table.DestroyQueryPool(device, queryPool, pAllocator);
    PostCallDestroyQueryPool(device_data, device, queryPool, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL GetQueryPoolResults(
    VkDevice                                    device,
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery,
    uint32_t                                    queryCount,
    size_t                                      dataSize,
    void*                                       pData,
    VkDeviceSize                                stride,
    VkQueryResultFlags                          flags)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetQueryPoolResults(device_data, device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
    VkResult result =
        device_data->dispatch_table.GetQueryPoolResults(device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
    PostCallGetQueryPoolResults(device_data, device, queryPool, firstQuery, queryCount, dataSize, pData, stride, flags);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL CreateBuffer(
    VkDevice                                    device,
    const VkBufferCreateInfo*                   pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkBuffer*                                   pBuffer)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateBuffer(device_data, device, pCreateInfo, pAllocator, pBuffer);
    VkResult result = device_data->dispatch_table.CreateBuffer(device, pCreateInfo, pAllocator, pBuffer);
    PostCallCreateBuffer(device_data, device, pCreateInfo, pAllocator, pBuffer);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyBuffer(
    VkDevice                                    device,
    VkBuffer                                    buffer,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroyBuffer(device_data, device, buffer, pAllocator);
    device_data->dispatch_table.DestroyBuffer(device, buffer, pAllocator);
    PostCallDestroyBuffer(device_data, device, buffer, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL CreateBufferView(
    VkDevice                                    device,
    const VkBufferViewCreateInfo*               pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkBufferView*                               pView)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateBufferView(device_data, device, pCreateInfo, pAllocator, pView);
    VkResult result = device_data->dispatch_table.CreateBufferView(device, pCreateInfo, pAllocator, pView);
    PostCallCreateBufferView(device_data, device, pCreateInfo, pAllocator, pView);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyBufferView(
    VkDevice                                    device,
    VkBufferView                                bufferView,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroyBufferView(device_data, device, bufferView, pAllocator);
    device_data->dispatch_table.DestroyBufferView(device, bufferView, pAllocator);
    PostCallDestroyBufferView(device_data, device, bufferView, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL CreateImage(
    VkDevice                                    device,
    const VkImageCreateInfo*                    pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkImage*                                    pImage)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateImage(device_data, device, pCreateInfo, pAllocator, pImage);
    VkResult result = device_data->dispatch_table.CreateImage(device, pCreateInfo, pAllocator, pImage);
    PostCallCreateImage(device_data, device, pCreateInfo, pAllocator, pImage);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyImage(
    VkDevice                                    device,
    VkImage                                     image,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroyImage(device_data, device, image, pAllocator);
    device_data->dispatch_table.DestroyImage(device, image, pAllocator);
    PostCallDestroyImage(device_data, device, image, pAllocator);
}

VKAPI_ATTR void VKAPI_CALL GetImageSubresourceLayout(
    VkDevice                                    device,
    VkImage                                     image,
    const VkImageSubresource*                   pSubresource,
    VkSubresourceLayout*                        pLayout)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetImageSubresourceLayout(device_data, device, image, pSubresource, pLayout);
    device_data->dispatch_table.GetImageSubresourceLayout(device, image, pSubresource, pLayout);
    PostCallGetImageSubresourceLayout(device_data, device, image, pSubresource, pLayout);
}

VKAPI_ATTR VkResult VKAPI_CALL CreateImageView(
    VkDevice                                    device,
    const VkImageViewCreateInfo*                pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkImageView*                                pView)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateImageView(device_data, device, pCreateInfo, pAllocator, pView);
    VkResult result = device_data->dispatch_table.CreateImageView(device, pCreateInfo, pAllocator, pView);
    PostCallCreateImageView(device_data, device, pCreateInfo, pAllocator, pView);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyImageView(
    VkDevice                                    device,
    VkImageView                                 imageView,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroyImageView(device_data, device, imageView, pAllocator);
    device_data->dispatch_table.DestroyImageView(device, imageView, pAllocator);
    PostCallDestroyImageView(device_data, device, imageView, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL CreateShaderModule(
    VkDevice                                    device,
    const VkShaderModuleCreateInfo*             pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkShaderModule*                             pShaderModule)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateShaderModule(device_data, device, pCreateInfo, pAllocator, pShaderModule);
    VkResult result = device_data->dispatch_table.CreateShaderModule(device, pCreateInfo, pAllocator, pShaderModule);
    PostCallCreateShaderModule(device_data, device, pCreateInfo, pAllocator, pShaderModule);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyShaderModule(
    VkDevice                                    device,
    VkShaderModule                              shaderModule,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroyShaderModule(device_data, device, shaderModule, pAllocator);
    device_data->dispatch_table.DestroyShaderModule(device, shaderModule, pAllocator);
    PostCallDestroyShaderModule(device_data, device, shaderModule, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL CreatePipelineCache(
    VkDevice                                    device,
    const VkPipelineCacheCreateInfo*            pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkPipelineCache*                            pPipelineCache)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreatePipelineCache(device_data, device, pCreateInfo, pAllocator, pPipelineCache);
    VkResult result = device_data->dispatch_table.CreatePipelineCache(device, pCreateInfo, pAllocator, pPipelineCache);
    PostCallCreatePipelineCache(device_data, device, pCreateInfo, pAllocator, pPipelineCache);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyPipelineCache(
    VkDevice                                    device,
    VkPipelineCache                             pipelineCache,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroyPipelineCache(device_data, device, pipelineCache, pAllocator);
    device_data->dispatch_table.DestroyPipelineCache(device, pipelineCache, pAllocator);
    PostCallDestroyPipelineCache(device_data, device, pipelineCache, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL GetPipelineCacheData(
    VkDevice                                    device,
    VkPipelineCache                             pipelineCache,
    size_t*                                     pDataSize,
    void*                                       pData)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetPipelineCacheData(device_data, device, pipelineCache, pDataSize, pData);
    VkResult result = device_data->dispatch_table.GetPipelineCacheData(device, pipelineCache, pDataSize, pData);
    PostCallGetPipelineCacheData(device_data, device, pipelineCache, pDataSize, pData);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL MergePipelineCaches(
    VkDevice                                    device,
    VkPipelineCache                             dstCache,
    uint32_t                                    srcCacheCount,
    const VkPipelineCache*                      pSrcCaches)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallMergePipelineCaches(device_data, device, dstCache, srcCacheCount, pSrcCaches);
    VkResult result = device_data->dispatch_table.MergePipelineCaches(device, dstCache, srcCacheCount, pSrcCaches);
    PostCallMergePipelineCaches(device_data, device, dstCache, srcCacheCount, pSrcCaches);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL CreateGraphicsPipelines(
    VkDevice                                    device,
    VkPipelineCache                             pipelineCache,
    uint32_t                                    createInfoCount,
    const VkGraphicsPipelineCreateInfo*         pCreateInfos,
    const VkAllocationCallbacks*                pAllocator,
    VkPipeline*                                 pPipelines)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateGraphicsPipelines(device_data, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
    VkResult result = device_data->dispatch_table.CreateGraphicsPipelines(device, pipelineCache, createInfoCount, pCreateInfos,
                                                                          pAllocator, pPipelines);
    PostCallCreateGraphicsPipelines(device_data, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL CreateComputePipelines(
    VkDevice                                    device,
    VkPipelineCache                             pipelineCache,
    uint32_t                                    createInfoCount,
    const VkComputePipelineCreateInfo*          pCreateInfos,
    const VkAllocationCallbacks*                pAllocator,
    VkPipeline*                                 pPipelines)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateComputePipelines(device_data, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
    VkResult result = device_data->dispatch_table.CreateComputePipelines(device, pipelineCache, createInfoCount, pCreateInfos,
                                                                         pAllocator, pPipelines);
    PostCallCreateComputePipelines(device_data, device, pipelineCache, createInfoCount, pCreateInfos, pAllocator, pPipelines);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyPipeline(
    VkDevice                                    device,
    VkPipeline                                  pipeline,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroyPipeline(device_data, device, pipeline, pAllocator);
    device_data->dispatch_table.DestroyPipeline(device, pipeline, pAllocator);
    PostCallDestroyPipeline(device_data, device, pipeline, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL CreatePipelineLayout(
    VkDevice                                    device,
    const VkPipelineLayoutCreateInfo*           pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkPipelineLayout*                           pPipelineLayout)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreatePipelineLayout(device_data, device, pCreateInfo, pAllocator, pPipelineLayout);
    VkResult result = device_data->dispatch_table.CreatePipelineLayout(device, pCreateInfo, pAllocator, pPipelineLayout);
    PostCallCreatePipelineLayout(device_data, device, pCreateInfo, pAllocator, pPipelineLayout);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyPipelineLayout(
    VkDevice                                    device,
    VkPipelineLayout                            pipelineLayout,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroyPipelineLayout(device_data, device, pipelineLayout, pAllocator);
    device_data->dispatch_table.DestroyPipelineLayout(device, pipelineLayout, pAllocator);
    PostCallDestroyPipelineLayout(device_data, device, pipelineLayout, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL CreateSampler(
    VkDevice                                    device,
    const VkSamplerCreateInfo*                  pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSampler*                                  pSampler)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateSampler(device_data, device, pCreateInfo, pAllocator, pSampler);
    VkResult result = device_data->dispatch_table.CreateSampler(device, pCreateInfo, pAllocator, pSampler);
    PostCallCreateSampler(device_data, device, pCreateInfo, pAllocator, pSampler);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroySampler(
    VkDevice                                    device,
    VkSampler                                   sampler,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroySampler(device_data, device, sampler, pAllocator);
    device_data->dispatch_table.DestroySampler(device, sampler, pAllocator);
    PostCallDestroySampler(device_data, device, sampler, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL CreateDescriptorSetLayout(
    VkDevice                                    device,
    const VkDescriptorSetLayoutCreateInfo*      pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDescriptorSetLayout*                      pSetLayout)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateDescriptorSetLayout(device_data, device, pCreateInfo, pAllocator, pSetLayout);
    VkResult result = device_data->dispatch_table.CreateDescriptorSetLayout(device, pCreateInfo, pAllocator, pSetLayout);
    PostCallCreateDescriptorSetLayout(device_data, device, pCreateInfo, pAllocator, pSetLayout);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyDescriptorSetLayout(
    VkDevice                                    device,
    VkDescriptorSetLayout                       descriptorSetLayout,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroyDescriptorSetLayout(device_data, device, descriptorSetLayout, pAllocator);
    device_data->dispatch_table.DestroyDescriptorSetLayout(device, descriptorSetLayout, pAllocator);
    PostCallDestroyDescriptorSetLayout(device_data, device, descriptorSetLayout, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL CreateDescriptorPool(
    VkDevice                                    device,
    const VkDescriptorPoolCreateInfo*           pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDescriptorPool*                           pDescriptorPool)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateDescriptorPool(device_data, device, pCreateInfo, pAllocator, pDescriptorPool);
    VkResult result = device_data->dispatch_table.CreateDescriptorPool(device, pCreateInfo, pAllocator, pDescriptorPool);
    PostCallCreateDescriptorPool(device_data, device, pCreateInfo, pAllocator, pDescriptorPool);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyDescriptorPool(
    VkDevice                                    device,
    VkDescriptorPool                            descriptorPool,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroyDescriptorPool(device_data, device, descriptorPool, pAllocator);
    device_data->dispatch_table.DestroyDescriptorPool(device, descriptorPool, pAllocator);
    PostCallDestroyDescriptorPool(device_data, device, descriptorPool, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL ResetDescriptorPool(
    VkDevice                                    device,
    VkDescriptorPool                            descriptorPool,
    VkDescriptorPoolResetFlags                  flags)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallResetDescriptorPool(device_data, device, descriptorPool, flags);
    VkResult result = device_data->dispatch_table.ResetDescriptorPool(device, descriptorPool, flags);
    PostCallResetDescriptorPool(device_data, device, descriptorPool, flags);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL AllocateDescriptorSets(
    VkDevice                                    device,
    const VkDescriptorSetAllocateInfo*          pAllocateInfo,
    VkDescriptorSet*                            pDescriptorSets)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallAllocateDescriptorSets(device_data, device, pAllocateInfo, pDescriptorSets);
    VkResult result = device_data->dispatch_table.AllocateDescriptorSets(device, pAllocateInfo, pDescriptorSets);
    PostCallAllocateDescriptorSets(device_data, device, pAllocateInfo, pDescriptorSets);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL FreeDescriptorSets(
    VkDevice                                    device,
    VkDescriptorPool                            descriptorPool,
    uint32_t                                    descriptorSetCount,
    const VkDescriptorSet*                      pDescriptorSets)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallFreeDescriptorSets(device_data, device, descriptorPool, descriptorSetCount, pDescriptorSets);
    VkResult result = device_data->dispatch_table.FreeDescriptorSets(device, descriptorPool, descriptorSetCount, pDescriptorSets);
    PostCallFreeDescriptorSets(device_data, device, descriptorPool, descriptorSetCount, pDescriptorSets);
    return result;
}

VKAPI_ATTR void VKAPI_CALL UpdateDescriptorSets(
    VkDevice                                    device,
    uint32_t                                    descriptorWriteCount,
    const VkWriteDescriptorSet*                 pDescriptorWrites,
    uint32_t                                    descriptorCopyCount,
    const VkCopyDescriptorSet*                  pDescriptorCopies)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallUpdateDescriptorSets(device_data, device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount,
                                pDescriptorCopies);
    device_data->dispatch_table.UpdateDescriptorSets(device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount,
                                                     pDescriptorCopies);
    PostCallUpdateDescriptorSets(device_data, device, descriptorWriteCount, pDescriptorWrites, descriptorCopyCount,
                                 pDescriptorCopies);
}

VKAPI_ATTR VkResult VKAPI_CALL CreateFramebuffer(
    VkDevice                                    device,
    const VkFramebufferCreateInfo*              pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkFramebuffer*                              pFramebuffer)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateFramebuffer(device_data, device, pCreateInfo, pAllocator, pFramebuffer);
    VkResult result = device_data->dispatch_table.CreateFramebuffer(device, pCreateInfo, pAllocator, pFramebuffer);
    PostCallCreateFramebuffer(device_data, device, pCreateInfo, pAllocator, pFramebuffer);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyFramebuffer(
    VkDevice                                    device,
    VkFramebuffer                               framebuffer,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroyFramebuffer(device_data, device, framebuffer, pAllocator);
    device_data->dispatch_table.DestroyFramebuffer(device, framebuffer, pAllocator);
    PostCallDestroyFramebuffer(device_data, device, framebuffer, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL CreateRenderPass(
    VkDevice                                    device,
    const VkRenderPassCreateInfo*               pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkRenderPass*                               pRenderPass)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateRenderPass(device_data, device, pCreateInfo, pAllocator, pRenderPass);
    VkResult result = device_data->dispatch_table.CreateRenderPass(device, pCreateInfo, pAllocator, pRenderPass);
    PostCallCreateRenderPass(device_data, device, pCreateInfo, pAllocator, pRenderPass);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyRenderPass(
    VkDevice                                    device,
    VkRenderPass                                renderPass,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroyRenderPass(device_data, device, renderPass, pAllocator);
    device_data->dispatch_table.DestroyRenderPass(device, renderPass, pAllocator);
    PostCallDestroyRenderPass(device_data, device, renderPass, pAllocator);
}

VKAPI_ATTR void VKAPI_CALL GetRenderAreaGranularity(
    VkDevice                                    device,
    VkRenderPass                                renderPass,
    VkExtent2D*                                 pGranularity)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetRenderAreaGranularity(device_data, device, renderPass, pGranularity);
    device_data->dispatch_table.GetRenderAreaGranularity(device, renderPass, pGranularity);
    PostCallGetRenderAreaGranularity(device_data, device, renderPass, pGranularity);
}

VKAPI_ATTR VkResult VKAPI_CALL CreateCommandPool(
    VkDevice                                    device,
    const VkCommandPoolCreateInfo*              pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkCommandPool*                              pCommandPool)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateCommandPool(device_data, device, pCreateInfo, pAllocator, pCommandPool);
    VkResult result = device_data->dispatch_table.CreateCommandPool(device, pCreateInfo, pAllocator, pCommandPool);
    PostCallCreateCommandPool(device_data, device, pCreateInfo, pAllocator, pCommandPool);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyCommandPool(
    VkDevice                                    device,
    VkCommandPool                               commandPool,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroyCommandPool(device_data, device, commandPool, pAllocator);
    device_data->dispatch_table.DestroyCommandPool(device, commandPool, pAllocator);
    PostCallDestroyCommandPool(device_data, device, commandPool, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL ResetCommandPool(
    VkDevice                                    device,
    VkCommandPool                               commandPool,
    VkCommandPoolResetFlags                     flags)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallResetCommandPool(device_data, device, commandPool, flags);
    VkResult result = device_data->dispatch_table.ResetCommandPool(device, commandPool, flags);
    PostCallResetCommandPool(device_data, device, commandPool, flags);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL AllocateCommandBuffers(
    VkDevice                                    device,
    const VkCommandBufferAllocateInfo*          pAllocateInfo,
    VkCommandBuffer*                            pCommandBuffers)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallAllocateCommandBuffers(device_data, device, pAllocateInfo, pCommandBuffers);
    VkResult result = device_data->dispatch_table.AllocateCommandBuffers(device, pAllocateInfo, pCommandBuffers);
    PostCallAllocateCommandBuffers(device_data, device, pAllocateInfo, pCommandBuffers);
    return result;
}

VKAPI_ATTR void VKAPI_CALL FreeCommandBuffers(
    VkDevice                                    device,
    VkCommandPool                               commandPool,
    uint32_t                                    commandBufferCount,
    const VkCommandBuffer*                      pCommandBuffers)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallFreeCommandBuffers(device_data, device, commandPool, commandBufferCount, pCommandBuffers);
    device_data->dispatch_table.FreeCommandBuffers(device, commandPool, commandBufferCount, pCommandBuffers);
    PostCallFreeCommandBuffers(device_data, device, commandPool, commandBufferCount, pCommandBuffers);
}

VKAPI_ATTR VkResult VKAPI_CALL BeginCommandBuffer(
    VkCommandBuffer                             commandBuffer,
    const VkCommandBufferBeginInfo*             pBeginInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallBeginCommandBuffer(device_data, commandBuffer, pBeginInfo);
    VkResult result = device_data->dispatch_table.BeginCommandBuffer(commandBuffer, pBeginInfo);
    PostCallBeginCommandBuffer(device_data, commandBuffer, pBeginInfo);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL EndCommandBuffer(
    VkCommandBuffer                             commandBuffer)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallEndCommandBuffer(device_data, commandBuffer);
    VkResult result = device_data->dispatch_table.EndCommandBuffer(commandBuffer);
    PostCallEndCommandBuffer(device_data, commandBuffer);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL ResetCommandBuffer(
    VkCommandBuffer                             commandBuffer,
    VkCommandBufferResetFlags                   flags)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallResetCommandBuffer(device_data, commandBuffer, flags);
    VkResult result = device_data->dispatch_table.ResetCommandBuffer(commandBuffer, flags);
    PostCallResetCommandBuffer(device_data, commandBuffer, flags);
    return result;
}

VKAPI_ATTR void VKAPI_CALL CmdBindPipeline(
    VkCommandBuffer                             commandBuffer,
    VkPipelineBindPoint                         pipelineBindPoint,
    VkPipeline                                  pipeline)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdBindPipeline(device_data, commandBuffer, pipelineBindPoint, pipeline);
    device_data->dispatch_table.CmdBindPipeline(commandBuffer, pipelineBindPoint, pipeline);
    PostCallCmdBindPipeline(device_data, commandBuffer, pipelineBindPoint, pipeline);
}

VKAPI_ATTR void VKAPI_CALL CmdSetViewport(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstViewport,
    uint32_t                                    viewportCount,
    const VkViewport*                           pViewports)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdSetViewport(device_data, commandBuffer, firstViewport, viewportCount, pViewports);
    device_data->dispatch_table.CmdSetViewport(commandBuffer, firstViewport, viewportCount, pViewports);
    PostCallCmdSetViewport(device_data, commandBuffer, firstViewport, viewportCount, pViewports);
}

VKAPI_ATTR void VKAPI_CALL CmdSetScissor(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstScissor,
    uint32_t                                    scissorCount,
    const VkRect2D*                             pScissors)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdSetScissor(device_data, commandBuffer, firstScissor, scissorCount, pScissors);
    device_data->dispatch_table.CmdSetScissor(commandBuffer, firstScissor, scissorCount, pScissors);
    PostCallCmdSetScissor(device_data, commandBuffer, firstScissor, scissorCount, pScissors);
}

VKAPI_ATTR void VKAPI_CALL CmdSetLineWidth(
    VkCommandBuffer                             commandBuffer,
    float                                       lineWidth)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdSetLineWidth(device_data, commandBuffer, lineWidth);
    device_data->dispatch_table.CmdSetLineWidth(commandBuffer, lineWidth);
    PostCallCmdSetLineWidth(device_data, commandBuffer, lineWidth);
}

VKAPI_ATTR void VKAPI_CALL CmdSetDepthBias(
    VkCommandBuffer                             commandBuffer,
    float                                       depthBiasConstantFactor,
    float                                       depthBiasClamp,
    float                                       depthBiasSlopeFactor)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdSetDepthBias(device_data, commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
    device_data->dispatch_table.CmdSetDepthBias(commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
    PostCallCmdSetDepthBias(device_data, commandBuffer, depthBiasConstantFactor, depthBiasClamp, depthBiasSlopeFactor);
}

VKAPI_ATTR void VKAPI_CALL CmdSetBlendConstants(
    VkCommandBuffer                             commandBuffer,
    const float                                 blendConstants[4])
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdSetBlendConstants(device_data, commandBuffer, blendConstants);
    device_data->dispatch_table.CmdSetBlendConstants(commandBuffer, blendConstants);
    PostCallCmdSetBlendConstants(device_data, commandBuffer, blendConstants);
}

VKAPI_ATTR void VKAPI_CALL CmdSetDepthBounds(
    VkCommandBuffer                             commandBuffer,
    float                                       minDepthBounds,
    float                                       maxDepthBounds)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdSetDepthBounds(device_data, commandBuffer, minDepthBounds, maxDepthBounds);
    device_data->dispatch_table.CmdSetDepthBounds(commandBuffer, minDepthBounds, maxDepthBounds);
    PostCallCmdSetDepthBounds(device_data, commandBuffer, minDepthBounds, maxDepthBounds);
}

VKAPI_ATTR void VKAPI_CALL CmdSetStencilCompareMask(
    VkCommandBuffer                             commandBuffer,
    VkStencilFaceFlags                          faceMask,
    uint32_t                                    compareMask)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdSetStencilCompareMask(device_data, commandBuffer, faceMask, compareMask);
    device_data->dispatch_table.CmdSetStencilCompareMask(commandBuffer, faceMask, compareMask);
    PostCallCmdSetStencilCompareMask(device_data, commandBuffer, faceMask, compareMask);
}

VKAPI_ATTR void VKAPI_CALL CmdSetStencilWriteMask(
    VkCommandBuffer                             commandBuffer,
    VkStencilFaceFlags                          faceMask,
    uint32_t                                    writeMask)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdSetStencilWriteMask(device_data, commandBuffer, faceMask, writeMask);
    device_data->dispatch_table.CmdSetStencilWriteMask(commandBuffer, faceMask, writeMask);
    PostCallCmdSetStencilWriteMask(device_data, commandBuffer, faceMask, writeMask);
}

VKAPI_ATTR void VKAPI_CALL CmdSetStencilReference(
    VkCommandBuffer                             commandBuffer,
    VkStencilFaceFlags                          faceMask,
    uint32_t                                    reference)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdSetStencilReference(device_data, commandBuffer, faceMask, reference);
    device_data->dispatch_table.CmdSetStencilReference(commandBuffer, faceMask, reference);
    PostCallCmdSetStencilReference(device_data, commandBuffer, faceMask, reference);
}

VKAPI_ATTR void VKAPI_CALL CmdBindDescriptorSets(
    VkCommandBuffer                             commandBuffer,
    VkPipelineBindPoint                         pipelineBindPoint,
    VkPipelineLayout                            layout,
    uint32_t                                    firstSet,
    uint32_t                                    descriptorSetCount,
    const VkDescriptorSet*                      pDescriptorSets,
    uint32_t                                    dynamicOffsetCount,
    const uint32_t*                             pDynamicOffsets)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdBindDescriptorSets(device_data, commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount,
                                 pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
    device_data->dispatch_table.CmdBindDescriptorSets(commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount,
                                                      pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
    PostCallCmdBindDescriptorSets(device_data, commandBuffer, pipelineBindPoint, layout, firstSet, descriptorSetCount,
                                  pDescriptorSets, dynamicOffsetCount, pDynamicOffsets);
}

VKAPI_ATTR void VKAPI_CALL CmdBindIndexBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    VkIndexType                                 indexType)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdBindIndexBuffer(device_data, commandBuffer, buffer, offset, indexType);
    device_data->dispatch_table.CmdBindIndexBuffer(commandBuffer, buffer, offset, indexType);
    PostCallCmdBindIndexBuffer(device_data, commandBuffer, buffer, offset, indexType);
}

VKAPI_ATTR void VKAPI_CALL CmdBindVertexBuffers(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstBinding,
    uint32_t                                    bindingCount,
    const VkBuffer*                             pBuffers,
    const VkDeviceSize*                         pOffsets)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdBindVertexBuffers(device_data, commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
    device_data->dispatch_table.CmdBindVertexBuffers(commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
    PostCallCmdBindVertexBuffers(device_data, commandBuffer, firstBinding, bindingCount, pBuffers, pOffsets);
}

VKAPI_ATTR void VKAPI_CALL CmdDraw(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    vertexCount,
    uint32_t                                    instanceCount,
    uint32_t                                    firstVertex,
    uint32_t                                    firstInstance)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdDraw(device_data, commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
    device_data->dispatch_table.CmdDraw(commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
    PostCallCmdDraw(device_data, commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
}

VKAPI_ATTR void VKAPI_CALL CmdDrawIndexed(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    indexCount,
    uint32_t                                    instanceCount,
    uint32_t                                    firstIndex,
    int32_t                                     vertexOffset,
    uint32_t                                    firstInstance)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdDrawIndexed(device_data, commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    device_data->dispatch_table.CmdDrawIndexed(commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
    PostCallCmdDrawIndexed(device_data, commandBuffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
}

VKAPI_ATTR void VKAPI_CALL CmdDrawIndirect(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    uint32_t                                    drawCount,
    uint32_t                                    stride)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdDrawIndirect(device_data, commandBuffer, buffer, offset, drawCount, stride);
    device_data->dispatch_table.CmdDrawIndirect(commandBuffer, buffer, offset, drawCount, stride);
    PostCallCmdDrawIndirect(device_data, commandBuffer, buffer, offset, drawCount, stride);
}

VKAPI_ATTR void VKAPI_CALL CmdDrawIndexedIndirect(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    uint32_t                                    drawCount,
    uint32_t                                    stride)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdDrawIndexedIndirect(device_data, commandBuffer, buffer, offset, drawCount, stride);
    device_data->dispatch_table.CmdDrawIndexedIndirect(commandBuffer, buffer, offset, drawCount, stride);
    PostCallCmdDrawIndexedIndirect(device_data, commandBuffer, buffer, offset, drawCount, stride);
}

VKAPI_ATTR void VKAPI_CALL CmdDispatch(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    groupCountX,
    uint32_t                                    groupCountY,
    uint32_t                                    groupCountZ)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdDispatch(device_data, commandBuffer, groupCountX, groupCountY, groupCountZ);
    device_data->dispatch_table.CmdDispatch(commandBuffer, groupCountX, groupCountY, groupCountZ);
    PostCallCmdDispatch(device_data, commandBuffer, groupCountX, groupCountY, groupCountZ);
}

VKAPI_ATTR void VKAPI_CALL CmdDispatchIndirect(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdDispatchIndirect(device_data, commandBuffer, buffer, offset);
    device_data->dispatch_table.CmdDispatchIndirect(commandBuffer, buffer, offset);
    PostCallCmdDispatchIndirect(device_data, commandBuffer, buffer, offset);
}

VKAPI_ATTR void VKAPI_CALL CmdCopyBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    srcBuffer,
    VkBuffer                                    dstBuffer,
    uint32_t                                    regionCount,
    const VkBufferCopy*                         pRegions)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdCopyBuffer(device_data, commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
    device_data->dispatch_table.CmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
    PostCallCmdCopyBuffer(device_data, commandBuffer, srcBuffer, dstBuffer, regionCount, pRegions);
}

VKAPI_ATTR void VKAPI_CALL CmdCopyImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     srcImage,
    VkImageLayout                               srcImageLayout,
    VkImage                                     dstImage,
    VkImageLayout                               dstImageLayout,
    uint32_t                                    regionCount,
    const VkImageCopy*                          pRegions)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdCopyImage(device_data, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
    device_data->dispatch_table.CmdCopyImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount,
                                             pRegions);
    PostCallCmdCopyImage(device_data, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

VKAPI_ATTR void VKAPI_CALL CmdBlitImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     srcImage,
    VkImageLayout                               srcImageLayout,
    VkImage                                     dstImage,
    VkImageLayout                               dstImageLayout,
    uint32_t                                    regionCount,
    const VkImageBlit*                          pRegions,
    VkFilter                                    filter)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdBlitImage(device_data, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions,
                        filter);
    device_data->dispatch_table.CmdBlitImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount,
                                             pRegions, filter);
    PostCallCmdBlitImage(device_data, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions,
                         filter);
}

VKAPI_ATTR void VKAPI_CALL CmdCopyBufferToImage(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    srcBuffer,
    VkImage                                     dstImage,
    VkImageLayout                               dstImageLayout,
    uint32_t                                    regionCount,
    const VkBufferImageCopy*                    pRegions)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdCopyBufferToImage(device_data, commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
    device_data->dispatch_table.CmdCopyBufferToImage(commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
    PostCallCmdCopyBufferToImage(device_data, commandBuffer, srcBuffer, dstImage, dstImageLayout, regionCount, pRegions);
}

VKAPI_ATTR void VKAPI_CALL CmdCopyImageToBuffer(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     srcImage,
    VkImageLayout                               srcImageLayout,
    VkBuffer                                    dstBuffer,
    uint32_t                                    regionCount,
    const VkBufferImageCopy*                    pRegions)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdCopyImageToBuffer(device_data, commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
    device_data->dispatch_table.CmdCopyImageToBuffer(commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
    PostCallCmdCopyImageToBuffer(device_data, commandBuffer, srcImage, srcImageLayout, dstBuffer, regionCount, pRegions);
}

VKAPI_ATTR void VKAPI_CALL CmdUpdateBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    dstBuffer,
    VkDeviceSize                                dstOffset,
    VkDeviceSize                                dataSize,
    const void*                                 pData)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdUpdateBuffer(device_data, commandBuffer, dstBuffer, dstOffset, dataSize, pData);
    device_data->dispatch_table.CmdUpdateBuffer(commandBuffer, dstBuffer, dstOffset, dataSize, pData);
    PostCallCmdUpdateBuffer(device_data, commandBuffer, dstBuffer, dstOffset, dataSize, pData);
}

VKAPI_ATTR void VKAPI_CALL CmdFillBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    dstBuffer,
    VkDeviceSize                                dstOffset,
    VkDeviceSize                                size,
    uint32_t                                    data)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdFillBuffer(device_data, commandBuffer, dstBuffer, dstOffset, size, data);
    device_data->dispatch_table.CmdFillBuffer(commandBuffer, dstBuffer, dstOffset, size, data);
    PostCallCmdFillBuffer(device_data, commandBuffer, dstBuffer, dstOffset, size, data);
}

VKAPI_ATTR void VKAPI_CALL CmdClearColorImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     image,
    VkImageLayout                               imageLayout,
    const VkClearColorValue*                    pColor,
    uint32_t                                    rangeCount,
    const VkImageSubresourceRange*              pRanges)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdClearColorImage(device_data, commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
    device_data->dispatch_table.CmdClearColorImage(commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
    PostCallCmdClearColorImage(device_data, commandBuffer, image, imageLayout, pColor, rangeCount, pRanges);
}

VKAPI_ATTR void VKAPI_CALL CmdClearDepthStencilImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     image,
    VkImageLayout                               imageLayout,
    const VkClearDepthStencilValue*             pDepthStencil,
    uint32_t                                    rangeCount,
    const VkImageSubresourceRange*              pRanges)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdClearDepthStencilImage(device_data, commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
    device_data->dispatch_table.CmdClearDepthStencilImage(commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
    PostCallCmdClearDepthStencilImage(device_data, commandBuffer, image, imageLayout, pDepthStencil, rangeCount, pRanges);
}

VKAPI_ATTR void VKAPI_CALL CmdClearAttachments(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    attachmentCount,
    const VkClearAttachment*                    pAttachments,
    uint32_t                                    rectCount,
    const VkClearRect*                          pRects)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdClearAttachments(device_data, commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
    device_data->dispatch_table.CmdClearAttachments(commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
    PostCallCmdClearAttachments(device_data, commandBuffer, attachmentCount, pAttachments, rectCount, pRects);
}

VKAPI_ATTR void VKAPI_CALL CmdResolveImage(
    VkCommandBuffer                             commandBuffer,
    VkImage                                     srcImage,
    VkImageLayout                               srcImageLayout,
    VkImage                                     dstImage,
    VkImageLayout                               dstImageLayout,
    uint32_t                                    regionCount,
    const VkImageResolve*                       pRegions)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdResolveImage(device_data, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
    device_data->dispatch_table.CmdResolveImage(commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount,
                                                pRegions);
    PostCallCmdResolveImage(device_data, commandBuffer, srcImage, srcImageLayout, dstImage, dstImageLayout, regionCount, pRegions);
}

VKAPI_ATTR void VKAPI_CALL CmdSetEvent(
    VkCommandBuffer                             commandBuffer,
    VkEvent                                     event,
    VkPipelineStageFlags                        stageMask)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdSetEvent(device_data, commandBuffer, event, stageMask);
    device_data->dispatch_table.CmdSetEvent(commandBuffer, event, stageMask);
    PostCallCmdSetEvent(device_data, commandBuffer, event, stageMask);
}

VKAPI_ATTR void VKAPI_CALL CmdResetEvent(
    VkCommandBuffer                             commandBuffer,
    VkEvent                                     event,
    VkPipelineStageFlags                        stageMask)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdResetEvent(device_data, commandBuffer, event, stageMask);
    device_data->dispatch_table.CmdResetEvent(commandBuffer, event, stageMask);
    PostCallCmdResetEvent(device_data, commandBuffer, event, stageMask);
}

VKAPI_ATTR void VKAPI_CALL CmdWaitEvents(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    eventCount,
    const VkEvent*                              pEvents,
    VkPipelineStageFlags                        srcStageMask,
    VkPipelineStageFlags                        dstStageMask,
    uint32_t                                    memoryBarrierCount,
    const VkMemoryBarrier*                      pMemoryBarriers,
    uint32_t                                    bufferMemoryBarrierCount,
    const VkBufferMemoryBarrier*                pBufferMemoryBarriers,
    uint32_t                                    imageMemoryBarrierCount,
    const VkImageMemoryBarrier*                 pImageMemoryBarriers)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdWaitEvents(device_data, commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount,
                         pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount,
                         pImageMemoryBarriers);
    device_data->dispatch_table.CmdWaitEvents(commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount,
                                              pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers,
                                              imageMemoryBarrierCount, pImageMemoryBarriers);
    PostCallCmdWaitEvents(device_data, commandBuffer, eventCount, pEvents, srcStageMask, dstStageMask, memoryBarrierCount,
                          pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount,
                          pImageMemoryBarriers);
}

VKAPI_ATTR void VKAPI_CALL CmdPipelineBarrier(
    VkCommandBuffer                             commandBuffer,
    VkPipelineStageFlags                        srcStageMask,
    VkPipelineStageFlags                        dstStageMask,
    VkDependencyFlags                           dependencyFlags,
    uint32_t                                    memoryBarrierCount,
    const VkMemoryBarrier*                      pMemoryBarriers,
    uint32_t                                    bufferMemoryBarrierCount,
    const VkBufferMemoryBarrier*                pBufferMemoryBarriers,
    uint32_t                                    imageMemoryBarrierCount,
    const VkImageMemoryBarrier*                 pImageMemoryBarriers)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdPipelineBarrier(device_data, commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount,
                              pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount,
                              pImageMemoryBarriers);
    device_data->dispatch_table.CmdPipelineBarrier(commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount,
                                                   pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers,
                                                   imageMemoryBarrierCount, pImageMemoryBarriers);
    PostCallCmdPipelineBarrier(device_data, commandBuffer, srcStageMask, dstStageMask, dependencyFlags, memoryBarrierCount,
                               pMemoryBarriers, bufferMemoryBarrierCount, pBufferMemoryBarriers, imageMemoryBarrierCount,
                               pImageMemoryBarriers);
}

VKAPI_ATTR void VKAPI_CALL CmdBeginQuery(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    query,
    VkQueryControlFlags                         flags)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdBeginQuery(device_data, commandBuffer, queryPool, query, flags);
    device_data->dispatch_table.CmdBeginQuery(commandBuffer, queryPool, query, flags);
    PostCallCmdBeginQuery(device_data, commandBuffer, queryPool, query, flags);
}

VKAPI_ATTR void VKAPI_CALL CmdEndQuery(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    query)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdEndQuery(device_data, commandBuffer, queryPool, query);
    device_data->dispatch_table.CmdEndQuery(commandBuffer, queryPool, query);
    PostCallCmdEndQuery(device_data, commandBuffer, queryPool, query);
}

VKAPI_ATTR void VKAPI_CALL CmdResetQueryPool(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery,
    uint32_t                                    queryCount)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdResetQueryPool(device_data, commandBuffer, queryPool, firstQuery, queryCount);
    device_data->dispatch_table.CmdResetQueryPool(commandBuffer, queryPool, firstQuery, queryCount);
    PostCallCmdResetQueryPool(device_data, commandBuffer, queryPool, firstQuery, queryCount);
}

VKAPI_ATTR void VKAPI_CALL CmdWriteTimestamp(
    VkCommandBuffer                             commandBuffer,
    VkPipelineStageFlagBits                     pipelineStage,
    VkQueryPool                                 queryPool,
    uint32_t                                    query)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdWriteTimestamp(device_data, commandBuffer, pipelineStage, queryPool, query);
    device_data->dispatch_table.CmdWriteTimestamp(commandBuffer, pipelineStage, queryPool, query);
    PostCallCmdWriteTimestamp(device_data, commandBuffer, pipelineStage, queryPool, query);
}

VKAPI_ATTR void VKAPI_CALL CmdCopyQueryPoolResults(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery,
    uint32_t                                    queryCount,
    VkBuffer                                    dstBuffer,
    VkDeviceSize                                dstOffset,
    VkDeviceSize                                stride,
    VkQueryResultFlags                          flags)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdCopyQueryPoolResults(device_data, commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride,
                                   flags);
    device_data->dispatch_table.CmdCopyQueryPoolResults(commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset,
                                                        stride, flags);
    PostCallCmdCopyQueryPoolResults(device_data, commandBuffer, queryPool, firstQuery, queryCount, dstBuffer, dstOffset, stride,
                                    flags);
}

VKAPI_ATTR void VKAPI_CALL CmdPushConstants(
    VkCommandBuffer                             commandBuffer,
    VkPipelineLayout                            layout,
    VkShaderStageFlags                          stageFlags,
    uint32_t                                    offset,
    uint32_t                                    size,
    const void*                                 pValues)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdPushConstants(device_data, commandBuffer, layout, stageFlags, offset, size, pValues);
    device_data->dispatch_table.CmdPushConstants(commandBuffer, layout, stageFlags, offset, size, pValues);
    PostCallCmdPushConstants(device_data, commandBuffer, layout, stageFlags, offset, size, pValues);
}

VKAPI_ATTR void VKAPI_CALL CmdBeginRenderPass(
    VkCommandBuffer                             commandBuffer,
    const VkRenderPassBeginInfo*                pRenderPassBegin,
    VkSubpassContents                           contents)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdBeginRenderPass(device_data, commandBuffer, pRenderPassBegin, contents);
    device_data->dispatch_table.CmdBeginRenderPass(commandBuffer, pRenderPassBegin, contents);
    PostCallCmdBeginRenderPass(device_data, commandBuffer, pRenderPassBegin, contents);
}

VKAPI_ATTR void VKAPI_CALL CmdNextSubpass(
    VkCommandBuffer                             commandBuffer,
    VkSubpassContents                           contents)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdNextSubpass(device_data, commandBuffer, contents);
    device_data->dispatch_table.CmdNextSubpass(commandBuffer, contents);
    PostCallCmdNextSubpass(device_data, commandBuffer, contents);
}

VKAPI_ATTR void VKAPI_CALL CmdEndRenderPass(
    VkCommandBuffer                             commandBuffer)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdEndRenderPass(device_data, commandBuffer);
    device_data->dispatch_table.CmdEndRenderPass(commandBuffer);
    PostCallCmdEndRenderPass(device_data, commandBuffer);
}

VKAPI_ATTR void VKAPI_CALL CmdExecuteCommands(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    commandBufferCount,
    const VkCommandBuffer*                      pCommandBuffers)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdExecuteCommands(device_data, commandBuffer, commandBufferCount, pCommandBuffers);
    device_data->dispatch_table.CmdExecuteCommands(commandBuffer, commandBufferCount, pCommandBuffers);
    PostCallCmdExecuteCommands(device_data, commandBuffer, commandBufferCount, pCommandBuffers);
}


VKAPI_ATTR void VKAPI_CALL DestroySurfaceKHR(
    VkInstance                                  instance,
    VkSurfaceKHR                                surface,
    const VkAllocationCallbacks*                pAllocator)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    PreCallDestroySurfaceKHR(instance_data, instance, surface, pAllocator);
    instance_data->dispatch_table.DestroySurfaceKHR(instance, surface, pAllocator);
    PostCallDestroySurfaceKHR(instance_data, instance, surface, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceSurfaceSupportKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    queueFamilyIndex,
    VkSurfaceKHR                                surface,
    VkBool32*                                   pSupported)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceSurfaceSupportKHR(instance_data, physicalDevice, queueFamilyIndex, surface, pSupported);
    VkResult result =
        instance_data->dispatch_table.GetPhysicalDeviceSurfaceSupportKHR(physicalDevice, queueFamilyIndex, surface, pSupported);
    PostCallGetPhysicalDeviceSurfaceSupportKHR(instance_data, physicalDevice, queueFamilyIndex, surface, pSupported);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceSurfaceCapabilitiesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    VkSurfaceCapabilitiesKHR*                   pSurfaceCapabilities)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceSurfaceCapabilitiesKHR(instance_data, physicalDevice, surface, pSurfaceCapabilities);
    VkResult result =
        instance_data->dispatch_table.GetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, pSurfaceCapabilities);
    PostCallGetPhysicalDeviceSurfaceCapabilitiesKHR(instance_data, physicalDevice, surface, pSurfaceCapabilities);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceSurfaceFormatsKHR(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    uint32_t*                                   pSurfaceFormatCount,
    VkSurfaceFormatKHR*                         pSurfaceFormats)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceSurfaceFormatsKHR(instance_data, physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
    VkResult result = instance_data->dispatch_table.GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, pSurfaceFormatCount,
                                                                                       pSurfaceFormats);
    PostCallGetPhysicalDeviceSurfaceFormatsKHR(instance_data, physicalDevice, surface, pSurfaceFormatCount, pSurfaceFormats);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceSurfacePresentModesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    uint32_t*                                   pPresentModeCount,
    VkPresentModeKHR*                           pPresentModes)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceSurfacePresentModesKHR(instance_data, physicalDevice, surface, pPresentModeCount, pPresentModes);
    VkResult result = instance_data->dispatch_table.GetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface,
                                                                                            pPresentModeCount, pPresentModes);
    PostCallGetPhysicalDeviceSurfacePresentModesKHR(instance_data, physicalDevice, surface, pPresentModeCount, pPresentModes);
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL CreateSwapchainKHR(
    VkDevice                                    device,
    const VkSwapchainCreateInfoKHR*             pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSwapchainKHR*                             pSwapchain)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateSwapchainKHR(device_data, device, pCreateInfo, pAllocator, pSwapchain);
    VkResult result = device_data->dispatch_table.CreateSwapchainKHR(device, pCreateInfo, pAllocator, pSwapchain);
    PostCallCreateSwapchainKHR(device_data, device, pCreateInfo, pAllocator, pSwapchain);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroySwapchainKHR(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroySwapchainKHR(device_data, device, swapchain, pAllocator);
    device_data->dispatch_table.DestroySwapchainKHR(device, swapchain, pAllocator);
    PostCallDestroySwapchainKHR(device_data, device, swapchain, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL GetSwapchainImagesKHR(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain,
    uint32_t*                                   pSwapchainImageCount,
    VkImage*                                    pSwapchainImages)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetSwapchainImagesKHR(device_data, device, swapchain, pSwapchainImageCount, pSwapchainImages);
    VkResult result = device_data->dispatch_table.GetSwapchainImagesKHR(device, swapchain, pSwapchainImageCount, pSwapchainImages);
    PostCallGetSwapchainImagesKHR(device_data, device, swapchain, pSwapchainImageCount, pSwapchainImages);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL AcquireNextImageKHR(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain,
    uint64_t                                    timeout,
    VkSemaphore                                 semaphore,
    VkFence                                     fence,
    uint32_t*                                   pImageIndex)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallAcquireNextImageKHR(device_data, device, swapchain, timeout, semaphore, fence, pImageIndex);
    VkResult result = device_data->dispatch_table.AcquireNextImageKHR(device, swapchain, timeout, semaphore, fence, pImageIndex);
    PostCallAcquireNextImageKHR(device_data, device, swapchain, timeout, semaphore, fence, pImageIndex);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL QueuePresentKHR(
    VkQueue                                     queue,
    const VkPresentInfoKHR*                     pPresentInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(queue), device_layer_data_map);
    PreCallQueuePresentKHR(device_data, queue, pPresentInfo);
    VkResult result = device_data->dispatch_table.QueuePresentKHR(queue, pPresentInfo);
    PostCallQueuePresentKHR(device_data, queue, pPresentInfo);
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceDisplayPropertiesKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t*                                   pPropertyCount,
    VkDisplayPropertiesKHR*                     pProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceDisplayPropertiesKHR(instance_data, physicalDevice, pPropertyCount, pProperties);
    VkResult result =
        instance_data->dispatch_table.GetPhysicalDeviceDisplayPropertiesKHR(physicalDevice, pPropertyCount, pProperties);
    PostCallGetPhysicalDeviceDisplayPropertiesKHR(instance_data, physicalDevice, pPropertyCount, pProperties);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceDisplayPlanePropertiesKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t*                                   pPropertyCount,
    VkDisplayPlanePropertiesKHR*                pProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceDisplayPlanePropertiesKHR(instance_data, physicalDevice, pPropertyCount, pProperties);
    VkResult result =
        instance_data->dispatch_table.GetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice, pPropertyCount, pProperties);
    PostCallGetPhysicalDeviceDisplayPlanePropertiesKHR(instance_data, physicalDevice, pPropertyCount, pProperties);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetDisplayPlaneSupportedDisplaysKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    planeIndex,
    uint32_t*                                   pDisplayCount,
    VkDisplayKHR*                               pDisplays)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetDisplayPlaneSupportedDisplaysKHR(instance_data, physicalDevice, planeIndex, pDisplayCount, pDisplays);
    VkResult result =
        instance_data->dispatch_table.GetDisplayPlaneSupportedDisplaysKHR(physicalDevice, planeIndex, pDisplayCount, pDisplays);
    PostCallGetDisplayPlaneSupportedDisplaysKHR(instance_data, physicalDevice, planeIndex, pDisplayCount, pDisplays);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetDisplayModePropertiesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkDisplayKHR                                display,
    uint32_t*                                   pPropertyCount,
    VkDisplayModePropertiesKHR*                 pProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetDisplayModePropertiesKHR(instance_data, physicalDevice, display, pPropertyCount, pProperties);
    VkResult result =
        instance_data->dispatch_table.GetDisplayModePropertiesKHR(physicalDevice, display, pPropertyCount, pProperties);
    PostCallGetDisplayModePropertiesKHR(instance_data, physicalDevice, display, pPropertyCount, pProperties);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL CreateDisplayModeKHR(
    VkPhysicalDevice                            physicalDevice,
    VkDisplayKHR                                display,
    const VkDisplayModeCreateInfoKHR*           pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDisplayModeKHR*                           pMode)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallCreateDisplayModeKHR(instance_data, physicalDevice, display, pCreateInfo, pAllocator, pMode);
    VkResult result = instance_data->dispatch_table.CreateDisplayModeKHR(physicalDevice, display, pCreateInfo, pAllocator, pMode);
    PostCallCreateDisplayModeKHR(instance_data, physicalDevice, display, pCreateInfo, pAllocator, pMode);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetDisplayPlaneCapabilitiesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkDisplayModeKHR                            mode,
    uint32_t                                    planeIndex,
    VkDisplayPlaneCapabilitiesKHR*              pCapabilities)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetDisplayPlaneCapabilitiesKHR(instance_data, physicalDevice, mode, planeIndex, pCapabilities);
    VkResult result = instance_data->dispatch_table.GetDisplayPlaneCapabilitiesKHR(physicalDevice, mode, planeIndex, pCapabilities);
    PostCallGetDisplayPlaneCapabilitiesKHR(instance_data, physicalDevice, mode, planeIndex, pCapabilities);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL CreateDisplayPlaneSurfaceKHR(
    VkInstance                                  instance,
    const VkDisplaySurfaceCreateInfoKHR*        pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSurfaceKHR*                               pSurface)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    PreCallCreateDisplayPlaneSurfaceKHR(instance_data, instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_data->dispatch_table.CreateDisplayPlaneSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    PostCallCreateDisplayPlaneSurfaceKHR(instance_data, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL CreateSharedSwapchainsKHR(
    VkDevice                                    device,
    uint32_t                                    swapchainCount,
    const VkSwapchainCreateInfoKHR*             pCreateInfos,
    const VkAllocationCallbacks*                pAllocator,
    VkSwapchainKHR*                             pSwapchains)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateSharedSwapchainsKHR(device_data, device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
    VkResult result =
        device_data->dispatch_table.CreateSharedSwapchainsKHR(device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
    PostCallCreateSharedSwapchainsKHR(device_data, device, swapchainCount, pCreateInfos, pAllocator, pSwapchains);
    return result;
}

#ifdef VK_USE_PLATFORM_XLIB_KHR

VKAPI_ATTR VkResult VKAPI_CALL CreateXlibSurfaceKHR(
    VkInstance                                  instance,
    const VkXlibSurfaceCreateInfoKHR*           pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSurfaceKHR*                               pSurface)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    PreCallCreateXlibSurfaceKHR(instance_data, instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_data->dispatch_table.CreateXlibSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    PostCallCreateXlibSurfaceKHR(instance_data, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}

VKAPI_ATTR VkBool32 VKAPI_CALL GetPhysicalDeviceXlibPresentationSupportKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    queueFamilyIndex,
    Display*                                    dpy,
    VisualID                                    visualID)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceXlibPresentationSupportKHR(instance_data, physicalDevice, queueFamilyIndex, dpy, visualID);
    VkBool32 result =
        instance_data->dispatch_table.GetPhysicalDeviceXlibPresentationSupportKHR(physicalDevice, queueFamilyIndex, dpy, visualID);
    PostCallGetPhysicalDeviceXlibPresentationSupportKHR(instance_data, physicalDevice, queueFamilyIndex, dpy, visualID);
    return result;
}
#endif /* VK_USE_PLATFORM_XLIB_KHR */

#ifdef VK_USE_PLATFORM_XCB_KHR

VKAPI_ATTR VkResult VKAPI_CALL CreateXcbSurfaceKHR(
    VkInstance                                  instance,
    const VkXcbSurfaceCreateInfoKHR*            pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSurfaceKHR*                               pSurface)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    PreCallCreateXcbSurfaceKHR(instance_data, instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_data->dispatch_table.CreateXcbSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    PostCallCreateXcbSurfaceKHR(instance_data, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}

VKAPI_ATTR VkBool32 VKAPI_CALL GetPhysicalDeviceXcbPresentationSupportKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    queueFamilyIndex,
    xcb_connection_t*                           connection,
    xcb_visualid_t                              visual_id)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceXcbPresentationSupportKHR(instance_data, physicalDevice, queueFamilyIndex, connection, visual_id);
    VkBool32 result = instance_data->dispatch_table.GetPhysicalDeviceXcbPresentationSupportKHR(physicalDevice, queueFamilyIndex,
                                                                                               connection, visual_id);
    PostCallGetPhysicalDeviceXcbPresentationSupportKHR(instance_data, physicalDevice, queueFamilyIndex, connection, visual_id);
    return result;
}
#endif /* VK_USE_PLATFORM_XCB_KHR */

#ifdef VK_USE_PLATFORM_WAYLAND_KHR

VKAPI_ATTR VkResult VKAPI_CALL CreateWaylandSurfaceKHR(
    VkInstance                                  instance,
    const VkWaylandSurfaceCreateInfoKHR*        pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSurfaceKHR*                               pSurface)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    PreCallCreateWaylandSurfaceKHR(instance_data, instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_data->dispatch_table.CreateWaylandSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    PostCallCreateWaylandSurfaceKHR(instance_data, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}

VKAPI_ATTR VkBool32 VKAPI_CALL GetPhysicalDeviceWaylandPresentationSupportKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    queueFamilyIndex,
    struct wl_display*                          display)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceWaylandPresentationSupportKHR(instance_data, physicalDevice, queueFamilyIndex, display);
    VkBool32 result =
        instance_data->dispatch_table.GetPhysicalDeviceWaylandPresentationSupportKHR(physicalDevice, queueFamilyIndex, display);
    PostCallGetPhysicalDeviceWaylandPresentationSupportKHR(instance_data, physicalDevice, queueFamilyIndex, display);
    return result;
}
#endif /* VK_USE_PLATFORM_WAYLAND_KHR */

#ifdef VK_USE_PLATFORM_MIR_KHR

VKAPI_ATTR VkResult VKAPI_CALL CreateMirSurfaceKHR(
    VkInstance                                  instance,
    const VkMirSurfaceCreateInfoKHR*            pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSurfaceKHR*                               pSurface)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    PreCallCreateMirSurfaceKHR(instance_data, instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_data->dispatch_table.CreateMirSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    PostCallCreateMirSurfaceKHR(instance_data, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}

VKAPI_ATTR VkBool32 VKAPI_CALL GetPhysicalDeviceMirPresentationSupportKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    queueFamilyIndex,
    MirConnection*                              connection)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceMirPresentationSupportKHR(instance_data, physicalDevice, queueFamilyIndex, connection);
    VkBool32 result =
        instance_data->dispatch_table.GetPhysicalDeviceMirPresentationSupportKHR(physicalDevice, queueFamilyIndex, connection);
    PostCallGetPhysicalDeviceMirPresentationSupportKHR(instance_data, physicalDevice, queueFamilyIndex, connection);
    return result;
}
#endif /* VK_USE_PLATFORM_MIR_KHR */

#ifdef VK_USE_PLATFORM_ANDROID_KHR

VKAPI_ATTR VkResult VKAPI_CALL CreateAndroidSurfaceKHR(
    VkInstance                                  instance,
    const VkAndroidSurfaceCreateInfoKHR*        pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSurfaceKHR*                               pSurface)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    PreCallCreateAndroidSurfaceKHR(instance_data, instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_data->dispatch_table.CreateAndroidSurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    PostCallCreateAndroidSurfaceKHR(instance_data, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}
#endif /* VK_USE_PLATFORM_ANDROID_KHR */

#ifdef VK_USE_PLATFORM_WIN32_KHR

VKAPI_ATTR VkResult VKAPI_CALL CreateWin32SurfaceKHR(
    VkInstance                                  instance,
    const VkWin32SurfaceCreateInfoKHR*          pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSurfaceKHR*                               pSurface)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    PreCallCreateWin32SurfaceKHR(instance_data, instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_data->dispatch_table.CreateWin32SurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
    PostCallCreateWin32SurfaceKHR(instance_data, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}

VKAPI_ATTR VkBool32 VKAPI_CALL GetPhysicalDeviceWin32PresentationSupportKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    queueFamilyIndex)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceWin32PresentationSupportKHR(instance_data, physicalDevice, queueFamilyIndex);
    VkBool32 result = instance_data->dispatch_table.GetPhysicalDeviceWin32PresentationSupportKHR(physicalDevice, queueFamilyIndex);
    PostCallGetPhysicalDeviceWin32PresentationSupportKHR(instance_data, physicalDevice, queueFamilyIndex);
    return result;
}
#endif /* VK_USE_PLATFORM_WIN32_KHR */



VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceFeatures2KHR(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceFeatures2KHR*               pFeatures)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceFeatures2KHR(instance_data, physicalDevice, pFeatures);
    instance_data->dispatch_table.GetPhysicalDeviceFeatures2KHR(physicalDevice, pFeatures);
    PostCallGetPhysicalDeviceFeatures2KHR(instance_data, physicalDevice, pFeatures);
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceProperties2KHR(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceProperties2KHR*             pProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceProperties2KHR(instance_data, physicalDevice, pProperties);
    instance_data->dispatch_table.GetPhysicalDeviceProperties2KHR(physicalDevice, pProperties);
    PostCallGetPhysicalDeviceProperties2KHR(instance_data, physicalDevice, pProperties);
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceFormatProperties2KHR(
    VkPhysicalDevice                            physicalDevice,
    VkFormat                                    format,
    VkFormatProperties2KHR*                     pFormatProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceFormatProperties2KHR(instance_data, physicalDevice, format, pFormatProperties);
    instance_data->dispatch_table.GetPhysicalDeviceFormatProperties2KHR(physicalDevice, format, pFormatProperties);
    PostCallGetPhysicalDeviceFormatProperties2KHR(instance_data, physicalDevice, format, pFormatProperties);
}

VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceImageFormatProperties2KHR(
    VkPhysicalDevice                            physicalDevice,
    const VkPhysicalDeviceImageFormatInfo2KHR*  pImageFormatInfo,
    VkImageFormatProperties2KHR*                pImageFormatProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceImageFormatProperties2KHR(instance_data, physicalDevice, pImageFormatInfo, pImageFormatProperties);
    VkResult result = instance_data->dispatch_table.GetPhysicalDeviceImageFormatProperties2KHR(physicalDevice, pImageFormatInfo,
                                                                                               pImageFormatProperties);
    PostCallGetPhysicalDeviceImageFormatProperties2KHR(instance_data, physicalDevice, pImageFormatInfo, pImageFormatProperties);
    return result;
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceQueueFamilyProperties2KHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t*                                   pQueueFamilyPropertyCount,
    VkQueueFamilyProperties2KHR*                pQueueFamilyProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceQueueFamilyProperties2KHR(instance_data, physicalDevice, pQueueFamilyPropertyCount,
                                                      pQueueFamilyProperties);
    instance_data->dispatch_table.GetPhysicalDeviceQueueFamilyProperties2KHR(physicalDevice, pQueueFamilyPropertyCount,
                                                                             pQueueFamilyProperties);
    PostCallGetPhysicalDeviceQueueFamilyProperties2KHR(instance_data, physicalDevice, pQueueFamilyPropertyCount,
                                                       pQueueFamilyProperties);
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceMemoryProperties2KHR(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceMemoryProperties2KHR*       pMemoryProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceMemoryProperties2KHR(instance_data, physicalDevice, pMemoryProperties);
    instance_data->dispatch_table.GetPhysicalDeviceMemoryProperties2KHR(physicalDevice, pMemoryProperties);
    PostCallGetPhysicalDeviceMemoryProperties2KHR(instance_data, physicalDevice, pMemoryProperties);
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceSparseImageFormatProperties2KHR(
    VkPhysicalDevice                            physicalDevice,
    const VkPhysicalDeviceSparseImageFormatInfo2KHR* pFormatInfo,
    uint32_t*                                   pPropertyCount,
    VkSparseImageFormatProperties2KHR*          pProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceSparseImageFormatProperties2KHR(instance_data, physicalDevice, pFormatInfo, pPropertyCount,
                                                            pProperties);
    instance_data->dispatch_table.GetPhysicalDeviceSparseImageFormatProperties2KHR(physicalDevice, pFormatInfo, pPropertyCount,
                                                                                   pProperties);
    PostCallGetPhysicalDeviceSparseImageFormatProperties2KHR(instance_data, physicalDevice, pFormatInfo, pPropertyCount,
                                                             pProperties);
}



VKAPI_ATTR void VKAPI_CALL TrimCommandPoolKHR(
    VkDevice                                    device,
    VkCommandPool                               commandPool,
    VkCommandPoolTrimFlagsKHR                   flags)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallTrimCommandPoolKHR(device_data, device, commandPool, flags);
    device_data->dispatch_table.TrimCommandPoolKHR(device, commandPool, flags);
    PostCallTrimCommandPoolKHR(device_data, device, commandPool, flags);
}


VKAPI_ATTR void VKAPI_CALL CmdPushDescriptorSetKHR(
    VkCommandBuffer                             commandBuffer,
    VkPipelineBindPoint                         pipelineBindPoint,
    VkPipelineLayout                            layout,
    uint32_t                                    set,
    uint32_t                                    descriptorWriteCount,
    const VkWriteDescriptorSet*                 pDescriptorWrites)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdPushDescriptorSetKHR(device_data, commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount,
                                   pDescriptorWrites);
    device_data->dispatch_table.CmdPushDescriptorSetKHR(commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount,
                                                        pDescriptorWrites);
    PostCallCmdPushDescriptorSetKHR(device_data, commandBuffer, pipelineBindPoint, layout, set, descriptorWriteCount,
                                    pDescriptorWrites);
}



VKAPI_ATTR VkResult VKAPI_CALL CreateDescriptorUpdateTemplateKHR(
    VkDevice                                    device,
    const VkDescriptorUpdateTemplateCreateInfoKHR* pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDescriptorUpdateTemplateKHR*              pDescriptorUpdateTemplate)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateDescriptorUpdateTemplateKHR(device_data, device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
    VkResult result =
        device_data->dispatch_table.CreateDescriptorUpdateTemplateKHR(device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
    PostCallCreateDescriptorUpdateTemplateKHR(device_data, device, pCreateInfo, pAllocator, pDescriptorUpdateTemplate);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyDescriptorUpdateTemplateKHR(
    VkDevice                                    device,
    VkDescriptorUpdateTemplateKHR               descriptorUpdateTemplate,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroyDescriptorUpdateTemplateKHR(device_data, device, descriptorUpdateTemplate, pAllocator);
    device_data->dispatch_table.DestroyDescriptorUpdateTemplateKHR(device, descriptorUpdateTemplate, pAllocator);
    PostCallDestroyDescriptorUpdateTemplateKHR(device_data, device, descriptorUpdateTemplate, pAllocator);
}

VKAPI_ATTR void VKAPI_CALL UpdateDescriptorSetWithTemplateKHR(
    VkDevice                                    device,
    VkDescriptorSet                             descriptorSet,
    VkDescriptorUpdateTemplateKHR               descriptorUpdateTemplate,
    const void*                                 pData)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallUpdateDescriptorSetWithTemplateKHR(device_data, device, descriptorSet, descriptorUpdateTemplate, pData);
    device_data->dispatch_table.UpdateDescriptorSetWithTemplateKHR(device, descriptorSet, descriptorUpdateTemplate, pData);
    PostCallUpdateDescriptorSetWithTemplateKHR(device_data, device, descriptorSet, descriptorUpdateTemplate, pData);
}

VKAPI_ATTR void VKAPI_CALL CmdPushDescriptorSetWithTemplateKHR(
    VkCommandBuffer                             commandBuffer,
    VkDescriptorUpdateTemplateKHR               descriptorUpdateTemplate,
    VkPipelineLayout                            layout,
    uint32_t                                    set,
    const void*                                 pData)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdPushDescriptorSetWithTemplateKHR(device_data, commandBuffer, descriptorUpdateTemplate, layout, set, pData);
    device_data->dispatch_table.CmdPushDescriptorSetWithTemplateKHR(commandBuffer, descriptorUpdateTemplate, layout, set, pData);
    PostCallCmdPushDescriptorSetWithTemplateKHR(device_data, commandBuffer, descriptorUpdateTemplate, layout, set, pData);
}


VKAPI_ATTR VkResult VKAPI_CALL GetSwapchainStatusKHR(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetSwapchainStatusKHR(device_data, device, swapchain);
    VkResult result = device_data->dispatch_table.GetSwapchainStatusKHR(device, swapchain);
    PostCallGetSwapchainStatusKHR(device_data, device, swapchain);
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceSurfaceCapabilities2KHR(
    VkPhysicalDevice                            physicalDevice,
    const VkPhysicalDeviceSurfaceInfo2KHR*      pSurfaceInfo,
    VkSurfaceCapabilities2KHR*                  pSurfaceCapabilities)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceSurfaceCapabilities2KHR(instance_data, physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
    VkResult result =
        instance_data->dispatch_table.GetPhysicalDeviceSurfaceCapabilities2KHR(physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
    PostCallGetPhysicalDeviceSurfaceCapabilities2KHR(instance_data, physicalDevice, pSurfaceInfo, pSurfaceCapabilities);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceSurfaceFormats2KHR(
    VkPhysicalDevice                            physicalDevice,
    const VkPhysicalDeviceSurfaceInfo2KHR*      pSurfaceInfo,
    uint32_t*                                   pSurfaceFormatCount,
    VkSurfaceFormat2KHR*                        pSurfaceFormats)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceSurfaceFormats2KHR(instance_data, physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
    VkResult result = instance_data->dispatch_table.GetPhysicalDeviceSurfaceFormats2KHR(physicalDevice, pSurfaceInfo,
                                                                                        pSurfaceFormatCount, pSurfaceFormats);
    PostCallGetPhysicalDeviceSurfaceFormats2KHR(instance_data, physicalDevice, pSurfaceInfo, pSurfaceFormatCount, pSurfaceFormats);
    return result;
}

// declare only
//VKAPI_ATTR VkResult VKAPI_CALL CreateDebugReportCallbackEXT(
//    VkInstance                                  instance,
//    const VkDebugReportCallbackCreateInfoEXT*   pCreateInfo,
//    const VkAllocationCallbacks*                pAllocator,
//    VkDebugReportCallbackEXT*                   pCallback);
// TODO: Implement custom intercept body

// declare only
//VKAPI_ATTR void VKAPI_CALL DestroyDebugReportCallbackEXT(
//    VkInstance                                  instance,
//    VkDebugReportCallbackEXT                    callback,
//    const VkAllocationCallbacks*                pAllocator);
// TODO: Implement custom intercept body

VKAPI_ATTR void VKAPI_CALL DebugReportMessageEXT(
    VkInstance                                  instance,
    VkDebugReportFlagsEXT                       flags,
    VkDebugReportObjectTypeEXT                  objectType,
    uint64_t                                    object,
    size_t                                      location,
    int32_t                                     messageCode,
    const char*                                 pLayerPrefix,
    const char*                                 pMessage)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    PreCallDebugReportMessageEXT(instance_data, instance, flags, objectType, object, location, messageCode, pLayerPrefix, pMessage);
    instance_data->dispatch_table.DebugReportMessageEXT(instance, flags, objectType, object, location, messageCode, pLayerPrefix,
                                                        pMessage);
    PostCallDebugReportMessageEXT(instance_data, instance, flags, objectType, object, location, messageCode, pLayerPrefix,
                                  pMessage);
}







VKAPI_ATTR VkResult VKAPI_CALL DebugMarkerSetObjectTagEXT(
    VkDevice                                    device,
    VkDebugMarkerObjectTagInfoEXT*              pTagInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDebugMarkerSetObjectTagEXT(device_data, device, pTagInfo);
    VkResult result = device_data->dispatch_table.DebugMarkerSetObjectTagEXT(device, pTagInfo);
    PostCallDebugMarkerSetObjectTagEXT(device_data, device, pTagInfo);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL DebugMarkerSetObjectNameEXT(
    VkDevice                                    device,
    VkDebugMarkerObjectNameInfoEXT*             pNameInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDebugMarkerSetObjectNameEXT(device_data, device, pNameInfo);
    VkResult result = device_data->dispatch_table.DebugMarkerSetObjectNameEXT(device, pNameInfo);
    PostCallDebugMarkerSetObjectNameEXT(device_data, device, pNameInfo);
    return result;
}

VKAPI_ATTR void VKAPI_CALL CmdDebugMarkerBeginEXT(
    VkCommandBuffer                             commandBuffer,
    VkDebugMarkerMarkerInfoEXT*                 pMarkerInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdDebugMarkerBeginEXT(device_data, commandBuffer, pMarkerInfo);
    device_data->dispatch_table.CmdDebugMarkerBeginEXT(commandBuffer, pMarkerInfo);
    PostCallCmdDebugMarkerBeginEXT(device_data, commandBuffer, pMarkerInfo);
}

VKAPI_ATTR void VKAPI_CALL CmdDebugMarkerEndEXT(
    VkCommandBuffer                             commandBuffer)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdDebugMarkerEndEXT(device_data, commandBuffer);
    device_data->dispatch_table.CmdDebugMarkerEndEXT(commandBuffer);
    PostCallCmdDebugMarkerEndEXT(device_data, commandBuffer);
}

VKAPI_ATTR void VKAPI_CALL CmdDebugMarkerInsertEXT(
    VkCommandBuffer                             commandBuffer,
    VkDebugMarkerMarkerInfoEXT*                 pMarkerInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdDebugMarkerInsertEXT(device_data, commandBuffer, pMarkerInfo);
    device_data->dispatch_table.CmdDebugMarkerInsertEXT(commandBuffer, pMarkerInfo);
    PostCallCmdDebugMarkerInsertEXT(device_data, commandBuffer, pMarkerInfo);
}




VKAPI_ATTR void VKAPI_CALL CmdDrawIndirectCountAMD(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    VkBuffer                                    countBuffer,
    VkDeviceSize                                countBufferOffset,
    uint32_t                                    maxDrawCount,
    uint32_t                                    stride)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdDrawIndirectCountAMD(device_data, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount,
                                   stride);
    device_data->dispatch_table.CmdDrawIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount,
                                                        stride);
    PostCallCmdDrawIndirectCountAMD(device_data, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount,
                                    stride);
}

VKAPI_ATTR void VKAPI_CALL CmdDrawIndexedIndirectCountAMD(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    VkBuffer                                    countBuffer,
    VkDeviceSize                                countBufferOffset,
    uint32_t                                    maxDrawCount,
    uint32_t                                    stride)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdDrawIndexedIndirectCountAMD(device_data, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount,
                                          stride);
    device_data->dispatch_table.CmdDrawIndexedIndirectCountAMD(commandBuffer, buffer, offset, countBuffer, countBufferOffset,
                                                               maxDrawCount, stride);
    PostCallCmdDrawIndexedIndirectCountAMD(device_data, commandBuffer, buffer, offset, countBuffer, countBufferOffset, maxDrawCount,
                                           stride);
}








VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceExternalImageFormatPropertiesNV(
    VkPhysicalDevice                            physicalDevice,
    VkFormat                                    format,
    VkImageType                                 type,
    VkImageTiling                               tiling,
    VkImageUsageFlags                           usage,
    VkImageCreateFlags                          flags,
    VkExternalMemoryHandleTypeFlagsNV           externalHandleType,
    VkExternalImageFormatPropertiesNV*          pExternalImageFormatProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceExternalImageFormatPropertiesNV(instance_data, physicalDevice, format, type, tiling, usage, flags,
                                                            externalHandleType, pExternalImageFormatProperties);
    VkResult result = instance_data->dispatch_table.GetPhysicalDeviceExternalImageFormatPropertiesNV(
        physicalDevice, format, type, tiling, usage, flags, externalHandleType, pExternalImageFormatProperties);
    PostCallGetPhysicalDeviceExternalImageFormatPropertiesNV(instance_data, physicalDevice, format, type, tiling, usage, flags,
                                                             externalHandleType, pExternalImageFormatProperties);
    return result;
}


#ifdef VK_USE_PLATFORM_WIN32_KHR

VKAPI_ATTR VkResult VKAPI_CALL GetMemoryWin32HandleNV(
    VkDevice                                    device,
    VkDeviceMemory                              memory,
    VkExternalMemoryHandleTypeFlagsNV           handleType,
    HANDLE*                                     pHandle)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetMemoryWin32HandleNV(device_data, device, memory, handleType, pHandle);
    VkResult result = device_data->dispatch_table.GetMemoryWin32HandleNV(device, memory, handleType, pHandle);
    PostCallGetMemoryWin32HandleNV(device_data, device, memory, handleType, pHandle);
    return result;
}
#endif /* VK_USE_PLATFORM_WIN32_KHR */

#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /* VK_USE_PLATFORM_WIN32_KHR */


VKAPI_ATTR void VKAPI_CALL GetDeviceGroupPeerMemoryFeaturesKHX(
    VkDevice                                    device,
    uint32_t                                    heapIndex,
    uint32_t                                    localDeviceIndex,
    uint32_t                                    remoteDeviceIndex,
    VkPeerMemoryFeatureFlagsKHX*                pPeerMemoryFeatures)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetDeviceGroupPeerMemoryFeaturesKHX(device_data, device, heapIndex, localDeviceIndex, remoteDeviceIndex,
                                               pPeerMemoryFeatures);
    device_data->dispatch_table.GetDeviceGroupPeerMemoryFeaturesKHX(device, heapIndex, localDeviceIndex, remoteDeviceIndex,
                                                                    pPeerMemoryFeatures);
    PostCallGetDeviceGroupPeerMemoryFeaturesKHX(device_data, device, heapIndex, localDeviceIndex, remoteDeviceIndex,
                                                pPeerMemoryFeatures);
}

VKAPI_ATTR VkResult VKAPI_CALL BindBufferMemory2KHX(
    VkDevice                                    device,
    uint32_t                                    bindInfoCount,
    const VkBindBufferMemoryInfoKHX*            pBindInfos)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallBindBufferMemory2KHX(device_data, device, bindInfoCount, pBindInfos);
    VkResult result = device_data->dispatch_table.BindBufferMemory2KHX(device, bindInfoCount, pBindInfos);
    PostCallBindBufferMemory2KHX(device_data, device, bindInfoCount, pBindInfos);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL BindImageMemory2KHX(
    VkDevice                                    device,
    uint32_t                                    bindInfoCount,
    const VkBindImageMemoryInfoKHX*             pBindInfos)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallBindImageMemory2KHX(device_data, device, bindInfoCount, pBindInfos);
    VkResult result = device_data->dispatch_table.BindImageMemory2KHX(device, bindInfoCount, pBindInfos);
    PostCallBindImageMemory2KHX(device_data, device, bindInfoCount, pBindInfos);
    return result;
}

VKAPI_ATTR void VKAPI_CALL CmdSetDeviceMaskKHX(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    deviceMask)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdSetDeviceMaskKHX(device_data, commandBuffer, deviceMask);
    device_data->dispatch_table.CmdSetDeviceMaskKHX(commandBuffer, deviceMask);
    PostCallCmdSetDeviceMaskKHX(device_data, commandBuffer, deviceMask);
}

VKAPI_ATTR VkResult VKAPI_CALL GetDeviceGroupPresentCapabilitiesKHX(
    VkDevice                                    device,
    VkDeviceGroupPresentCapabilitiesKHX*        pDeviceGroupPresentCapabilities)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetDeviceGroupPresentCapabilitiesKHX(device_data, device, pDeviceGroupPresentCapabilities);
    VkResult result = device_data->dispatch_table.GetDeviceGroupPresentCapabilitiesKHX(device, pDeviceGroupPresentCapabilities);
    PostCallGetDeviceGroupPresentCapabilitiesKHX(device_data, device, pDeviceGroupPresentCapabilities);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetDeviceGroupSurfacePresentModesKHX(
    VkDevice                                    device,
    VkSurfaceKHR                                surface,
    VkDeviceGroupPresentModeFlagsKHX*           pModes)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetDeviceGroupSurfacePresentModesKHX(device_data, device, surface, pModes);
    VkResult result = device_data->dispatch_table.GetDeviceGroupSurfacePresentModesKHX(device, surface, pModes);
    PostCallGetDeviceGroupSurfacePresentModesKHX(device_data, device, surface, pModes);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL AcquireNextImage2KHX(
    VkDevice                                    device,
    const VkAcquireNextImageInfoKHX*            pAcquireInfo,
    uint32_t*                                   pImageIndex)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallAcquireNextImage2KHX(device_data, device, pAcquireInfo, pImageIndex);
    VkResult result = device_data->dispatch_table.AcquireNextImage2KHX(device, pAcquireInfo, pImageIndex);
    PostCallAcquireNextImage2KHX(device_data, device, pAcquireInfo, pImageIndex);
    return result;
}

VKAPI_ATTR void VKAPI_CALL CmdDispatchBaseKHX(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    baseGroupX,
    uint32_t                                    baseGroupY,
    uint32_t                                    baseGroupZ,
    uint32_t                                    groupCountX,
    uint32_t                                    groupCountY,
    uint32_t                                    groupCountZ)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdDispatchBaseKHX(device_data, commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY,
                              groupCountZ);
    device_data->dispatch_table.CmdDispatchBaseKHX(commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY,
                                                   groupCountZ);
    PostCallCmdDispatchBaseKHX(device_data, commandBuffer, baseGroupX, baseGroupY, baseGroupZ, groupCountX, groupCountY,
                               groupCountZ);
}

VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDevicePresentRectanglesKHX(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    uint32_t*                                   pRectCount,
    VkRect2D*                                   pRects)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDevicePresentRectanglesKHX(instance_data, physicalDevice, surface, pRectCount, pRects);
    VkResult result =
        instance_data->dispatch_table.GetPhysicalDevicePresentRectanglesKHX(physicalDevice, surface, pRectCount, pRects);
    PostCallGetPhysicalDevicePresentRectanglesKHX(instance_data, physicalDevice, surface, pRectCount, pRects);
    return result;
}


#ifdef VK_USE_PLATFORM_VI_NN

VKAPI_ATTR VkResult VKAPI_CALL CreateViSurfaceNN(
    VkInstance                                  instance,
    const VkViSurfaceCreateInfoNN*              pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSurfaceKHR*                               pSurface)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    PreCallCreateViSurfaceNN(instance_data, instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_data->dispatch_table.CreateViSurfaceNN(instance, pCreateInfo, pAllocator, pSurface);
    PostCallCreateViSurfaceNN(instance_data, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}
#endif /* VK_USE_PLATFORM_VI_NN */




VKAPI_ATTR VkResult VKAPI_CALL EnumeratePhysicalDeviceGroupsKHX(
    VkInstance                                  instance,
    uint32_t*                                   pPhysicalDeviceGroupCount,
    VkPhysicalDeviceGroupPropertiesKHX*         pPhysicalDeviceGroupProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    PreCallEnumeratePhysicalDeviceGroupsKHX(instance_data, instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
    VkResult result = instance_data->dispatch_table.EnumeratePhysicalDeviceGroupsKHX(instance, pPhysicalDeviceGroupCount,
                                                                                     pPhysicalDeviceGroupProperties);
    PostCallEnumeratePhysicalDeviceGroupsKHX(instance_data, instance, pPhysicalDeviceGroupCount, pPhysicalDeviceGroupProperties);
    return result;
}


VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceExternalBufferPropertiesKHX(
    VkPhysicalDevice                            physicalDevice,
    const VkPhysicalDeviceExternalBufferInfoKHX* pExternalBufferInfo,
    VkExternalBufferPropertiesKHX*              pExternalBufferProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceExternalBufferPropertiesKHX(instance_data, physicalDevice, pExternalBufferInfo,
                                                        pExternalBufferProperties);
    instance_data->dispatch_table.GetPhysicalDeviceExternalBufferPropertiesKHX(physicalDevice, pExternalBufferInfo,
                                                                               pExternalBufferProperties);
    PostCallGetPhysicalDeviceExternalBufferPropertiesKHX(instance_data, physicalDevice, pExternalBufferInfo,
                                                         pExternalBufferProperties);
}


#ifdef VK_USE_PLATFORM_WIN32_KHX

VKAPI_ATTR VkResult VKAPI_CALL GetMemoryWin32HandleKHX(
    VkDevice                                    device,
    VkDeviceMemory                              memory,
    VkExternalMemoryHandleTypeFlagBitsKHX       handleType,
    HANDLE*                                     pHandle)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetMemoryWin32HandleKHX(device_data, device, memory, handleType, pHandle);
    VkResult result = device_data->dispatch_table.GetMemoryWin32HandleKHX(device, memory, handleType, pHandle);
    PostCallGetMemoryWin32HandleKHX(device_data, device, memory, handleType, pHandle);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetMemoryWin32HandlePropertiesKHX(
    VkDevice                                    device,
    VkExternalMemoryHandleTypeFlagBitsKHX       handleType,
    HANDLE                                      handle,
    VkMemoryWin32HandlePropertiesKHX*           pMemoryWin32HandleProperties)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetMemoryWin32HandlePropertiesKHX(device_data, device, handleType, handle, pMemoryWin32HandleProperties);
    VkResult result =
        device_data->dispatch_table.GetMemoryWin32HandlePropertiesKHX(device, handleType, handle, pMemoryWin32HandleProperties);
    PostCallGetMemoryWin32HandlePropertiesKHX(device_data, device, handleType, handle, pMemoryWin32HandleProperties);
    return result;
}
#endif /* VK_USE_PLATFORM_WIN32_KHX */


VKAPI_ATTR VkResult VKAPI_CALL GetMemoryFdKHX(
    VkDevice                                    device,
    VkDeviceMemory                              memory,
    VkExternalMemoryHandleTypeFlagBitsKHX       handleType,
    int*                                        pFd)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetMemoryFdKHX(device_data, device, memory, handleType, pFd);
    VkResult result = device_data->dispatch_table.GetMemoryFdKHX(device, memory, handleType, pFd);
    PostCallGetMemoryFdKHX(device_data, device, memory, handleType, pFd);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetMemoryFdPropertiesKHX(
    VkDevice                                    device,
    VkExternalMemoryHandleTypeFlagBitsKHX       handleType,
    int                                         fd,
    VkMemoryFdPropertiesKHX*                    pMemoryFdProperties)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetMemoryFdPropertiesKHX(device_data, device, handleType, fd, pMemoryFdProperties);
    VkResult result = device_data->dispatch_table.GetMemoryFdPropertiesKHX(device, handleType, fd, pMemoryFdProperties);
    PostCallGetMemoryFdPropertiesKHX(device_data, device, handleType, fd, pMemoryFdProperties);
    return result;
}

#ifdef VK_USE_PLATFORM_WIN32_KHR
#endif /* VK_USE_PLATFORM_WIN32_KHR */


VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceExternalSemaphorePropertiesKHX(
    VkPhysicalDevice                            physicalDevice,
    const VkPhysicalDeviceExternalSemaphoreInfoKHX* pExternalSemaphoreInfo,
    VkExternalSemaphorePropertiesKHX*           pExternalSemaphoreProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceExternalSemaphorePropertiesKHX(instance_data, physicalDevice, pExternalSemaphoreInfo,
                                                           pExternalSemaphoreProperties);
    instance_data->dispatch_table.GetPhysicalDeviceExternalSemaphorePropertiesKHX(physicalDevice, pExternalSemaphoreInfo,
                                                                                  pExternalSemaphoreProperties);
    PostCallGetPhysicalDeviceExternalSemaphorePropertiesKHX(instance_data, physicalDevice, pExternalSemaphoreInfo,
                                                            pExternalSemaphoreProperties);
}

#ifdef VK_USE_PLATFORM_WIN32_KHX

VKAPI_ATTR VkResult VKAPI_CALL ImportSemaphoreWin32HandleKHX(
    VkDevice                                    device,
    const VkImportSemaphoreWin32HandleInfoKHX*  pImportSemaphoreWin32HandleInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallImportSemaphoreWin32HandleKHX(device_data, device, pImportSemaphoreWin32HandleInfo);
    VkResult result = device_data->dispatch_table.ImportSemaphoreWin32HandleKHX(device, pImportSemaphoreWin32HandleInfo);
    PostCallImportSemaphoreWin32HandleKHX(device_data, device, pImportSemaphoreWin32HandleInfo);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetSemaphoreWin32HandleKHX(
    VkDevice                                    device,
    VkSemaphore                                 semaphore,
    VkExternalSemaphoreHandleTypeFlagBitsKHX    handleType,
    HANDLE*                                     pHandle)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetSemaphoreWin32HandleKHX(device_data, device, semaphore, handleType, pHandle);
    VkResult result = device_data->dispatch_table.GetSemaphoreWin32HandleKHX(device, semaphore, handleType, pHandle);
    PostCallGetSemaphoreWin32HandleKHX(device_data, device, semaphore, handleType, pHandle);
    return result;
}
#endif /* VK_USE_PLATFORM_WIN32_KHX */

VKAPI_ATTR VkResult VKAPI_CALL ImportSemaphoreFdKHX(
    VkDevice                                    device,
    const VkImportSemaphoreFdInfoKHX*           pImportSemaphoreFdInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallImportSemaphoreFdKHX(device_data, device, pImportSemaphoreFdInfo);
    VkResult result = device_data->dispatch_table.ImportSemaphoreFdKHX(device, pImportSemaphoreFdInfo);
    PostCallImportSemaphoreFdKHX(device_data, device, pImportSemaphoreFdInfo);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetSemaphoreFdKHX(
    VkDevice                                    device,
    VkSemaphore                                 semaphore,
    VkExternalSemaphoreHandleTypeFlagBitsKHX    handleType,
    int*                                        pFd)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetSemaphoreFdKHX(device_data, device, semaphore, handleType, pFd);
    VkResult result = device_data->dispatch_table.GetSemaphoreFdKHX(device, semaphore, handleType, pFd);
    PostCallGetSemaphoreFdKHX(device_data, device, semaphore, handleType, pFd);
    return result;
}


VKAPI_ATTR void VKAPI_CALL CmdProcessCommandsNVX(
    VkCommandBuffer                             commandBuffer,
    const VkCmdProcessCommandsInfoNVX*          pProcessCommandsInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdProcessCommandsNVX(device_data, commandBuffer, pProcessCommandsInfo);
    device_data->dispatch_table.CmdProcessCommandsNVX(commandBuffer, pProcessCommandsInfo);
    PostCallCmdProcessCommandsNVX(device_data, commandBuffer, pProcessCommandsInfo);
}

VKAPI_ATTR void VKAPI_CALL CmdReserveSpaceForCommandsNVX(
    VkCommandBuffer                             commandBuffer,
    const VkCmdReserveSpaceForCommandsInfoNVX*  pReserveSpaceInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdReserveSpaceForCommandsNVX(device_data, commandBuffer, pReserveSpaceInfo);
    device_data->dispatch_table.CmdReserveSpaceForCommandsNVX(commandBuffer, pReserveSpaceInfo);
    PostCallCmdReserveSpaceForCommandsNVX(device_data, commandBuffer, pReserveSpaceInfo);
}

VKAPI_ATTR VkResult VKAPI_CALL CreateIndirectCommandsLayoutNVX(
    VkDevice                                    device,
    const VkIndirectCommandsLayoutCreateInfoNVX* pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkIndirectCommandsLayoutNVX*                pIndirectCommandsLayout)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateIndirectCommandsLayoutNVX(device_data, device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
    VkResult result =
        device_data->dispatch_table.CreateIndirectCommandsLayoutNVX(device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
    PostCallCreateIndirectCommandsLayoutNVX(device_data, device, pCreateInfo, pAllocator, pIndirectCommandsLayout);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyIndirectCommandsLayoutNVX(
    VkDevice                                    device,
    VkIndirectCommandsLayoutNVX                 indirectCommandsLayout,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroyIndirectCommandsLayoutNVX(device_data, device, indirectCommandsLayout, pAllocator);
    device_data->dispatch_table.DestroyIndirectCommandsLayoutNVX(device, indirectCommandsLayout, pAllocator);
    PostCallDestroyIndirectCommandsLayoutNVX(device_data, device, indirectCommandsLayout, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL CreateObjectTableNVX(
    VkDevice                                    device,
    const VkObjectTableCreateInfoNVX*           pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkObjectTableNVX*                           pObjectTable)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallCreateObjectTableNVX(device_data, device, pCreateInfo, pAllocator, pObjectTable);
    VkResult result = device_data->dispatch_table.CreateObjectTableNVX(device, pCreateInfo, pAllocator, pObjectTable);
    PostCallCreateObjectTableNVX(device_data, device, pCreateInfo, pAllocator, pObjectTable);
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyObjectTableNVX(
    VkDevice                                    device,
    VkObjectTableNVX                            objectTable,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDestroyObjectTableNVX(device_data, device, objectTable, pAllocator);
    device_data->dispatch_table.DestroyObjectTableNVX(device, objectTable, pAllocator);
    PostCallDestroyObjectTableNVX(device_data, device, objectTable, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL RegisterObjectsNVX(
    VkDevice                                    device,
    VkObjectTableNVX                            objectTable,
    uint32_t                                    objectCount,
    const VkObjectTableEntryNVX* const*         ppObjectTableEntries,
    const uint32_t*                             pObjectIndices)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallRegisterObjectsNVX(device_data, device, objectTable, objectCount, ppObjectTableEntries, pObjectIndices);
    VkResult result =
        device_data->dispatch_table.RegisterObjectsNVX(device, objectTable, objectCount, ppObjectTableEntries, pObjectIndices);
    PostCallRegisterObjectsNVX(device_data, device, objectTable, objectCount, ppObjectTableEntries, pObjectIndices);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL UnregisterObjectsNVX(
    VkDevice                                    device,
    VkObjectTableNVX                            objectTable,
    uint32_t                                    objectCount,
    const VkObjectEntryTypeNVX*                 pObjectEntryTypes,
    const uint32_t*                             pObjectIndices)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallUnregisterObjectsNVX(device_data, device, objectTable, objectCount, pObjectEntryTypes, pObjectIndices);
    VkResult result =
        device_data->dispatch_table.UnregisterObjectsNVX(device, objectTable, objectCount, pObjectEntryTypes, pObjectIndices);
    PostCallUnregisterObjectsNVX(device_data, device, objectTable, objectCount, pObjectEntryTypes, pObjectIndices);
    return result;
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceGeneratedCommandsPropertiesNVX(
    VkPhysicalDevice                            physicalDevice,
    VkDeviceGeneratedCommandsFeaturesNVX*       pFeatures,
    VkDeviceGeneratedCommandsLimitsNVX*         pLimits)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceGeneratedCommandsPropertiesNVX(instance_data, physicalDevice, pFeatures, pLimits);
    instance_data->dispatch_table.GetPhysicalDeviceGeneratedCommandsPropertiesNVX(physicalDevice, pFeatures, pLimits);
    PostCallGetPhysicalDeviceGeneratedCommandsPropertiesNVX(instance_data, physicalDevice, pFeatures, pLimits);
}


VKAPI_ATTR void VKAPI_CALL CmdSetViewportWScalingNV(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstViewport,
    uint32_t                                    viewportCount,
    const VkViewportWScalingNV*                 pViewportWScalings)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdSetViewportWScalingNV(device_data, commandBuffer, firstViewport, viewportCount, pViewportWScalings);
    device_data->dispatch_table.CmdSetViewportWScalingNV(commandBuffer, firstViewport, viewportCount, pViewportWScalings);
    PostCallCmdSetViewportWScalingNV(device_data, commandBuffer, firstViewport, viewportCount, pViewportWScalings);
}


VKAPI_ATTR VkResult VKAPI_CALL ReleaseDisplayEXT(
    VkPhysicalDevice                            physicalDevice,
    VkDisplayKHR                                display)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallReleaseDisplayEXT(instance_data, physicalDevice, display);
    VkResult result = instance_data->dispatch_table.ReleaseDisplayEXT(physicalDevice, display);
    PostCallReleaseDisplayEXT(instance_data, physicalDevice, display);
    return result;
}

#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT

VKAPI_ATTR VkResult VKAPI_CALL AcquireXlibDisplayEXT(
    VkPhysicalDevice                            physicalDevice,
    Display*                                    dpy,
    VkDisplayKHR                                display)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallAcquireXlibDisplayEXT(instance_data, physicalDevice, dpy, display);
    VkResult result = instance_data->dispatch_table.AcquireXlibDisplayEXT(physicalDevice, dpy, display);
    PostCallAcquireXlibDisplayEXT(instance_data, physicalDevice, dpy, display);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetRandROutputDisplayEXT(
    VkPhysicalDevice                            physicalDevice,
    Display*                                    dpy,
    RROutput                                    rrOutput,
    VkDisplayKHR*                               pDisplay)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetRandROutputDisplayEXT(instance_data, physicalDevice, dpy, rrOutput, pDisplay);
    VkResult result = instance_data->dispatch_table.GetRandROutputDisplayEXT(physicalDevice, dpy, rrOutput, pDisplay);
    PostCallGetRandROutputDisplayEXT(instance_data, physicalDevice, dpy, rrOutput, pDisplay);
    return result;
}
#endif /* VK_USE_PLATFORM_XLIB_XRANDR_EXT */


VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceSurfaceCapabilities2EXT(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    VkSurfaceCapabilities2EXT*                  pSurfaceCapabilities)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    PreCallGetPhysicalDeviceSurfaceCapabilities2EXT(instance_data, physicalDevice, surface, pSurfaceCapabilities);
    VkResult result =
        instance_data->dispatch_table.GetPhysicalDeviceSurfaceCapabilities2EXT(physicalDevice, surface, pSurfaceCapabilities);
    PostCallGetPhysicalDeviceSurfaceCapabilities2EXT(instance_data, physicalDevice, surface, pSurfaceCapabilities);
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL DisplayPowerControlEXT(
    VkDevice                                    device,
    VkDisplayKHR                                display,
    const VkDisplayPowerInfoEXT*                pDisplayPowerInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallDisplayPowerControlEXT(device_data, device, display, pDisplayPowerInfo);
    VkResult result = device_data->dispatch_table.DisplayPowerControlEXT(device, display, pDisplayPowerInfo);
    PostCallDisplayPowerControlEXT(device_data, device, display, pDisplayPowerInfo);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL RegisterDeviceEventEXT(
    VkDevice                                    device,
    const VkDeviceEventInfoEXT*                 pDeviceEventInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkFence*                                    pFence)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallRegisterDeviceEventEXT(device_data, device, pDeviceEventInfo, pAllocator, pFence);
    VkResult result = device_data->dispatch_table.RegisterDeviceEventEXT(device, pDeviceEventInfo, pAllocator, pFence);
    PostCallRegisterDeviceEventEXT(device_data, device, pDeviceEventInfo, pAllocator, pFence);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL RegisterDisplayEventEXT(
    VkDevice                                    device,
    VkDisplayKHR                                display,
    const VkDisplayEventInfoEXT*                pDisplayEventInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkFence*                                    pFence)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallRegisterDisplayEventEXT(device_data, device, display, pDisplayEventInfo, pAllocator, pFence);
    VkResult result = device_data->dispatch_table.RegisterDisplayEventEXT(device, display, pDisplayEventInfo, pAllocator, pFence);
    PostCallRegisterDisplayEventEXT(device_data, device, display, pDisplayEventInfo, pAllocator, pFence);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetSwapchainCounterEXT(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain,
    VkSurfaceCounterFlagBitsEXT                 counter,
    uint64_t*                                   pCounterValue)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetSwapchainCounterEXT(device_data, device, swapchain, counter, pCounterValue);
    VkResult result = device_data->dispatch_table.GetSwapchainCounterEXT(device, swapchain, counter, pCounterValue);
    PostCallGetSwapchainCounterEXT(device_data, device, swapchain, counter, pCounterValue);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetRefreshCycleDurationGOOGLE(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain,
    VkRefreshCycleDurationGOOGLE*               pDisplayTimingProperties)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetRefreshCycleDurationGOOGLE(device_data, device, swapchain, pDisplayTimingProperties);
    VkResult result = device_data->dispatch_table.GetRefreshCycleDurationGOOGLE(device, swapchain, pDisplayTimingProperties);
    PostCallGetRefreshCycleDurationGOOGLE(device_data, device, swapchain, pDisplayTimingProperties);
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetPastPresentationTimingGOOGLE(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain,
    uint32_t*                                   pPresentationTimingCount,
    VkPastPresentationTimingGOOGLE*             pPresentationTimings)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallGetPastPresentationTimingGOOGLE(device_data, device, swapchain, pPresentationTimingCount, pPresentationTimings);
    VkResult result = device_data->dispatch_table.GetPastPresentationTimingGOOGLE(device, swapchain, pPresentationTimingCount,
                                                                                  pPresentationTimings);
    PostCallGetPastPresentationTimingGOOGLE(device_data, device, swapchain, pPresentationTimingCount, pPresentationTimings);
    return result;
}

VKAPI_ATTR void VKAPI_CALL CmdSetDiscardRectangleEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstDiscardRectangle,
    uint32_t                                    discardRectangleCount,
    const VkRect2D*                             pDiscardRectangles)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    PreCallCmdSetDiscardRectangleEXT(device_data, commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
    device_data->dispatch_table.CmdSetDiscardRectangleEXT(commandBuffer, firstDiscardRectangle, discardRectangleCount,
                                                          pDiscardRectangles);
    PostCallCmdSetDiscardRectangleEXT(device_data, commandBuffer, firstDiscardRectangle, discardRectangleCount, pDiscardRectangles);
}

VKAPI_ATTR void VKAPI_CALL SetHdrMetadataEXT(
    VkDevice                                    device,
    uint32_t                                    swapchainCount,
    const VkSwapchainKHR*                       pSwapchains,
    const VkHdrMetadataEXT*                     pMetadata)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    PreCallSetHdrMetadataEXT(device_data, device, swapchainCount, pSwapchains, pMetadata);
    device_data->dispatch_table.SetHdrMetadataEXT(device, swapchainCount, pSwapchains, pMetadata);
    PostCallSetHdrMetadataEXT(device_data, device, swapchainCount, pSwapchains, pMetadata);
}

#ifdef VK_USE_PLATFORM_IOS_MVK

VKAPI_ATTR VkResult VKAPI_CALL CreateIOSSurfaceMVK(
    VkInstance                                  instance,
    const VkIOSSurfaceCreateInfoMVK*            pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSurfaceKHR*                               pSurface)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    PreCallCreateIOSSurfaceMVK(instance_data, instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_data->dispatch_table.CreateIOSSurfaceMVK(instance, pCreateInfo, pAllocator, pSurface);
    PostCallCreateIOSSurfaceMVK(instance_data, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}
#endif /* VK_USE_PLATFORM_IOS_MVK */

#ifdef VK_USE_PLATFORM_MACOS_MVK

VKAPI_ATTR VkResult VKAPI_CALL CreateMacOSSurfaceMVK(
    VkInstance                                  instance,
    const VkMacOSSurfaceCreateInfoMVK*          pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSurfaceKHR*                               pSurface)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    PreCallCreateMacOSSurfaceMVK(instance_data, instance, pCreateInfo, pAllocator, pSurface);
    VkResult result = instance_data->dispatch_table.CreateMacOSSurfaceMVK(instance, pCreateInfo, pAllocator, pSurface);
    PostCallCreateMacOSSurfaceMVK(instance_data, instance, pCreateInfo, pAllocator, pSurface);
    return result;
}
#endif /* VK_USE_PLATFORM_MACOS_MVK */

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL GetPhysicalDeviceProcAddr(VkInstance instance, const char *funcName) {
    assert(instance);
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);

    const auto &table = instance_data->dispatch_table;
    if (!table.GetPhysicalDeviceProcAddr) return nullptr;
    return table.GetPhysicalDeviceProcAddr(instance, funcName);
}

} // namespace vkaid

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetDeviceProcAddr(VkDevice dev, const char *funcName) {
    return vkaid::GetDeviceProcAddr(dev, funcName);
}

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vkGetInstanceProcAddr(VkInstance instance, const char *funcName) {
    return vkaid::GetInstanceProcAddr(instance, funcName);
}

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL vk_layerGetPhysicalDeviceProcAddr(VkInstance instance,
                                                                                           const char *funcName) {
    return vkaid::GetPhysicalDeviceProcAddr(instance, funcName);
}

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL vkNegotiateLoaderLayerInterfaceVersion(VkNegotiateLayerInterface *pVersionStruct) {
    assert(pVersionStruct != NULL);
    assert(pVersionStruct->sType == LAYER_NEGOTIATE_INTERFACE_STRUCT);

    // Fill in the function pointers if our version is at least capable of having the structure contain them.
    if (pVersionStruct->loaderLayerInterfaceVersion >= 2) {
        pVersionStruct->pfnGetInstanceProcAddr = vkGetInstanceProcAddr;
        pVersionStruct->pfnGetDeviceProcAddr = vkGetDeviceProcAddr;
        pVersionStruct->pfnGetPhysicalDeviceProcAddr = vk_layerGetPhysicalDeviceProcAddr;
    }

    if (pVersionStruct->loaderLayerInterfaceVersion < CURRENT_LOADER_LAYER_INTERFACE_VERSION) {
        vkaid::loader_layer_if_version = pVersionStruct->loaderLayerInterfaceVersion;
    } else if (pVersionStruct->loaderLayerInterfaceVersion > CURRENT_LOADER_LAYER_INTERFACE_VERSION) {
        pVersionStruct->loaderLayerInterfaceVersion = CURRENT_LOADER_LAYER_INTERFACE_VERSION;
    }

    return VK_SUCCESS;
}