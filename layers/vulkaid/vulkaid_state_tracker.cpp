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

#include "vulkaid_state_tracker.h"
#include <utility>

// Including cpp due to limitation of visual studio parallel build. Really need to put this in lib
#include "vk_safe_struct.cpp"

namespace vkaid {

CommandBufferState::CommandBufferState(const VkCommandBufferAllocateInfo* alloc_info, VkCommandBuffer cmd_buffer) :
     command_buffer(cmd_buffer), create_info(new safe_VkCommandBufferAllocateInfo(alloc_info))
{
    // all current work handled via initializers
    fprintf(stdout, "Created state wrapper for cmd buffer 0x%p", command_buffer);
}

void CommandBufferState::Add(const Command cmd) { commands.push_back(cmd); }

void CommandBufferState::Display() {
    fprintf(stdout, "Cmd buffer 0x%p:", command_buffer);
    // TODO : Print commands in command buffer
}

namespace state_tracker {

void AllocateCommandBuffers(
    device_layer_data*                          device_data,
    const VkCommandBufferAllocateInfo*          pAllocateInfo,
    VkCommandBuffer*                            pCommandBuffers)
{
    device_state_struct* device_state = static_cast<device_state_struct*>(device_data->state_ptr);
    for (uint32_t i=0; i<pAllocateInfo->commandBufferCount; ++i) {
        device_state->command_buffer_map[pCommandBuffers[i]] =
            unique_ptr<CommandBufferState>(new CommandBufferState(&pAllocateInfo[i], pCommandBuffers[i]));
    }
}

void AddCommand(device_layer_data* device_data, VkCommandBuffer commandBuffer, COMMAND_TYPE ctype) {
    device_state_struct* device_state = static_cast<device_state_struct*>(device_data->state_ptr);
    device_state->command_buffer_map[commandBuffer]->Add(ctype);
}

void DisplayCommandBuffers(device_layer_data* device_data) {
    device_state_struct* device_state = static_cast<device_state_struct*>(device_data->state_ptr);
    for (auto const& cb_state_pair : device_state->command_buffer_map) {
        device_state->command_buffer_map[cb_state_pair.first]->Display();
    }
}

} // namespace vkaid
} // namespace state_tracker
