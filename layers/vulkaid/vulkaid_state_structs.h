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

#include "vk_safe_struct.h"
#include <unordered_map>
#include <memory>

namespace vkaid {
using std::unordered_map;
using std::unique_ptr;

struct instance_state_struct {
    // TODO : Define custom instance state here
};

// Define state wrapper classes
class CommandBufferState {
public:
    CommandBufferState(const VkCommandBufferAllocateInfo*, VkCommandBuffer);
private:
    VkCommandBuffer command_buffer;
    unique_ptr<safe_VkCommandBufferAllocateInfo> create_info;
};

struct device_state_struct {
    unordered_map<VkCommandBuffer, unique_ptr<CommandBufferState>> command_buffer_map;
};

}

#endif