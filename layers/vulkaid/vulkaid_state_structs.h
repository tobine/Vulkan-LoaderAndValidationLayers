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
#include "vk_safe_struct.h"
#include <vector>
#include <unordered_map>
#include <memory>

namespace vkaid {
using std::unordered_map;
using std::unique_ptr;

struct instance_layer_data {
    VkLayerInstanceDispatchTable dispatch_table = {};
    VkInstance instance = VK_NULL_HANDLE;
    InstanceExtensions extensions = {};
    // TODO : Extend struct w/ further custom data
};

// Device-level state containers. Pre-declare here and define in cpp file where used
//class CommandBufferState;
// Define state wrapper classes
class CommandBufferState {
public:
    CommandBufferState(const VkCommandBufferAllocateInfo*, VkCommandBuffer);
private:
    VkCommandBuffer command_buffer;
    unique_ptr<safe_VkCommandBufferAllocateInfo> create_info;
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
    unordered_map<VkCommandBuffer, unique_ptr<CommandBufferState>> command_buffer_map;
    // TODO : Extend struct w/ further custom data

    // declare constructor and destructor to prevent default destructor which
    //  breaks when using unique_ptr w/ incomplete type
//    device_layer_data();
//    ~device_layer_data();
};

static unordered_map<void *, device_layer_data *> device_layer_data_map;
static unordered_map<void *, instance_layer_data *> instance_layer_data_map;

}

#endif