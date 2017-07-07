#ifndef __vkaid_state_structs_h_
#define __vkaid_state_structs_h_ 1

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

#include "vk_dispatch_table_helper.h"
#include "vk_extension_helper.h"
#include <vector>
#include <unordered_map>

namespace vkaid {
using std::unordered_map;

struct instance_dispatch_data {
    VkLayerInstanceDispatchTable dispatch_table = {};
    VkInstance instance = VK_NULL_HANDLE;
    InstanceExtensions extensions = {};
};

struct device_dispatch_data {
    VkLayerDispatchTable dispatch_table = {};
    instance_dispatch_data *instance_data = nullptr;
    // Physical Device Data
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties physical_device_properties = {};
    std::vector<VkQueueFamilyProperties> queue_family_properties;
    VkPhysicalDeviceFeatures enabled_features = {};
    VkPhysicalDeviceMemoryProperties physical_device_mem_properties = {};
    // Device-specific State
    VkDevice device = VK_NULL_HANDLE;
    DeviceExtensions extensions = {};
};

static unordered_map<void *, instance_dispatch_data *> instance_dispatch_data_map;
static unordered_map<void *, device_dispatch_data *> device_dispatch_data_map;

}  // namespace vkaid

#endif