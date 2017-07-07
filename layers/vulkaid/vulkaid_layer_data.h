#ifndef __vkaid_layer_data_h_
#define __vkaid_layer_data_h_ 1

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
#include "vk_safe_struct.h"
#include <vector>
#include <unordered_map>
#include <memory>

namespace vkaid {
using std::unique_ptr;
using std::unordered_map;

struct instance_layer_data {
    VkLayerInstanceDispatchTable dispatch_table = {};
    VkInstance instance = VK_NULL_HANDLE;
    InstanceExtensions extensions = {};
    // Ptr to opaque state wrappers that are defined in state layer
    void *state_ptr = nullptr;
};

struct device_layer_data {
    VkLayerDispatchTable dispatch_table = {};
    instance_layer_data *instance_data = nullptr;
    // Physical Device Data
    VkPhysicalDevice physical_device = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties physical_device_properties = {};
    std::vector<VkQueueFamilyProperties> queue_family_properties;
    VkPhysicalDeviceFeatures enabled_features = {};
    VkPhysicalDeviceMemoryProperties physical_device_mem_properties = {};
    // Device-specific State
    VkDevice device = VK_NULL_HANDLE;
    DeviceExtensions extensions = {};
    // Ptr to opaque state wrappers that are defined in state layer
    void *state_ptr = nullptr;
};

static unordered_map<void *, device_layer_data *> device_layer_data_map;
static unordered_map<void *, instance_layer_data *> instance_layer_data_map;

}  // namespace vkaid

#endif  // #define __vkaid_layer_data_h_ 1