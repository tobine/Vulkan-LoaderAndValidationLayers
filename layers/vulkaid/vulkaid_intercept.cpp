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
    //PreCallvkEnumeratePhysicalDevices();
    VkResult result = instance_data->dispatch_table.EnumeratePhysicalDevices(instance,pPhysicalDeviceCount,pPhysicalDevices);
    //PostCallvkEnumeratePhysicalDevices();
    return result;
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceFeatures(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceFeatures*                   pFeatures)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceFeatures();
    instance_data->dispatch_table.GetPhysicalDeviceFeatures(physicalDevice,pFeatures);
    //PostCallvkGetPhysicalDeviceFeatures();
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceFormatProperties(
    VkPhysicalDevice                            physicalDevice,
    VkFormat                                    format,
    VkFormatProperties*                         pFormatProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceFormatProperties();
    instance_data->dispatch_table.GetPhysicalDeviceFormatProperties(physicalDevice,format,pFormatProperties);
    //PostCallvkGetPhysicalDeviceFormatProperties();
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
    //PreCallvkGetPhysicalDeviceImageFormatProperties();
    VkResult result = instance_data->dispatch_table.GetPhysicalDeviceImageFormatProperties(physicalDevice,format,type,tiling,usage,flags,pImageFormatProperties);
    //PostCallvkGetPhysicalDeviceImageFormatProperties();
    return result;
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceProperties(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceProperties*                 pProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceProperties();
    instance_data->dispatch_table.GetPhysicalDeviceProperties(physicalDevice,pProperties);
    //PostCallvkGetPhysicalDeviceProperties();
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceQueueFamilyProperties(
    VkPhysicalDevice                            physicalDevice,
    uint32_t*                                   pQueueFamilyPropertyCount,
    VkQueueFamilyProperties*                    pQueueFamilyProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceQueueFamilyProperties();
    instance_data->dispatch_table.GetPhysicalDeviceQueueFamilyProperties(physicalDevice,pQueueFamilyPropertyCount,pQueueFamilyProperties);
    //PostCallvkGetPhysicalDeviceQueueFamilyProperties();
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceMemoryProperties(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceMemoryProperties*           pMemoryProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceMemoryProperties();
    instance_data->dispatch_table.GetPhysicalDeviceMemoryProperties(physicalDevice,pMemoryProperties);
    //PostCallvkGetPhysicalDeviceMemoryProperties();
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
    //PreCallvkGetDeviceQueue();
    device_data->dispatch_table.GetDeviceQueue(device,queueFamilyIndex,queueIndex,pQueue);
    //PostCallvkGetDeviceQueue();
}

VKAPI_ATTR VkResult VKAPI_CALL QueueSubmit(
    VkQueue                                     queue,
    uint32_t                                    submitCount,
    const VkSubmitInfo*                         pSubmits,
    VkFence                                     fence)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(queue), device_layer_data_map);
    //PreCallvkQueueSubmit();
    VkResult result = device_data->dispatch_table.QueueSubmit(queue,submitCount,pSubmits,fence);
    //PostCallvkQueueSubmit();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL QueueWaitIdle(
    VkQueue                                     queue)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(queue), device_layer_data_map);
    //PreCallvkQueueWaitIdle();
    VkResult result = device_data->dispatch_table.QueueWaitIdle(queue);
    //PostCallvkQueueWaitIdle();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL DeviceWaitIdle(
    VkDevice                                    device)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDeviceWaitIdle();
    VkResult result = device_data->dispatch_table.DeviceWaitIdle(device);
    //PostCallvkDeviceWaitIdle();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL AllocateMemory(
    VkDevice                                    device,
    const VkMemoryAllocateInfo*                 pAllocateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDeviceMemory*                             pMemory)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkAllocateMemory();
    VkResult result = device_data->dispatch_table.AllocateMemory(device,pAllocateInfo,pAllocator,pMemory);
    //PostCallvkAllocateMemory();
    return result;
}

VKAPI_ATTR void VKAPI_CALL FreeMemory(
    VkDevice                                    device,
    VkDeviceMemory                              memory,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkFreeMemory();
    device_data->dispatch_table.FreeMemory(device,memory,pAllocator);
    //PostCallvkFreeMemory();
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
    //PreCallvkMapMemory();
    VkResult result = device_data->dispatch_table.MapMemory(device,memory,offset,size,flags,ppData);
    //PostCallvkMapMemory();
    return result;
}

VKAPI_ATTR void VKAPI_CALL UnmapMemory(
    VkDevice                                    device,
    VkDeviceMemory                              memory)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkUnmapMemory();
    device_data->dispatch_table.UnmapMemory(device,memory);
    //PostCallvkUnmapMemory();
}

VKAPI_ATTR VkResult VKAPI_CALL FlushMappedMemoryRanges(
    VkDevice                                    device,
    uint32_t                                    memoryRangeCount,
    const VkMappedMemoryRange*                  pMemoryRanges)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkFlushMappedMemoryRanges();
    VkResult result = device_data->dispatch_table.FlushMappedMemoryRanges(device,memoryRangeCount,pMemoryRanges);
    //PostCallvkFlushMappedMemoryRanges();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL InvalidateMappedMemoryRanges(
    VkDevice                                    device,
    uint32_t                                    memoryRangeCount,
    const VkMappedMemoryRange*                  pMemoryRanges)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkInvalidateMappedMemoryRanges();
    VkResult result = device_data->dispatch_table.InvalidateMappedMemoryRanges(device,memoryRangeCount,pMemoryRanges);
    //PostCallvkInvalidateMappedMemoryRanges();
    return result;
}

VKAPI_ATTR void VKAPI_CALL GetDeviceMemoryCommitment(
    VkDevice                                    device,
    VkDeviceMemory                              memory,
    VkDeviceSize*                               pCommittedMemoryInBytes)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkGetDeviceMemoryCommitment();
    device_data->dispatch_table.GetDeviceMemoryCommitment(device,memory,pCommittedMemoryInBytes);
    //PostCallvkGetDeviceMemoryCommitment();
}

VKAPI_ATTR VkResult VKAPI_CALL BindBufferMemory(
    VkDevice                                    device,
    VkBuffer                                    buffer,
    VkDeviceMemory                              memory,
    VkDeviceSize                                memoryOffset)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkBindBufferMemory();
    VkResult result = device_data->dispatch_table.BindBufferMemory(device,buffer,memory,memoryOffset);
    //PostCallvkBindBufferMemory();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL BindImageMemory(
    VkDevice                                    device,
    VkImage                                     image,
    VkDeviceMemory                              memory,
    VkDeviceSize                                memoryOffset)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkBindImageMemory();
    VkResult result = device_data->dispatch_table.BindImageMemory(device,image,memory,memoryOffset);
    //PostCallvkBindImageMemory();
    return result;
}

VKAPI_ATTR void VKAPI_CALL GetBufferMemoryRequirements(
    VkDevice                                    device,
    VkBuffer                                    buffer,
    VkMemoryRequirements*                       pMemoryRequirements)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkGetBufferMemoryRequirements();
    device_data->dispatch_table.GetBufferMemoryRequirements(device,buffer,pMemoryRequirements);
    //PostCallvkGetBufferMemoryRequirements();
}

VKAPI_ATTR void VKAPI_CALL GetImageMemoryRequirements(
    VkDevice                                    device,
    VkImage                                     image,
    VkMemoryRequirements*                       pMemoryRequirements)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkGetImageMemoryRequirements();
    device_data->dispatch_table.GetImageMemoryRequirements(device,image,pMemoryRequirements);
    //PostCallvkGetImageMemoryRequirements();
}

VKAPI_ATTR void VKAPI_CALL GetImageSparseMemoryRequirements(
    VkDevice                                    device,
    VkImage                                     image,
    uint32_t*                                   pSparseMemoryRequirementCount,
    VkSparseImageMemoryRequirements*            pSparseMemoryRequirements)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkGetImageSparseMemoryRequirements();
    device_data->dispatch_table.GetImageSparseMemoryRequirements(device,image,pSparseMemoryRequirementCount,pSparseMemoryRequirements);
    //PostCallvkGetImageSparseMemoryRequirements();
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
    //PreCallvkGetPhysicalDeviceSparseImageFormatProperties();
    instance_data->dispatch_table.GetPhysicalDeviceSparseImageFormatProperties(physicalDevice,format,type,samples,usage,tiling,pPropertyCount,pProperties);
    //PostCallvkGetPhysicalDeviceSparseImageFormatProperties();
}

VKAPI_ATTR VkResult VKAPI_CALL QueueBindSparse(
    VkQueue                                     queue,
    uint32_t                                    bindInfoCount,
    const VkBindSparseInfo*                     pBindInfo,
    VkFence                                     fence)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(queue), device_layer_data_map);
    //PreCallvkQueueBindSparse();
    VkResult result = device_data->dispatch_table.QueueBindSparse(queue,bindInfoCount,pBindInfo,fence);
    //PostCallvkQueueBindSparse();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL CreateFence(
    VkDevice                                    device,
    const VkFenceCreateInfo*                    pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkFence*                                    pFence)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkCreateFence();
    VkResult result = device_data->dispatch_table.CreateFence(device,pCreateInfo,pAllocator,pFence);
    //PostCallvkCreateFence();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyFence(
    VkDevice                                    device,
    VkFence                                     fence,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroyFence();
    device_data->dispatch_table.DestroyFence(device,fence,pAllocator);
    //PostCallvkDestroyFence();
}

VKAPI_ATTR VkResult VKAPI_CALL ResetFences(
    VkDevice                                    device,
    uint32_t                                    fenceCount,
    const VkFence*                              pFences)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkResetFences();
    VkResult result = device_data->dispatch_table.ResetFences(device,fenceCount,pFences);
    //PostCallvkResetFences();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetFenceStatus(
    VkDevice                                    device,
    VkFence                                     fence)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkGetFenceStatus();
    VkResult result = device_data->dispatch_table.GetFenceStatus(device,fence);
    //PostCallvkGetFenceStatus();
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
    //PreCallvkWaitForFences();
    VkResult result = device_data->dispatch_table.WaitForFences(device,fenceCount,pFences,waitAll,timeout);
    //PostCallvkWaitForFences();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL CreateSemaphore(
    VkDevice                                    device,
    const VkSemaphoreCreateInfo*                pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSemaphore*                                pSemaphore)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkCreateSemaphore();
    VkResult result = device_data->dispatch_table.CreateSemaphore(device,pCreateInfo,pAllocator,pSemaphore);
    //PostCallvkCreateSemaphore();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroySemaphore(
    VkDevice                                    device,
    VkSemaphore                                 semaphore,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroySemaphore();
    device_data->dispatch_table.DestroySemaphore(device,semaphore,pAllocator);
    //PostCallvkDestroySemaphore();
}

VKAPI_ATTR VkResult VKAPI_CALL CreateEvent(
    VkDevice                                    device,
    const VkEventCreateInfo*                    pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkEvent*                                    pEvent)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkCreateEvent();
    VkResult result = device_data->dispatch_table.CreateEvent(device,pCreateInfo,pAllocator,pEvent);
    //PostCallvkCreateEvent();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyEvent(
    VkDevice                                    device,
    VkEvent                                     event,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroyEvent();
    device_data->dispatch_table.DestroyEvent(device,event,pAllocator);
    //PostCallvkDestroyEvent();
}

VKAPI_ATTR VkResult VKAPI_CALL GetEventStatus(
    VkDevice                                    device,
    VkEvent                                     event)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkGetEventStatus();
    VkResult result = device_data->dispatch_table.GetEventStatus(device,event);
    //PostCallvkGetEventStatus();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL SetEvent(
    VkDevice                                    device,
    VkEvent                                     event)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkSetEvent();
    VkResult result = device_data->dispatch_table.SetEvent(device,event);
    //PostCallvkSetEvent();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL ResetEvent(
    VkDevice                                    device,
    VkEvent                                     event)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkResetEvent();
    VkResult result = device_data->dispatch_table.ResetEvent(device,event);
    //PostCallvkResetEvent();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL CreateQueryPool(
    VkDevice                                    device,
    const VkQueryPoolCreateInfo*                pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkQueryPool*                                pQueryPool)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkCreateQueryPool();
    VkResult result = device_data->dispatch_table.CreateQueryPool(device,pCreateInfo,pAllocator,pQueryPool);
    //PostCallvkCreateQueryPool();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyQueryPool(
    VkDevice                                    device,
    VkQueryPool                                 queryPool,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroyQueryPool();
    device_data->dispatch_table.DestroyQueryPool(device,queryPool,pAllocator);
    //PostCallvkDestroyQueryPool();
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
    //PreCallvkGetQueryPoolResults();
    VkResult result = device_data->dispatch_table.GetQueryPoolResults(device,queryPool,firstQuery,queryCount,dataSize,pData,stride,flags);
    //PostCallvkGetQueryPoolResults();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL CreateBuffer(
    VkDevice                                    device,
    const VkBufferCreateInfo*                   pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkBuffer*                                   pBuffer)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkCreateBuffer();
    VkResult result = device_data->dispatch_table.CreateBuffer(device,pCreateInfo,pAllocator,pBuffer);
    //PostCallvkCreateBuffer();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyBuffer(
    VkDevice                                    device,
    VkBuffer                                    buffer,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroyBuffer();
    device_data->dispatch_table.DestroyBuffer(device,buffer,pAllocator);
    //PostCallvkDestroyBuffer();
}

VKAPI_ATTR VkResult VKAPI_CALL CreateBufferView(
    VkDevice                                    device,
    const VkBufferViewCreateInfo*               pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkBufferView*                               pView)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkCreateBufferView();
    VkResult result = device_data->dispatch_table.CreateBufferView(device,pCreateInfo,pAllocator,pView);
    //PostCallvkCreateBufferView();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyBufferView(
    VkDevice                                    device,
    VkBufferView                                bufferView,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroyBufferView();
    device_data->dispatch_table.DestroyBufferView(device,bufferView,pAllocator);
    //PostCallvkDestroyBufferView();
}

VKAPI_ATTR VkResult VKAPI_CALL CreateImage(
    VkDevice                                    device,
    const VkImageCreateInfo*                    pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkImage*                                    pImage)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkCreateImage();
    VkResult result = device_data->dispatch_table.CreateImage(device,pCreateInfo,pAllocator,pImage);
    //PostCallvkCreateImage();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyImage(
    VkDevice                                    device,
    VkImage                                     image,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroyImage();
    device_data->dispatch_table.DestroyImage(device,image,pAllocator);
    //PostCallvkDestroyImage();
}

VKAPI_ATTR void VKAPI_CALL GetImageSubresourceLayout(
    VkDevice                                    device,
    VkImage                                     image,
    const VkImageSubresource*                   pSubresource,
    VkSubresourceLayout*                        pLayout)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkGetImageSubresourceLayout();
    device_data->dispatch_table.GetImageSubresourceLayout(device,image,pSubresource,pLayout);
    //PostCallvkGetImageSubresourceLayout();
}

VKAPI_ATTR VkResult VKAPI_CALL CreateImageView(
    VkDevice                                    device,
    const VkImageViewCreateInfo*                pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkImageView*                                pView)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkCreateImageView();
    VkResult result = device_data->dispatch_table.CreateImageView(device,pCreateInfo,pAllocator,pView);
    //PostCallvkCreateImageView();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyImageView(
    VkDevice                                    device,
    VkImageView                                 imageView,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroyImageView();
    device_data->dispatch_table.DestroyImageView(device,imageView,pAllocator);
    //PostCallvkDestroyImageView();
}

VKAPI_ATTR VkResult VKAPI_CALL CreateShaderModule(
    VkDevice                                    device,
    const VkShaderModuleCreateInfo*             pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkShaderModule*                             pShaderModule)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkCreateShaderModule();
    VkResult result = device_data->dispatch_table.CreateShaderModule(device,pCreateInfo,pAllocator,pShaderModule);
    //PostCallvkCreateShaderModule();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyShaderModule(
    VkDevice                                    device,
    VkShaderModule                              shaderModule,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroyShaderModule();
    device_data->dispatch_table.DestroyShaderModule(device,shaderModule,pAllocator);
    //PostCallvkDestroyShaderModule();
}

VKAPI_ATTR VkResult VKAPI_CALL CreatePipelineCache(
    VkDevice                                    device,
    const VkPipelineCacheCreateInfo*            pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkPipelineCache*                            pPipelineCache)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkCreatePipelineCache();
    VkResult result = device_data->dispatch_table.CreatePipelineCache(device,pCreateInfo,pAllocator,pPipelineCache);
    //PostCallvkCreatePipelineCache();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyPipelineCache(
    VkDevice                                    device,
    VkPipelineCache                             pipelineCache,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroyPipelineCache();
    device_data->dispatch_table.DestroyPipelineCache(device,pipelineCache,pAllocator);
    //PostCallvkDestroyPipelineCache();
}

VKAPI_ATTR VkResult VKAPI_CALL GetPipelineCacheData(
    VkDevice                                    device,
    VkPipelineCache                             pipelineCache,
    size_t*                                     pDataSize,
    void*                                       pData)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkGetPipelineCacheData();
    VkResult result = device_data->dispatch_table.GetPipelineCacheData(device,pipelineCache,pDataSize,pData);
    //PostCallvkGetPipelineCacheData();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL MergePipelineCaches(
    VkDevice                                    device,
    VkPipelineCache                             dstCache,
    uint32_t                                    srcCacheCount,
    const VkPipelineCache*                      pSrcCaches)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkMergePipelineCaches();
    VkResult result = device_data->dispatch_table.MergePipelineCaches(device,dstCache,srcCacheCount,pSrcCaches);
    //PostCallvkMergePipelineCaches();
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
    //PreCallvkCreateGraphicsPipelines();
    VkResult result = device_data->dispatch_table.CreateGraphicsPipelines(device,pipelineCache,createInfoCount,pCreateInfos,pAllocator,pPipelines);
    //PostCallvkCreateGraphicsPipelines();
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
    //PreCallvkCreateComputePipelines();
    VkResult result = device_data->dispatch_table.CreateComputePipelines(device,pipelineCache,createInfoCount,pCreateInfos,pAllocator,pPipelines);
    //PostCallvkCreateComputePipelines();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyPipeline(
    VkDevice                                    device,
    VkPipeline                                  pipeline,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroyPipeline();
    device_data->dispatch_table.DestroyPipeline(device,pipeline,pAllocator);
    //PostCallvkDestroyPipeline();
}

VKAPI_ATTR VkResult VKAPI_CALL CreatePipelineLayout(
    VkDevice                                    device,
    const VkPipelineLayoutCreateInfo*           pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkPipelineLayout*                           pPipelineLayout)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkCreatePipelineLayout();
    VkResult result = device_data->dispatch_table.CreatePipelineLayout(device,pCreateInfo,pAllocator,pPipelineLayout);
    //PostCallvkCreatePipelineLayout();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyPipelineLayout(
    VkDevice                                    device,
    VkPipelineLayout                            pipelineLayout,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroyPipelineLayout();
    device_data->dispatch_table.DestroyPipelineLayout(device,pipelineLayout,pAllocator);
    //PostCallvkDestroyPipelineLayout();
}

VKAPI_ATTR VkResult VKAPI_CALL CreateSampler(
    VkDevice                                    device,
    const VkSamplerCreateInfo*                  pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSampler*                                  pSampler)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkCreateSampler();
    VkResult result = device_data->dispatch_table.CreateSampler(device,pCreateInfo,pAllocator,pSampler);
    //PostCallvkCreateSampler();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroySampler(
    VkDevice                                    device,
    VkSampler                                   sampler,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroySampler();
    device_data->dispatch_table.DestroySampler(device,sampler,pAllocator);
    //PostCallvkDestroySampler();
}

VKAPI_ATTR VkResult VKAPI_CALL CreateDescriptorSetLayout(
    VkDevice                                    device,
    const VkDescriptorSetLayoutCreateInfo*      pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDescriptorSetLayout*                      pSetLayout)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkCreateDescriptorSetLayout();
    VkResult result = device_data->dispatch_table.CreateDescriptorSetLayout(device,pCreateInfo,pAllocator,pSetLayout);
    //PostCallvkCreateDescriptorSetLayout();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyDescriptorSetLayout(
    VkDevice                                    device,
    VkDescriptorSetLayout                       descriptorSetLayout,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroyDescriptorSetLayout();
    device_data->dispatch_table.DestroyDescriptorSetLayout(device,descriptorSetLayout,pAllocator);
    //PostCallvkDestroyDescriptorSetLayout();
}

VKAPI_ATTR VkResult VKAPI_CALL CreateDescriptorPool(
    VkDevice                                    device,
    const VkDescriptorPoolCreateInfo*           pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDescriptorPool*                           pDescriptorPool)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkCreateDescriptorPool();
    VkResult result = device_data->dispatch_table.CreateDescriptorPool(device,pCreateInfo,pAllocator,pDescriptorPool);
    //PostCallvkCreateDescriptorPool();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyDescriptorPool(
    VkDevice                                    device,
    VkDescriptorPool                            descriptorPool,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroyDescriptorPool();
    device_data->dispatch_table.DestroyDescriptorPool(device,descriptorPool,pAllocator);
    //PostCallvkDestroyDescriptorPool();
}

VKAPI_ATTR VkResult VKAPI_CALL ResetDescriptorPool(
    VkDevice                                    device,
    VkDescriptorPool                            descriptorPool,
    VkDescriptorPoolResetFlags                  flags)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkResetDescriptorPool();
    VkResult result = device_data->dispatch_table.ResetDescriptorPool(device,descriptorPool,flags);
    //PostCallvkResetDescriptorPool();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL AllocateDescriptorSets(
    VkDevice                                    device,
    const VkDescriptorSetAllocateInfo*          pAllocateInfo,
    VkDescriptorSet*                            pDescriptorSets)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkAllocateDescriptorSets();
    VkResult result = device_data->dispatch_table.AllocateDescriptorSets(device,pAllocateInfo,pDescriptorSets);
    //PostCallvkAllocateDescriptorSets();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL FreeDescriptorSets(
    VkDevice                                    device,
    VkDescriptorPool                            descriptorPool,
    uint32_t                                    descriptorSetCount,
    const VkDescriptorSet*                      pDescriptorSets)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkFreeDescriptorSets();
    VkResult result = device_data->dispatch_table.FreeDescriptorSets(device,descriptorPool,descriptorSetCount,pDescriptorSets);
    //PostCallvkFreeDescriptorSets();
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
    //PreCallvkUpdateDescriptorSets();
    device_data->dispatch_table.UpdateDescriptorSets(device,descriptorWriteCount,pDescriptorWrites,descriptorCopyCount,pDescriptorCopies);
    //PostCallvkUpdateDescriptorSets();
}

VKAPI_ATTR VkResult VKAPI_CALL CreateFramebuffer(
    VkDevice                                    device,
    const VkFramebufferCreateInfo*              pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkFramebuffer*                              pFramebuffer)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkCreateFramebuffer();
    VkResult result = device_data->dispatch_table.CreateFramebuffer(device,pCreateInfo,pAllocator,pFramebuffer);
    //PostCallvkCreateFramebuffer();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyFramebuffer(
    VkDevice                                    device,
    VkFramebuffer                               framebuffer,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroyFramebuffer();
    device_data->dispatch_table.DestroyFramebuffer(device,framebuffer,pAllocator);
    //PostCallvkDestroyFramebuffer();
}

VKAPI_ATTR VkResult VKAPI_CALL CreateRenderPass(
    VkDevice                                    device,
    const VkRenderPassCreateInfo*               pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkRenderPass*                               pRenderPass)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkCreateRenderPass();
    VkResult result = device_data->dispatch_table.CreateRenderPass(device,pCreateInfo,pAllocator,pRenderPass);
    //PostCallvkCreateRenderPass();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyRenderPass(
    VkDevice                                    device,
    VkRenderPass                                renderPass,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroyRenderPass();
    device_data->dispatch_table.DestroyRenderPass(device,renderPass,pAllocator);
    //PostCallvkDestroyRenderPass();
}

VKAPI_ATTR void VKAPI_CALL GetRenderAreaGranularity(
    VkDevice                                    device,
    VkRenderPass                                renderPass,
    VkExtent2D*                                 pGranularity)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkGetRenderAreaGranularity();
    device_data->dispatch_table.GetRenderAreaGranularity(device,renderPass,pGranularity);
    //PostCallvkGetRenderAreaGranularity();
}

VKAPI_ATTR VkResult VKAPI_CALL CreateCommandPool(
    VkDevice                                    device,
    const VkCommandPoolCreateInfo*              pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkCommandPool*                              pCommandPool)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkCreateCommandPool();
    VkResult result = device_data->dispatch_table.CreateCommandPool(device,pCreateInfo,pAllocator,pCommandPool);
    //PostCallvkCreateCommandPool();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyCommandPool(
    VkDevice                                    device,
    VkCommandPool                               commandPool,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroyCommandPool();
    device_data->dispatch_table.DestroyCommandPool(device,commandPool,pAllocator);
    //PostCallvkDestroyCommandPool();
}

VKAPI_ATTR VkResult VKAPI_CALL ResetCommandPool(
    VkDevice                                    device,
    VkCommandPool                               commandPool,
    VkCommandPoolResetFlags                     flags)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkResetCommandPool();
    VkResult result = device_data->dispatch_table.ResetCommandPool(device,commandPool,flags);
    //PostCallvkResetCommandPool();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL AllocateCommandBuffers(
    VkDevice                                    device,
    const VkCommandBufferAllocateInfo*          pAllocateInfo,
    VkCommandBuffer*                            pCommandBuffers)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkAllocateCommandBuffers();
    VkResult result = device_data->dispatch_table.AllocateCommandBuffers(device,pAllocateInfo,pCommandBuffers);
    PostCallAllocateCommandBuffers(device_data, pAllocateInfo, pCommandBuffers);
    return result;
}

VKAPI_ATTR void VKAPI_CALL FreeCommandBuffers(
    VkDevice                                    device,
    VkCommandPool                               commandPool,
    uint32_t                                    commandBufferCount,
    const VkCommandBuffer*                      pCommandBuffers)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkFreeCommandBuffers();
    device_data->dispatch_table.FreeCommandBuffers(device,commandPool,commandBufferCount,pCommandBuffers);
    //PostCallvkFreeCommandBuffers();
}

VKAPI_ATTR VkResult VKAPI_CALL BeginCommandBuffer(
    VkCommandBuffer                             commandBuffer,
    const VkCommandBufferBeginInfo*             pBeginInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkBeginCommandBuffer();
    VkResult result = device_data->dispatch_table.BeginCommandBuffer(commandBuffer,pBeginInfo);
    //PostCallvkBeginCommandBuffer();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL EndCommandBuffer(
    VkCommandBuffer                             commandBuffer)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkEndCommandBuffer();
    VkResult result = device_data->dispatch_table.EndCommandBuffer(commandBuffer);
    //PostCallvkEndCommandBuffer();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL ResetCommandBuffer(
    VkCommandBuffer                             commandBuffer,
    VkCommandBufferResetFlags                   flags)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkResetCommandBuffer();
    VkResult result = device_data->dispatch_table.ResetCommandBuffer(commandBuffer,flags);
    //PostCallvkResetCommandBuffer();
    return result;
}

VKAPI_ATTR void VKAPI_CALL CmdBindPipeline(
    VkCommandBuffer                             commandBuffer,
    VkPipelineBindPoint                         pipelineBindPoint,
    VkPipeline                                  pipeline)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdBindPipeline();
    device_data->dispatch_table.CmdBindPipeline(commandBuffer,pipelineBindPoint,pipeline);
    //PostCallvkCmdBindPipeline();
}

VKAPI_ATTR void VKAPI_CALL CmdSetViewport(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstViewport,
    uint32_t                                    viewportCount,
    const VkViewport*                           pViewports)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdSetViewport();
    device_data->dispatch_table.CmdSetViewport(commandBuffer,firstViewport,viewportCount,pViewports);
    //PostCallvkCmdSetViewport();
}

VKAPI_ATTR void VKAPI_CALL CmdSetScissor(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstScissor,
    uint32_t                                    scissorCount,
    const VkRect2D*                             pScissors)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdSetScissor();
    device_data->dispatch_table.CmdSetScissor(commandBuffer,firstScissor,scissorCount,pScissors);
    //PostCallvkCmdSetScissor();
}

VKAPI_ATTR void VKAPI_CALL CmdSetLineWidth(
    VkCommandBuffer                             commandBuffer,
    float                                       lineWidth)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdSetLineWidth();
    device_data->dispatch_table.CmdSetLineWidth(commandBuffer,lineWidth);
    //PostCallvkCmdSetLineWidth();
}

VKAPI_ATTR void VKAPI_CALL CmdSetDepthBias(
    VkCommandBuffer                             commandBuffer,
    float                                       depthBiasConstantFactor,
    float                                       depthBiasClamp,
    float                                       depthBiasSlopeFactor)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdSetDepthBias();
    device_data->dispatch_table.CmdSetDepthBias(commandBuffer,depthBiasConstantFactor,depthBiasClamp,depthBiasSlopeFactor);
    //PostCallvkCmdSetDepthBias();
}

VKAPI_ATTR void VKAPI_CALL CmdSetBlendConstants(
    VkCommandBuffer                             commandBuffer,
    const float                                 blendConstants[4])
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdSetBlendConstants();
    device_data->dispatch_table.CmdSetBlendConstants(commandBuffer,blendConstants);
    //PostCallvkCmdSetBlendConstants();
}

VKAPI_ATTR void VKAPI_CALL CmdSetDepthBounds(
    VkCommandBuffer                             commandBuffer,
    float                                       minDepthBounds,
    float                                       maxDepthBounds)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdSetDepthBounds();
    device_data->dispatch_table.CmdSetDepthBounds(commandBuffer,minDepthBounds,maxDepthBounds);
    //PostCallvkCmdSetDepthBounds();
}

VKAPI_ATTR void VKAPI_CALL CmdSetStencilCompareMask(
    VkCommandBuffer                             commandBuffer,
    VkStencilFaceFlags                          faceMask,
    uint32_t                                    compareMask)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdSetStencilCompareMask();
    device_data->dispatch_table.CmdSetStencilCompareMask(commandBuffer,faceMask,compareMask);
    //PostCallvkCmdSetStencilCompareMask();
}

VKAPI_ATTR void VKAPI_CALL CmdSetStencilWriteMask(
    VkCommandBuffer                             commandBuffer,
    VkStencilFaceFlags                          faceMask,
    uint32_t                                    writeMask)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdSetStencilWriteMask();
    device_data->dispatch_table.CmdSetStencilWriteMask(commandBuffer,faceMask,writeMask);
    //PostCallvkCmdSetStencilWriteMask();
}

VKAPI_ATTR void VKAPI_CALL CmdSetStencilReference(
    VkCommandBuffer                             commandBuffer,
    VkStencilFaceFlags                          faceMask,
    uint32_t                                    reference)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdSetStencilReference();
    device_data->dispatch_table.CmdSetStencilReference(commandBuffer,faceMask,reference);
    //PostCallvkCmdSetStencilReference();
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
    //PreCallvkCmdBindDescriptorSets();
    device_data->dispatch_table.CmdBindDescriptorSets(commandBuffer,pipelineBindPoint,layout,firstSet,descriptorSetCount,pDescriptorSets,dynamicOffsetCount,pDynamicOffsets);
    //PostCallvkCmdBindDescriptorSets();
}

VKAPI_ATTR void VKAPI_CALL CmdBindIndexBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    VkIndexType                                 indexType)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdBindIndexBuffer();
    device_data->dispatch_table.CmdBindIndexBuffer(commandBuffer,buffer,offset,indexType);
    //PostCallvkCmdBindIndexBuffer();
}

VKAPI_ATTR void VKAPI_CALL CmdBindVertexBuffers(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstBinding,
    uint32_t                                    bindingCount,
    const VkBuffer*                             pBuffers,
    const VkDeviceSize*                         pOffsets)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdBindVertexBuffers();
    device_data->dispatch_table.CmdBindVertexBuffers(commandBuffer,firstBinding,bindingCount,pBuffers,pOffsets);
    //PostCallvkCmdBindVertexBuffers();
}

VKAPI_ATTR void VKAPI_CALL CmdDraw(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    vertexCount,
    uint32_t                                    instanceCount,
    uint32_t                                    firstVertex,
    uint32_t                                    firstInstance)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallCmdDraw(device_data, commandBuffer, vertexCount, instanceCount, firstVertex, firstInstance);
    device_data->dispatch_table.CmdDraw(commandBuffer,vertexCount,instanceCount,firstVertex,firstInstance);
    //PostCallvkCmdDraw();
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
    //PreCallvkCmdDrawIndexed();
    device_data->dispatch_table.CmdDrawIndexed(commandBuffer,indexCount,instanceCount,firstIndex,vertexOffset,firstInstance);
    //PostCallvkCmdDrawIndexed();
}

VKAPI_ATTR void VKAPI_CALL CmdDrawIndirect(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    uint32_t                                    drawCount,
    uint32_t                                    stride)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdDrawIndirect();
    device_data->dispatch_table.CmdDrawIndirect(commandBuffer,buffer,offset,drawCount,stride);
    //PostCallvkCmdDrawIndirect();
}

VKAPI_ATTR void VKAPI_CALL CmdDrawIndexedIndirect(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset,
    uint32_t                                    drawCount,
    uint32_t                                    stride)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdDrawIndexedIndirect();
    device_data->dispatch_table.CmdDrawIndexedIndirect(commandBuffer,buffer,offset,drawCount,stride);
    //PostCallvkCmdDrawIndexedIndirect();
}

VKAPI_ATTR void VKAPI_CALL CmdDispatch(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    groupCountX,
    uint32_t                                    groupCountY,
    uint32_t                                    groupCountZ)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdDispatch();
    device_data->dispatch_table.CmdDispatch(commandBuffer,groupCountX,groupCountY,groupCountZ);
    //PostCallvkCmdDispatch();
}

VKAPI_ATTR void VKAPI_CALL CmdDispatchIndirect(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    buffer,
    VkDeviceSize                                offset)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdDispatchIndirect();
    device_data->dispatch_table.CmdDispatchIndirect(commandBuffer,buffer,offset);
    //PostCallvkCmdDispatchIndirect();
}

VKAPI_ATTR void VKAPI_CALL CmdCopyBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    srcBuffer,
    VkBuffer                                    dstBuffer,
    uint32_t                                    regionCount,
    const VkBufferCopy*                         pRegions)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdCopyBuffer();
    device_data->dispatch_table.CmdCopyBuffer(commandBuffer,srcBuffer,dstBuffer,regionCount,pRegions);
    //PostCallvkCmdCopyBuffer();
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
    //PreCallvkCmdCopyImage();
    device_data->dispatch_table.CmdCopyImage(commandBuffer,srcImage,srcImageLayout,dstImage,dstImageLayout,regionCount,pRegions);
    //PostCallvkCmdCopyImage();
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
    //PreCallvkCmdBlitImage();
    device_data->dispatch_table.CmdBlitImage(commandBuffer,srcImage,srcImageLayout,dstImage,dstImageLayout,regionCount,pRegions,filter);
    //PostCallvkCmdBlitImage();
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
    //PreCallvkCmdCopyBufferToImage();
    device_data->dispatch_table.CmdCopyBufferToImage(commandBuffer,srcBuffer,dstImage,dstImageLayout,regionCount,pRegions);
    //PostCallvkCmdCopyBufferToImage();
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
    //PreCallvkCmdCopyImageToBuffer();
    device_data->dispatch_table.CmdCopyImageToBuffer(commandBuffer,srcImage,srcImageLayout,dstBuffer,regionCount,pRegions);
    //PostCallvkCmdCopyImageToBuffer();
}

VKAPI_ATTR void VKAPI_CALL CmdUpdateBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    dstBuffer,
    VkDeviceSize                                dstOffset,
    VkDeviceSize                                dataSize,
    const void*                                 pData)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdUpdateBuffer();
    device_data->dispatch_table.CmdUpdateBuffer(commandBuffer,dstBuffer,dstOffset,dataSize,pData);
    //PostCallvkCmdUpdateBuffer();
}

VKAPI_ATTR void VKAPI_CALL CmdFillBuffer(
    VkCommandBuffer                             commandBuffer,
    VkBuffer                                    dstBuffer,
    VkDeviceSize                                dstOffset,
    VkDeviceSize                                size,
    uint32_t                                    data)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdFillBuffer();
    device_data->dispatch_table.CmdFillBuffer(commandBuffer,dstBuffer,dstOffset,size,data);
    //PostCallvkCmdFillBuffer();
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
    //PreCallvkCmdClearColorImage();
    device_data->dispatch_table.CmdClearColorImage(commandBuffer,image,imageLayout,pColor,rangeCount,pRanges);
    //PostCallvkCmdClearColorImage();
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
    //PreCallvkCmdClearDepthStencilImage();
    device_data->dispatch_table.CmdClearDepthStencilImage(commandBuffer,image,imageLayout,pDepthStencil,rangeCount,pRanges);
    //PostCallvkCmdClearDepthStencilImage();
}

VKAPI_ATTR void VKAPI_CALL CmdClearAttachments(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    attachmentCount,
    const VkClearAttachment*                    pAttachments,
    uint32_t                                    rectCount,
    const VkClearRect*                          pRects)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdClearAttachments();
    device_data->dispatch_table.CmdClearAttachments(commandBuffer,attachmentCount,pAttachments,rectCount,pRects);
    //PostCallvkCmdClearAttachments();
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
    //PreCallvkCmdResolveImage();
    device_data->dispatch_table.CmdResolveImage(commandBuffer,srcImage,srcImageLayout,dstImage,dstImageLayout,regionCount,pRegions);
    //PostCallvkCmdResolveImage();
}

VKAPI_ATTR void VKAPI_CALL CmdSetEvent(
    VkCommandBuffer                             commandBuffer,
    VkEvent                                     event,
    VkPipelineStageFlags                        stageMask)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdSetEvent();
    device_data->dispatch_table.CmdSetEvent(commandBuffer,event,stageMask);
    //PostCallvkCmdSetEvent();
}

VKAPI_ATTR void VKAPI_CALL CmdResetEvent(
    VkCommandBuffer                             commandBuffer,
    VkEvent                                     event,
    VkPipelineStageFlags                        stageMask)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdResetEvent();
    device_data->dispatch_table.CmdResetEvent(commandBuffer,event,stageMask);
    //PostCallvkCmdResetEvent();
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
    //PreCallvkCmdWaitEvents();
    device_data->dispatch_table.CmdWaitEvents(commandBuffer,eventCount,pEvents,srcStageMask,dstStageMask,memoryBarrierCount,pMemoryBarriers,bufferMemoryBarrierCount,pBufferMemoryBarriers,imageMemoryBarrierCount,pImageMemoryBarriers);
    //PostCallvkCmdWaitEvents();
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
    //PreCallvkCmdPipelineBarrier();
    device_data->dispatch_table.CmdPipelineBarrier(commandBuffer,srcStageMask,dstStageMask,dependencyFlags,memoryBarrierCount,pMemoryBarriers,bufferMemoryBarrierCount,pBufferMemoryBarriers,imageMemoryBarrierCount,pImageMemoryBarriers);
    //PostCallvkCmdPipelineBarrier();
}

VKAPI_ATTR void VKAPI_CALL CmdBeginQuery(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    query,
    VkQueryControlFlags                         flags)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdBeginQuery();
    device_data->dispatch_table.CmdBeginQuery(commandBuffer,queryPool,query,flags);
    //PostCallvkCmdBeginQuery();
}

VKAPI_ATTR void VKAPI_CALL CmdEndQuery(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    query)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdEndQuery();
    device_data->dispatch_table.CmdEndQuery(commandBuffer,queryPool,query);
    //PostCallvkCmdEndQuery();
}

VKAPI_ATTR void VKAPI_CALL CmdResetQueryPool(
    VkCommandBuffer                             commandBuffer,
    VkQueryPool                                 queryPool,
    uint32_t                                    firstQuery,
    uint32_t                                    queryCount)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdResetQueryPool();
    device_data->dispatch_table.CmdResetQueryPool(commandBuffer,queryPool,firstQuery,queryCount);
    //PostCallvkCmdResetQueryPool();
}

VKAPI_ATTR void VKAPI_CALL CmdWriteTimestamp(
    VkCommandBuffer                             commandBuffer,
    VkPipelineStageFlagBits                     pipelineStage,
    VkQueryPool                                 queryPool,
    uint32_t                                    query)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdWriteTimestamp();
    device_data->dispatch_table.CmdWriteTimestamp(commandBuffer,pipelineStage,queryPool,query);
    //PostCallvkCmdWriteTimestamp();
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
    //PreCallvkCmdCopyQueryPoolResults();
    device_data->dispatch_table.CmdCopyQueryPoolResults(commandBuffer,queryPool,firstQuery,queryCount,dstBuffer,dstOffset,stride,flags);
    //PostCallvkCmdCopyQueryPoolResults();
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
    //PreCallvkCmdPushConstants();
    device_data->dispatch_table.CmdPushConstants(commandBuffer,layout,stageFlags,offset,size,pValues);
    //PostCallvkCmdPushConstants();
}

VKAPI_ATTR void VKAPI_CALL CmdBeginRenderPass(
    VkCommandBuffer                             commandBuffer,
    const VkRenderPassBeginInfo*                pRenderPassBegin,
    VkSubpassContents                           contents)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdBeginRenderPass();
    device_data->dispatch_table.CmdBeginRenderPass(commandBuffer,pRenderPassBegin,contents);
    //PostCallvkCmdBeginRenderPass();
}

VKAPI_ATTR void VKAPI_CALL CmdNextSubpass(
    VkCommandBuffer                             commandBuffer,
    VkSubpassContents                           contents)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdNextSubpass();
    device_data->dispatch_table.CmdNextSubpass(commandBuffer,contents);
    //PostCallvkCmdNextSubpass();
}

VKAPI_ATTR void VKAPI_CALL CmdEndRenderPass(
    VkCommandBuffer                             commandBuffer)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdEndRenderPass();
    device_data->dispatch_table.CmdEndRenderPass(commandBuffer);
    //PostCallvkCmdEndRenderPass();
}

VKAPI_ATTR void VKAPI_CALL CmdExecuteCommands(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    commandBufferCount,
    const VkCommandBuffer*                      pCommandBuffers)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdExecuteCommands();
    device_data->dispatch_table.CmdExecuteCommands(commandBuffer,commandBufferCount,pCommandBuffers);
    //PostCallvkCmdExecuteCommands();
}


VKAPI_ATTR void VKAPI_CALL DestroySurfaceKHR(
    VkInstance                                  instance,
    VkSurfaceKHR                                surface,
    const VkAllocationCallbacks*                pAllocator)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    //PreCallvkDestroySurfaceKHR();
    instance_data->dispatch_table.DestroySurfaceKHR(instance,surface,pAllocator);
    //PostCallvkDestroySurfaceKHR();
}

VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceSurfaceSupportKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    queueFamilyIndex,
    VkSurfaceKHR                                surface,
    VkBool32*                                   pSupported)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceSurfaceSupportKHR();
    VkResult result = instance_data->dispatch_table.GetPhysicalDeviceSurfaceSupportKHR(physicalDevice,queueFamilyIndex,surface,pSupported);
    //PostCallvkGetPhysicalDeviceSurfaceSupportKHR();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceSurfaceCapabilitiesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    VkSurfaceCapabilitiesKHR*                   pSurfaceCapabilities)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceSurfaceCapabilitiesKHR();
    VkResult result = instance_data->dispatch_table.GetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice,surface,pSurfaceCapabilities);
    //PostCallvkGetPhysicalDeviceSurfaceCapabilitiesKHR();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceSurfaceFormatsKHR(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    uint32_t*                                   pSurfaceFormatCount,
    VkSurfaceFormatKHR*                         pSurfaceFormats)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceSurfaceFormatsKHR();
    VkResult result = instance_data->dispatch_table.GetPhysicalDeviceSurfaceFormatsKHR(physicalDevice,surface,pSurfaceFormatCount,pSurfaceFormats);
    //PostCallvkGetPhysicalDeviceSurfaceFormatsKHR();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceSurfacePresentModesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    uint32_t*                                   pPresentModeCount,
    VkPresentModeKHR*                           pPresentModes)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceSurfacePresentModesKHR();
    VkResult result = instance_data->dispatch_table.GetPhysicalDeviceSurfacePresentModesKHR(physicalDevice,surface,pPresentModeCount,pPresentModes);
    //PostCallvkGetPhysicalDeviceSurfacePresentModesKHR();
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL CreateSwapchainKHR(
    VkDevice                                    device,
    const VkSwapchainCreateInfoKHR*             pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSwapchainKHR*                             pSwapchain)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkCreateSwapchainKHR();
    VkResult result = device_data->dispatch_table.CreateSwapchainKHR(device,pCreateInfo,pAllocator,pSwapchain);
    //PostCallvkCreateSwapchainKHR();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroySwapchainKHR(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroySwapchainKHR();
    device_data->dispatch_table.DestroySwapchainKHR(device,swapchain,pAllocator);
    //PostCallvkDestroySwapchainKHR();
}

VKAPI_ATTR VkResult VKAPI_CALL GetSwapchainImagesKHR(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain,
    uint32_t*                                   pSwapchainImageCount,
    VkImage*                                    pSwapchainImages)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkGetSwapchainImagesKHR();
    VkResult result = device_data->dispatch_table.GetSwapchainImagesKHR(device,swapchain,pSwapchainImageCount,pSwapchainImages);
    //PostCallvkGetSwapchainImagesKHR();
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
    //PreCallvkAcquireNextImageKHR();
    VkResult result = device_data->dispatch_table.AcquireNextImageKHR(device,swapchain,timeout,semaphore,fence,pImageIndex);
    //PostCallvkAcquireNextImageKHR();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL QueuePresentKHR(
    VkQueue                                     queue,
    const VkPresentInfoKHR*                     pPresentInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(queue), device_layer_data_map);
    //PreCallvkQueuePresentKHR();
    VkResult result = device_data->dispatch_table.QueuePresentKHR(queue,pPresentInfo);
    //PostCallvkQueuePresentKHR();
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceDisplayPropertiesKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t*                                   pPropertyCount,
    VkDisplayPropertiesKHR*                     pProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceDisplayPropertiesKHR();
    VkResult result = instance_data->dispatch_table.GetPhysicalDeviceDisplayPropertiesKHR(physicalDevice,pPropertyCount,pProperties);
    //PostCallvkGetPhysicalDeviceDisplayPropertiesKHR();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceDisplayPlanePropertiesKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t*                                   pPropertyCount,
    VkDisplayPlanePropertiesKHR*                pProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceDisplayPlanePropertiesKHR();
    VkResult result = instance_data->dispatch_table.GetPhysicalDeviceDisplayPlanePropertiesKHR(physicalDevice,pPropertyCount,pProperties);
    //PostCallvkGetPhysicalDeviceDisplayPlanePropertiesKHR();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetDisplayPlaneSupportedDisplaysKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    planeIndex,
    uint32_t*                                   pDisplayCount,
    VkDisplayKHR*                               pDisplays)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetDisplayPlaneSupportedDisplaysKHR();
    VkResult result = instance_data->dispatch_table.GetDisplayPlaneSupportedDisplaysKHR(physicalDevice,planeIndex,pDisplayCount,pDisplays);
    //PostCallvkGetDisplayPlaneSupportedDisplaysKHR();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetDisplayModePropertiesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkDisplayKHR                                display,
    uint32_t*                                   pPropertyCount,
    VkDisplayModePropertiesKHR*                 pProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetDisplayModePropertiesKHR();
    VkResult result = instance_data->dispatch_table.GetDisplayModePropertiesKHR(physicalDevice,display,pPropertyCount,pProperties);
    //PostCallvkGetDisplayModePropertiesKHR();
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
    //PreCallvkCreateDisplayModeKHR();
    VkResult result = instance_data->dispatch_table.CreateDisplayModeKHR(physicalDevice,display,pCreateInfo,pAllocator,pMode);
    //PostCallvkCreateDisplayModeKHR();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetDisplayPlaneCapabilitiesKHR(
    VkPhysicalDevice                            physicalDevice,
    VkDisplayModeKHR                            mode,
    uint32_t                                    planeIndex,
    VkDisplayPlaneCapabilitiesKHR*              pCapabilities)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetDisplayPlaneCapabilitiesKHR();
    VkResult result = instance_data->dispatch_table.GetDisplayPlaneCapabilitiesKHR(physicalDevice,mode,planeIndex,pCapabilities);
    //PostCallvkGetDisplayPlaneCapabilitiesKHR();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL CreateDisplayPlaneSurfaceKHR(
    VkInstance                                  instance,
    const VkDisplaySurfaceCreateInfoKHR*        pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSurfaceKHR*                               pSurface)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    //PreCallvkCreateDisplayPlaneSurfaceKHR();
    VkResult result = instance_data->dispatch_table.CreateDisplayPlaneSurfaceKHR(instance,pCreateInfo,pAllocator,pSurface);
    //PostCallvkCreateDisplayPlaneSurfaceKHR();
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
    //PreCallvkCreateSharedSwapchainsKHR();
    VkResult result = device_data->dispatch_table.CreateSharedSwapchainsKHR(device,swapchainCount,pCreateInfos,pAllocator,pSwapchains);
    //PostCallvkCreateSharedSwapchainsKHR();
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
    //PreCallvkCreateXlibSurfaceKHR();
    VkResult result = instance_data->dispatch_table.CreateXlibSurfaceKHR(instance,pCreateInfo,pAllocator,pSurface);
    //PostCallvkCreateXlibSurfaceKHR();
    return result;
}

VKAPI_ATTR VkBool32 VKAPI_CALL GetPhysicalDeviceXlibPresentationSupportKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    queueFamilyIndex,
    Display*                                    dpy,
    VisualID                                    visualID)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceXlibPresentationSupportKHR();
    VkBool32 result = instance_data->dispatch_table.GetPhysicalDeviceXlibPresentationSupportKHR(physicalDevice,queueFamilyIndex,dpy,visualID);
    //PostCallvkGetPhysicalDeviceXlibPresentationSupportKHR();
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
    //PreCallvkCreateXcbSurfaceKHR();
    VkResult result = instance_data->dispatch_table.CreateXcbSurfaceKHR(instance,pCreateInfo,pAllocator,pSurface);
    //PostCallvkCreateXcbSurfaceKHR();
    return result;
}

VKAPI_ATTR VkBool32 VKAPI_CALL GetPhysicalDeviceXcbPresentationSupportKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    queueFamilyIndex,
    xcb_connection_t*                           connection,
    xcb_visualid_t                              visual_id)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceXcbPresentationSupportKHR();
    VkBool32 result = instance_data->dispatch_table.GetPhysicalDeviceXcbPresentationSupportKHR(physicalDevice,queueFamilyIndex,connection,visual_id);
    //PostCallvkGetPhysicalDeviceXcbPresentationSupportKHR();
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
    //PreCallvkCreateWaylandSurfaceKHR();
    VkResult result = instance_data->dispatch_table.CreateWaylandSurfaceKHR(instance,pCreateInfo,pAllocator,pSurface);
    //PostCallvkCreateWaylandSurfaceKHR();
    return result;
}

VKAPI_ATTR VkBool32 VKAPI_CALL GetPhysicalDeviceWaylandPresentationSupportKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    queueFamilyIndex,
    struct wl_display*                          display)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceWaylandPresentationSupportKHR();
    VkBool32 result = instance_data->dispatch_table.GetPhysicalDeviceWaylandPresentationSupportKHR(physicalDevice,queueFamilyIndex,display);
    //PostCallvkGetPhysicalDeviceWaylandPresentationSupportKHR();
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
    //PreCallvkCreateMirSurfaceKHR();
    VkResult result = instance_data->dispatch_table.CreateMirSurfaceKHR(instance,pCreateInfo,pAllocator,pSurface);
    //PostCallvkCreateMirSurfaceKHR();
    return result;
}

VKAPI_ATTR VkBool32 VKAPI_CALL GetPhysicalDeviceMirPresentationSupportKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    queueFamilyIndex,
    MirConnection*                              connection)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceMirPresentationSupportKHR();
    VkBool32 result = instance_data->dispatch_table.GetPhysicalDeviceMirPresentationSupportKHR(physicalDevice,queueFamilyIndex,connection);
    //PostCallvkGetPhysicalDeviceMirPresentationSupportKHR();
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
    //PreCallvkCreateAndroidSurfaceKHR();
    VkResult result = instance_data->dispatch_table.CreateAndroidSurfaceKHR(instance,pCreateInfo,pAllocator,pSurface);
    //PostCallvkCreateAndroidSurfaceKHR();
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
    //PreCallvkCreateWin32SurfaceKHR();
    VkResult result = instance_data->dispatch_table.CreateWin32SurfaceKHR(instance,pCreateInfo,pAllocator,pSurface);
    //PostCallvkCreateWin32SurfaceKHR();
    return result;
}

VKAPI_ATTR VkBool32 VKAPI_CALL GetPhysicalDeviceWin32PresentationSupportKHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t                                    queueFamilyIndex)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceWin32PresentationSupportKHR();
    VkBool32 result = instance_data->dispatch_table.GetPhysicalDeviceWin32PresentationSupportKHR(physicalDevice,queueFamilyIndex);
    //PostCallvkGetPhysicalDeviceWin32PresentationSupportKHR();
    return result;
}
#endif /* VK_USE_PLATFORM_WIN32_KHR */



VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceFeatures2KHR(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceFeatures2KHR*               pFeatures)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceFeatures2KHR();
    instance_data->dispatch_table.GetPhysicalDeviceFeatures2KHR(physicalDevice,pFeatures);
    //PostCallvkGetPhysicalDeviceFeatures2KHR();
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceProperties2KHR(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceProperties2KHR*             pProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceProperties2KHR();
    instance_data->dispatch_table.GetPhysicalDeviceProperties2KHR(physicalDevice,pProperties);
    //PostCallvkGetPhysicalDeviceProperties2KHR();
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceFormatProperties2KHR(
    VkPhysicalDevice                            physicalDevice,
    VkFormat                                    format,
    VkFormatProperties2KHR*                     pFormatProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceFormatProperties2KHR();
    instance_data->dispatch_table.GetPhysicalDeviceFormatProperties2KHR(physicalDevice,format,pFormatProperties);
    //PostCallvkGetPhysicalDeviceFormatProperties2KHR();
}

VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceImageFormatProperties2KHR(
    VkPhysicalDevice                            physicalDevice,
    const VkPhysicalDeviceImageFormatInfo2KHR*  pImageFormatInfo,
    VkImageFormatProperties2KHR*                pImageFormatProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceImageFormatProperties2KHR();
    VkResult result = instance_data->dispatch_table.GetPhysicalDeviceImageFormatProperties2KHR(physicalDevice,pImageFormatInfo,pImageFormatProperties);
    //PostCallvkGetPhysicalDeviceImageFormatProperties2KHR();
    return result;
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceQueueFamilyProperties2KHR(
    VkPhysicalDevice                            physicalDevice,
    uint32_t*                                   pQueueFamilyPropertyCount,
    VkQueueFamilyProperties2KHR*                pQueueFamilyProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceQueueFamilyProperties2KHR();
    instance_data->dispatch_table.GetPhysicalDeviceQueueFamilyProperties2KHR(physicalDevice,pQueueFamilyPropertyCount,pQueueFamilyProperties);
    //PostCallvkGetPhysicalDeviceQueueFamilyProperties2KHR();
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceMemoryProperties2KHR(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceMemoryProperties2KHR*       pMemoryProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceMemoryProperties2KHR();
    instance_data->dispatch_table.GetPhysicalDeviceMemoryProperties2KHR(physicalDevice,pMemoryProperties);
    //PostCallvkGetPhysicalDeviceMemoryProperties2KHR();
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceSparseImageFormatProperties2KHR(
    VkPhysicalDevice                            physicalDevice,
    const VkPhysicalDeviceSparseImageFormatInfo2KHR* pFormatInfo,
    uint32_t*                                   pPropertyCount,
    VkSparseImageFormatProperties2KHR*          pProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceSparseImageFormatProperties2KHR();
    instance_data->dispatch_table.GetPhysicalDeviceSparseImageFormatProperties2KHR(physicalDevice,pFormatInfo,pPropertyCount,pProperties);
    //PostCallvkGetPhysicalDeviceSparseImageFormatProperties2KHR();
}



VKAPI_ATTR void VKAPI_CALL TrimCommandPoolKHR(
    VkDevice                                    device,
    VkCommandPool                               commandPool,
    VkCommandPoolTrimFlagsKHR                   flags)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkTrimCommandPoolKHR();
    device_data->dispatch_table.TrimCommandPoolKHR(device,commandPool,flags);
    //PostCallvkTrimCommandPoolKHR();
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
    //PreCallvkCmdPushDescriptorSetKHR();
    device_data->dispatch_table.CmdPushDescriptorSetKHR(commandBuffer,pipelineBindPoint,layout,set,descriptorWriteCount,pDescriptorWrites);
    //PostCallvkCmdPushDescriptorSetKHR();
}



VKAPI_ATTR VkResult VKAPI_CALL CreateDescriptorUpdateTemplateKHR(
    VkDevice                                    device,
    const VkDescriptorUpdateTemplateCreateInfoKHR* pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkDescriptorUpdateTemplateKHR*              pDescriptorUpdateTemplate)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkCreateDescriptorUpdateTemplateKHR();
    VkResult result = device_data->dispatch_table.CreateDescriptorUpdateTemplateKHR(device,pCreateInfo,pAllocator,pDescriptorUpdateTemplate);
    //PostCallvkCreateDescriptorUpdateTemplateKHR();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyDescriptorUpdateTemplateKHR(
    VkDevice                                    device,
    VkDescriptorUpdateTemplateKHR               descriptorUpdateTemplate,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroyDescriptorUpdateTemplateKHR();
    device_data->dispatch_table.DestroyDescriptorUpdateTemplateKHR(device,descriptorUpdateTemplate,pAllocator);
    //PostCallvkDestroyDescriptorUpdateTemplateKHR();
}

VKAPI_ATTR void VKAPI_CALL UpdateDescriptorSetWithTemplateKHR(
    VkDevice                                    device,
    VkDescriptorSet                             descriptorSet,
    VkDescriptorUpdateTemplateKHR               descriptorUpdateTemplate,
    const void*                                 pData)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkUpdateDescriptorSetWithTemplateKHR();
    device_data->dispatch_table.UpdateDescriptorSetWithTemplateKHR(device,descriptorSet,descriptorUpdateTemplate,pData);
    //PostCallvkUpdateDescriptorSetWithTemplateKHR();
}

VKAPI_ATTR void VKAPI_CALL CmdPushDescriptorSetWithTemplateKHR(
    VkCommandBuffer                             commandBuffer,
    VkDescriptorUpdateTemplateKHR               descriptorUpdateTemplate,
    VkPipelineLayout                            layout,
    uint32_t                                    set,
    const void*                                 pData)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdPushDescriptorSetWithTemplateKHR();
    device_data->dispatch_table.CmdPushDescriptorSetWithTemplateKHR(commandBuffer,descriptorUpdateTemplate,layout,set,pData);
    //PostCallvkCmdPushDescriptorSetWithTemplateKHR();
}


VKAPI_ATTR VkResult VKAPI_CALL GetSwapchainStatusKHR(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkGetSwapchainStatusKHR();
    VkResult result = device_data->dispatch_table.GetSwapchainStatusKHR(device,swapchain);
    //PostCallvkGetSwapchainStatusKHR();
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceSurfaceCapabilities2KHR(
    VkPhysicalDevice                            physicalDevice,
    const VkPhysicalDeviceSurfaceInfo2KHR*      pSurfaceInfo,
    VkSurfaceCapabilities2KHR*                  pSurfaceCapabilities)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceSurfaceCapabilities2KHR();
    VkResult result = instance_data->dispatch_table.GetPhysicalDeviceSurfaceCapabilities2KHR(physicalDevice,pSurfaceInfo,pSurfaceCapabilities);
    //PostCallvkGetPhysicalDeviceSurfaceCapabilities2KHR();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceSurfaceFormats2KHR(
    VkPhysicalDevice                            physicalDevice,
    const VkPhysicalDeviceSurfaceInfo2KHR*      pSurfaceInfo,
    uint32_t*                                   pSurfaceFormatCount,
    VkSurfaceFormat2KHR*                        pSurfaceFormats)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceSurfaceFormats2KHR();
    VkResult result = instance_data->dispatch_table.GetPhysicalDeviceSurfaceFormats2KHR(physicalDevice,pSurfaceInfo,pSurfaceFormatCount,pSurfaceFormats);
    //PostCallvkGetPhysicalDeviceSurfaceFormats2KHR();
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
    //PreCallvkDebugReportMessageEXT();
    instance_data->dispatch_table.DebugReportMessageEXT(instance,flags,objectType,object,location,messageCode,pLayerPrefix,pMessage);
    //PostCallvkDebugReportMessageEXT();
}







VKAPI_ATTR VkResult VKAPI_CALL DebugMarkerSetObjectTagEXT(
    VkDevice                                    device,
    VkDebugMarkerObjectTagInfoEXT*              pTagInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDebugMarkerSetObjectTagEXT();
    VkResult result = device_data->dispatch_table.DebugMarkerSetObjectTagEXT(device,pTagInfo);
    //PostCallvkDebugMarkerSetObjectTagEXT();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL DebugMarkerSetObjectNameEXT(
    VkDevice                                    device,
    VkDebugMarkerObjectNameInfoEXT*             pNameInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDebugMarkerSetObjectNameEXT();
    VkResult result = device_data->dispatch_table.DebugMarkerSetObjectNameEXT(device,pNameInfo);
    //PostCallvkDebugMarkerSetObjectNameEXT();
    return result;
}

VKAPI_ATTR void VKAPI_CALL CmdDebugMarkerBeginEXT(
    VkCommandBuffer                             commandBuffer,
    VkDebugMarkerMarkerInfoEXT*                 pMarkerInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdDebugMarkerBeginEXT();
    device_data->dispatch_table.CmdDebugMarkerBeginEXT(commandBuffer,pMarkerInfo);
    //PostCallvkCmdDebugMarkerBeginEXT();
}

VKAPI_ATTR void VKAPI_CALL CmdDebugMarkerEndEXT(
    VkCommandBuffer                             commandBuffer)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdDebugMarkerEndEXT();
    device_data->dispatch_table.CmdDebugMarkerEndEXT(commandBuffer);
    //PostCallvkCmdDebugMarkerEndEXT();
}

VKAPI_ATTR void VKAPI_CALL CmdDebugMarkerInsertEXT(
    VkCommandBuffer                             commandBuffer,
    VkDebugMarkerMarkerInfoEXT*                 pMarkerInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdDebugMarkerInsertEXT();
    device_data->dispatch_table.CmdDebugMarkerInsertEXT(commandBuffer,pMarkerInfo);
    //PostCallvkCmdDebugMarkerInsertEXT();
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
    //PreCallvkCmdDrawIndirectCountAMD();
    device_data->dispatch_table.CmdDrawIndirectCountAMD(commandBuffer,buffer,offset,countBuffer,countBufferOffset,maxDrawCount,stride);
    //PostCallvkCmdDrawIndirectCountAMD();
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
    //PreCallvkCmdDrawIndexedIndirectCountAMD();
    device_data->dispatch_table.CmdDrawIndexedIndirectCountAMD(commandBuffer,buffer,offset,countBuffer,countBufferOffset,maxDrawCount,stride);
    //PostCallvkCmdDrawIndexedIndirectCountAMD();
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
    //PreCallvkGetPhysicalDeviceExternalImageFormatPropertiesNV();
    VkResult result = instance_data->dispatch_table.GetPhysicalDeviceExternalImageFormatPropertiesNV(physicalDevice,format,type,tiling,usage,flags,externalHandleType,pExternalImageFormatProperties);
    //PostCallvkGetPhysicalDeviceExternalImageFormatPropertiesNV();
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
    //PreCallvkGetMemoryWin32HandleNV();
    VkResult result = device_data->dispatch_table.GetMemoryWin32HandleNV(device,memory,handleType,pHandle);
    //PostCallvkGetMemoryWin32HandleNV();
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
    //PreCallvkGetDeviceGroupPeerMemoryFeaturesKHX();
    device_data->dispatch_table.GetDeviceGroupPeerMemoryFeaturesKHX(device,heapIndex,localDeviceIndex,remoteDeviceIndex,pPeerMemoryFeatures);
    //PostCallvkGetDeviceGroupPeerMemoryFeaturesKHX();
}

VKAPI_ATTR VkResult VKAPI_CALL BindBufferMemory2KHX(
    VkDevice                                    device,
    uint32_t                                    bindInfoCount,
    const VkBindBufferMemoryInfoKHX*            pBindInfos)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkBindBufferMemory2KHX();
    VkResult result = device_data->dispatch_table.BindBufferMemory2KHX(device,bindInfoCount,pBindInfos);
    //PostCallvkBindBufferMemory2KHX();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL BindImageMemory2KHX(
    VkDevice                                    device,
    uint32_t                                    bindInfoCount,
    const VkBindImageMemoryInfoKHX*             pBindInfos)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkBindImageMemory2KHX();
    VkResult result = device_data->dispatch_table.BindImageMemory2KHX(device,bindInfoCount,pBindInfos);
    //PostCallvkBindImageMemory2KHX();
    return result;
}

VKAPI_ATTR void VKAPI_CALL CmdSetDeviceMaskKHX(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    deviceMask)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdSetDeviceMaskKHX();
    device_data->dispatch_table.CmdSetDeviceMaskKHX(commandBuffer,deviceMask);
    //PostCallvkCmdSetDeviceMaskKHX();
}

VKAPI_ATTR VkResult VKAPI_CALL GetDeviceGroupPresentCapabilitiesKHX(
    VkDevice                                    device,
    VkDeviceGroupPresentCapabilitiesKHX*        pDeviceGroupPresentCapabilities)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkGetDeviceGroupPresentCapabilitiesKHX();
    VkResult result = device_data->dispatch_table.GetDeviceGroupPresentCapabilitiesKHX(device,pDeviceGroupPresentCapabilities);
    //PostCallvkGetDeviceGroupPresentCapabilitiesKHX();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetDeviceGroupSurfacePresentModesKHX(
    VkDevice                                    device,
    VkSurfaceKHR                                surface,
    VkDeviceGroupPresentModeFlagsKHX*           pModes)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkGetDeviceGroupSurfacePresentModesKHX();
    VkResult result = device_data->dispatch_table.GetDeviceGroupSurfacePresentModesKHX(device,surface,pModes);
    //PostCallvkGetDeviceGroupSurfacePresentModesKHX();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL AcquireNextImage2KHX(
    VkDevice                                    device,
    const VkAcquireNextImageInfoKHX*            pAcquireInfo,
    uint32_t*                                   pImageIndex)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkAcquireNextImage2KHX();
    VkResult result = device_data->dispatch_table.AcquireNextImage2KHX(device,pAcquireInfo,pImageIndex);
    //PostCallvkAcquireNextImage2KHX();
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
    //PreCallvkCmdDispatchBaseKHX();
    device_data->dispatch_table.CmdDispatchBaseKHX(commandBuffer,baseGroupX,baseGroupY,baseGroupZ,groupCountX,groupCountY,groupCountZ);
    //PostCallvkCmdDispatchBaseKHX();
}

VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDevicePresentRectanglesKHX(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    uint32_t*                                   pRectCount,
    VkRect2D*                                   pRects)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDevicePresentRectanglesKHX();
    VkResult result = instance_data->dispatch_table.GetPhysicalDevicePresentRectanglesKHX(physicalDevice,surface,pRectCount,pRects);
    //PostCallvkGetPhysicalDevicePresentRectanglesKHX();
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
    //PreCallvkCreateViSurfaceNN();
    VkResult result = instance_data->dispatch_table.CreateViSurfaceNN(instance,pCreateInfo,pAllocator,pSurface);
    //PostCallvkCreateViSurfaceNN();
    return result;
}
#endif /* VK_USE_PLATFORM_VI_NN */




VKAPI_ATTR VkResult VKAPI_CALL EnumeratePhysicalDeviceGroupsKHX(
    VkInstance                                  instance,
    uint32_t*                                   pPhysicalDeviceGroupCount,
    VkPhysicalDeviceGroupPropertiesKHX*         pPhysicalDeviceGroupProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    //PreCallvkEnumeratePhysicalDeviceGroupsKHX();
    VkResult result = instance_data->dispatch_table.EnumeratePhysicalDeviceGroupsKHX(instance,pPhysicalDeviceGroupCount,pPhysicalDeviceGroupProperties);
    //PostCallvkEnumeratePhysicalDeviceGroupsKHX();
    return result;
}


VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceExternalBufferPropertiesKHX(
    VkPhysicalDevice                            physicalDevice,
    const VkPhysicalDeviceExternalBufferInfoKHX* pExternalBufferInfo,
    VkExternalBufferPropertiesKHX*              pExternalBufferProperties)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceExternalBufferPropertiesKHX();
    instance_data->dispatch_table.GetPhysicalDeviceExternalBufferPropertiesKHX(physicalDevice,pExternalBufferInfo,pExternalBufferProperties);
    //PostCallvkGetPhysicalDeviceExternalBufferPropertiesKHX();
}


#ifdef VK_USE_PLATFORM_WIN32_KHX

VKAPI_ATTR VkResult VKAPI_CALL GetMemoryWin32HandleKHX(
    VkDevice                                    device,
    VkDeviceMemory                              memory,
    VkExternalMemoryHandleTypeFlagBitsKHX       handleType,
    HANDLE*                                     pHandle)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkGetMemoryWin32HandleKHX();
    VkResult result = device_data->dispatch_table.GetMemoryWin32HandleKHX(device,memory,handleType,pHandle);
    //PostCallvkGetMemoryWin32HandleKHX();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetMemoryWin32HandlePropertiesKHX(
    VkDevice                                    device,
    VkExternalMemoryHandleTypeFlagBitsKHX       handleType,
    HANDLE                                      handle,
    VkMemoryWin32HandlePropertiesKHX*           pMemoryWin32HandleProperties)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkGetMemoryWin32HandlePropertiesKHX();
    VkResult result = device_data->dispatch_table.GetMemoryWin32HandlePropertiesKHX(device,handleType,handle,pMemoryWin32HandleProperties);
    //PostCallvkGetMemoryWin32HandlePropertiesKHX();
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
    //PreCallvkGetMemoryFdKHX();
    VkResult result = device_data->dispatch_table.GetMemoryFdKHX(device,memory,handleType,pFd);
    //PostCallvkGetMemoryFdKHX();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetMemoryFdPropertiesKHX(
    VkDevice                                    device,
    VkExternalMemoryHandleTypeFlagBitsKHX       handleType,
    int                                         fd,
    VkMemoryFdPropertiesKHX*                    pMemoryFdProperties)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkGetMemoryFdPropertiesKHX();
    VkResult result = device_data->dispatch_table.GetMemoryFdPropertiesKHX(device,handleType,fd,pMemoryFdProperties);
    //PostCallvkGetMemoryFdPropertiesKHX();
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
    //PreCallvkGetPhysicalDeviceExternalSemaphorePropertiesKHX();
    instance_data->dispatch_table.GetPhysicalDeviceExternalSemaphorePropertiesKHX(physicalDevice,pExternalSemaphoreInfo,pExternalSemaphoreProperties);
    //PostCallvkGetPhysicalDeviceExternalSemaphorePropertiesKHX();
}


#ifdef VK_USE_PLATFORM_WIN32_KHX

VKAPI_ATTR VkResult VKAPI_CALL ImportSemaphoreWin32HandleKHX(
    VkDevice                                    device,
    const VkImportSemaphoreWin32HandleInfoKHX*  pImportSemaphoreWin32HandleInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkImportSemaphoreWin32HandleKHX();
    VkResult result = device_data->dispatch_table.ImportSemaphoreWin32HandleKHX(device,pImportSemaphoreWin32HandleInfo);
    //PostCallvkImportSemaphoreWin32HandleKHX();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetSemaphoreWin32HandleKHX(
    VkDevice                                    device,
    VkSemaphore                                 semaphore,
    VkExternalSemaphoreHandleTypeFlagBitsKHX    handleType,
    HANDLE*                                     pHandle)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkGetSemaphoreWin32HandleKHX();
    VkResult result = device_data->dispatch_table.GetSemaphoreWin32HandleKHX(device,semaphore,handleType,pHandle);
    //PostCallvkGetSemaphoreWin32HandleKHX();
    return result;
}
#endif /* VK_USE_PLATFORM_WIN32_KHX */


VKAPI_ATTR VkResult VKAPI_CALL ImportSemaphoreFdKHX(
    VkDevice                                    device,
    const VkImportSemaphoreFdInfoKHX*           pImportSemaphoreFdInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkImportSemaphoreFdKHX();
    VkResult result = device_data->dispatch_table.ImportSemaphoreFdKHX(device,pImportSemaphoreFdInfo);
    //PostCallvkImportSemaphoreFdKHX();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetSemaphoreFdKHX(
    VkDevice                                    device,
    VkSemaphore                                 semaphore,
    VkExternalSemaphoreHandleTypeFlagBitsKHX    handleType,
    int*                                        pFd)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkGetSemaphoreFdKHX();
    VkResult result = device_data->dispatch_table.GetSemaphoreFdKHX(device,semaphore,handleType,pFd);
    //PostCallvkGetSemaphoreFdKHX();
    return result;
}


VKAPI_ATTR void VKAPI_CALL CmdProcessCommandsNVX(
    VkCommandBuffer                             commandBuffer,
    const VkCmdProcessCommandsInfoNVX*          pProcessCommandsInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdProcessCommandsNVX();
    device_data->dispatch_table.CmdProcessCommandsNVX(commandBuffer,pProcessCommandsInfo);
    //PostCallvkCmdProcessCommandsNVX();
}

VKAPI_ATTR void VKAPI_CALL CmdReserveSpaceForCommandsNVX(
    VkCommandBuffer                             commandBuffer,
    const VkCmdReserveSpaceForCommandsInfoNVX*  pReserveSpaceInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdReserveSpaceForCommandsNVX();
    device_data->dispatch_table.CmdReserveSpaceForCommandsNVX(commandBuffer,pReserveSpaceInfo);
    //PostCallvkCmdReserveSpaceForCommandsNVX();
}

VKAPI_ATTR VkResult VKAPI_CALL CreateIndirectCommandsLayoutNVX(
    VkDevice                                    device,
    const VkIndirectCommandsLayoutCreateInfoNVX* pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkIndirectCommandsLayoutNVX*                pIndirectCommandsLayout)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkCreateIndirectCommandsLayoutNVX();
    VkResult result = device_data->dispatch_table.CreateIndirectCommandsLayoutNVX(device,pCreateInfo,pAllocator,pIndirectCommandsLayout);
    //PostCallvkCreateIndirectCommandsLayoutNVX();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyIndirectCommandsLayoutNVX(
    VkDevice                                    device,
    VkIndirectCommandsLayoutNVX                 indirectCommandsLayout,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroyIndirectCommandsLayoutNVX();
    device_data->dispatch_table.DestroyIndirectCommandsLayoutNVX(device,indirectCommandsLayout,pAllocator);
    //PostCallvkDestroyIndirectCommandsLayoutNVX();
}

VKAPI_ATTR VkResult VKAPI_CALL CreateObjectTableNVX(
    VkDevice                                    device,
    const VkObjectTableCreateInfoNVX*           pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkObjectTableNVX*                           pObjectTable)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkCreateObjectTableNVX();
    VkResult result = device_data->dispatch_table.CreateObjectTableNVX(device,pCreateInfo,pAllocator,pObjectTable);
    //PostCallvkCreateObjectTableNVX();
    return result;
}

VKAPI_ATTR void VKAPI_CALL DestroyObjectTableNVX(
    VkDevice                                    device,
    VkObjectTableNVX                            objectTable,
    const VkAllocationCallbacks*                pAllocator)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDestroyObjectTableNVX();
    device_data->dispatch_table.DestroyObjectTableNVX(device,objectTable,pAllocator);
    //PostCallvkDestroyObjectTableNVX();
}

VKAPI_ATTR VkResult VKAPI_CALL RegisterObjectsNVX(
    VkDevice                                    device,
    VkObjectTableNVX                            objectTable,
    uint32_t                                    objectCount,
    const VkObjectTableEntryNVX* const*         ppObjectTableEntries,
    const uint32_t*                             pObjectIndices)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkRegisterObjectsNVX();
    VkResult result = device_data->dispatch_table.RegisterObjectsNVX(device,objectTable,objectCount,ppObjectTableEntries,pObjectIndices);
    //PostCallvkRegisterObjectsNVX();
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
    //PreCallvkUnregisterObjectsNVX();
    VkResult result = device_data->dispatch_table.UnregisterObjectsNVX(device,objectTable,objectCount,pObjectEntryTypes,pObjectIndices);
    //PostCallvkUnregisterObjectsNVX();
    return result;
}

VKAPI_ATTR void VKAPI_CALL GetPhysicalDeviceGeneratedCommandsPropertiesNVX(
    VkPhysicalDevice                            physicalDevice,
    VkDeviceGeneratedCommandsFeaturesNVX*       pFeatures,
    VkDeviceGeneratedCommandsLimitsNVX*         pLimits)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceGeneratedCommandsPropertiesNVX();
    instance_data->dispatch_table.GetPhysicalDeviceGeneratedCommandsPropertiesNVX(physicalDevice,pFeatures,pLimits);
    //PostCallvkGetPhysicalDeviceGeneratedCommandsPropertiesNVX();
}


VKAPI_ATTR void VKAPI_CALL CmdSetViewportWScalingNV(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstViewport,
    uint32_t                                    viewportCount,
    const VkViewportWScalingNV*                 pViewportWScalings)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdSetViewportWScalingNV();
    device_data->dispatch_table.CmdSetViewportWScalingNV(commandBuffer,firstViewport,viewportCount,pViewportWScalings);
    //PostCallvkCmdSetViewportWScalingNV();
}


VKAPI_ATTR VkResult VKAPI_CALL ReleaseDisplayEXT(
    VkPhysicalDevice                            physicalDevice,
    VkDisplayKHR                                display)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkReleaseDisplayEXT();
    VkResult result = instance_data->dispatch_table.ReleaseDisplayEXT(physicalDevice,display);
    //PostCallvkReleaseDisplayEXT();
    return result;
}

#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT

VKAPI_ATTR VkResult VKAPI_CALL AcquireXlibDisplayEXT(
    VkPhysicalDevice                            physicalDevice,
    Display*                                    dpy,
    VkDisplayKHR                                display)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkAcquireXlibDisplayEXT();
    VkResult result = instance_data->dispatch_table.AcquireXlibDisplayEXT(physicalDevice,dpy,display);
    //PostCallvkAcquireXlibDisplayEXT();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetRandROutputDisplayEXT(
    VkPhysicalDevice                            physicalDevice,
    Display*                                    dpy,
    RROutput                                    rrOutput,
    VkDisplayKHR*                               pDisplay)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetRandROutputDisplayEXT();
    VkResult result = instance_data->dispatch_table.GetRandROutputDisplayEXT(physicalDevice,dpy,rrOutput,pDisplay);
    //PostCallvkGetRandROutputDisplayEXT();
    return result;
}
#endif /* VK_USE_PLATFORM_XLIB_XRANDR_EXT */


VKAPI_ATTR VkResult VKAPI_CALL GetPhysicalDeviceSurfaceCapabilities2EXT(
    VkPhysicalDevice                            physicalDevice,
    VkSurfaceKHR                                surface,
    VkSurfaceCapabilities2EXT*                  pSurfaceCapabilities)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(physicalDevice), instance_layer_data_map);
    //PreCallvkGetPhysicalDeviceSurfaceCapabilities2EXT();
    VkResult result = instance_data->dispatch_table.GetPhysicalDeviceSurfaceCapabilities2EXT(physicalDevice,surface,pSurfaceCapabilities);
    //PostCallvkGetPhysicalDeviceSurfaceCapabilities2EXT();
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL DisplayPowerControlEXT(
    VkDevice                                    device,
    VkDisplayKHR                                display,
    const VkDisplayPowerInfoEXT*                pDisplayPowerInfo)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkDisplayPowerControlEXT();
    VkResult result = device_data->dispatch_table.DisplayPowerControlEXT(device,display,pDisplayPowerInfo);
    //PostCallvkDisplayPowerControlEXT();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL RegisterDeviceEventEXT(
    VkDevice                                    device,
    const VkDeviceEventInfoEXT*                 pDeviceEventInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkFence*                                    pFence)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkRegisterDeviceEventEXT();
    VkResult result = device_data->dispatch_table.RegisterDeviceEventEXT(device,pDeviceEventInfo,pAllocator,pFence);
    //PostCallvkRegisterDeviceEventEXT();
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
    //PreCallvkRegisterDisplayEventEXT();
    VkResult result = device_data->dispatch_table.RegisterDisplayEventEXT(device,display,pDisplayEventInfo,pAllocator,pFence);
    //PostCallvkRegisterDisplayEventEXT();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetSwapchainCounterEXT(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain,
    VkSurfaceCounterFlagBitsEXT                 counter,
    uint64_t*                                   pCounterValue)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkGetSwapchainCounterEXT();
    VkResult result = device_data->dispatch_table.GetSwapchainCounterEXT(device,swapchain,counter,pCounterValue);
    //PostCallvkGetSwapchainCounterEXT();
    return result;
}


VKAPI_ATTR VkResult VKAPI_CALL GetRefreshCycleDurationGOOGLE(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain,
    VkRefreshCycleDurationGOOGLE*               pDisplayTimingProperties)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkGetRefreshCycleDurationGOOGLE();
    VkResult result = device_data->dispatch_table.GetRefreshCycleDurationGOOGLE(device,swapchain,pDisplayTimingProperties);
    //PostCallvkGetRefreshCycleDurationGOOGLE();
    return result;
}

VKAPI_ATTR VkResult VKAPI_CALL GetPastPresentationTimingGOOGLE(
    VkDevice                                    device,
    VkSwapchainKHR                              swapchain,
    uint32_t*                                   pPresentationTimingCount,
    VkPastPresentationTimingGOOGLE*             pPresentationTimings)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkGetPastPresentationTimingGOOGLE();
    VkResult result = device_data->dispatch_table.GetPastPresentationTimingGOOGLE(device,swapchain,pPresentationTimingCount,pPresentationTimings);
    //PostCallvkGetPastPresentationTimingGOOGLE();
    return result;
}







VKAPI_ATTR void VKAPI_CALL CmdSetDiscardRectangleEXT(
    VkCommandBuffer                             commandBuffer,
    uint32_t                                    firstDiscardRectangle,
    uint32_t                                    discardRectangleCount,
    const VkRect2D*                             pDiscardRectangles)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(commandBuffer), device_layer_data_map);
    //PreCallvkCmdSetDiscardRectangleEXT();
    device_data->dispatch_table.CmdSetDiscardRectangleEXT(commandBuffer,firstDiscardRectangle,discardRectangleCount,pDiscardRectangles);
    //PostCallvkCmdSetDiscardRectangleEXT();
}



VKAPI_ATTR void VKAPI_CALL SetHdrMetadataEXT(
    VkDevice                                    device,
    uint32_t                                    swapchainCount,
    const VkSwapchainKHR*                       pSwapchains,
    const VkHdrMetadataEXT*                     pMetadata)
{
    device_layer_data *device_data = GetLayerDataPtr(get_dispatch_key(device), device_layer_data_map);
    //PreCallvkSetHdrMetadataEXT();
    device_data->dispatch_table.SetHdrMetadataEXT(device,swapchainCount,pSwapchains,pMetadata);
    //PostCallvkSetHdrMetadataEXT();
}

#ifdef VK_USE_PLATFORM_IOS_MVK

VKAPI_ATTR VkResult VKAPI_CALL CreateIOSSurfaceMVK(
    VkInstance                                  instance,
    const VkIOSSurfaceCreateInfoMVK*            pCreateInfo,
    const VkAllocationCallbacks*                pAllocator,
    VkSurfaceKHR*                               pSurface)
{
    instance_layer_data *instance_data = GetLayerDataPtr(get_dispatch_key(instance), instance_layer_data_map);
    //PreCallvkCreateIOSSurfaceMVK();
    VkResult result = instance_data->dispatch_table.CreateIOSSurfaceMVK(instance,pCreateInfo,pAllocator,pSurface);
    //PostCallvkCreateIOSSurfaceMVK();
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
    //PreCallvkCreateMacOSSurfaceMVK();
    VkResult result = instance_data->dispatch_table.CreateMacOSSurfaceMVK(instance,pCreateInfo,pAllocator,pSurface);
    //PostCallvkCreateMacOSSurfaceMVK();
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