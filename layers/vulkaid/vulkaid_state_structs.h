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
#include <vector>

namespace vkaid {
using std::unordered_map;
using std::unique_ptr;
using std::vector;

struct instance_state_struct {
    // TODO : Define custom instance state here
};

enum COMMAND_TYPE {
    COMMAND_UNKNOWN = 0,
    COMMAND_BIND_PIPELINE = 1,
    COMMAND_SET_VIEWPORT = 2,
    COMMAND_SET_SCISSOR = 3,
    COMMAND_SET_LINE_WIDTH = 4,
    COMMAND_SET_DEPTH_BIAS = 5,
    COMMAND_SET_BLEND_CONSTANTS = 6,
    COMMAND_SET_DEPTH_BOUNDS = 7,
    COMMAND_SET_STENCIL_COMPARE_MASK = 8,
    COMMAND_SET_STENCIL_WRITE_MASK = 9,
    COMMAND_SET_STENCIL_REFERENCE = 10,
    COMMAND_BIND_DESCRIPTOR_SETS = 11,
    COMMAND_BIND_INDEX_BUFFER = 12,
    COMMAND_BIND_VERTEX_BUFFERS = 13,
    COMMAND_DRAW = 14,
    COMMAND_DRAW_INDEXED = 15,
    COMMAND_DRAW_INDIRECT = 16,
    COMMAND_DRAW_INDEXED_INDIRECT = 17,
    COMMAND_DISPATCH = 18,
    COMMAND_DISPATCH_INDIRECT = 19,
    COMMAND_COPY_BUFFER = 20,
    COMMAND_COPY_IMAGE = 21,
    COMMAND_BLIT_IMAGE = 22,
    COMMAND_COPY_BUFFER_TO_IMAGE = 23,
    COMMAND_COPY_IMAGE_TO_BUFFER = 24,
    COMMAND_UPDATE_BUFFER = 25,
    COMMAND_FILL_BUFFER = 26,
    COMMAND_CLEAR_COLOR_IMAGE = 27,
    COMMAND_CLEAR_DEPTH_STENCIL_IMAGE = 28,
    COMMAND_CLEAR_ATTACHMENTS = 29,
    COMMAND_RESOLVE_IMAGE = 30,
    COMMAND_SET_EVENT = 31,
    COMMAND_RESET_EVENT = 32,
    COMMAND_WAIT_EVENTS = 33,
    COMMAND_PIPELINE_BARRIER = 34,
    COMMAND_BEGIN_QUERY = 35,
    COMMAND_END_QUERY = 36,
    COMMAND_RESET_QUERY_POOL = 37,
    COMMAND_WRITE_TIMESTAMP = 38,
    COMMAND_COPY_QUERY_POOL_RESULTS = 39,
    COMMAND_PUSH_CONSTANTS = 40,
    COMMAND_BEGIN_RENDER_PASS = 41,
    COMMAND_NEXT_SUBPASS = 42,
    COMMAND_END_RENDER_PASS = 43,
    COMMAND_EXECUTE_COMMANDS = 44,
    COMMAND_PUSH_DESCRIPTOR_SET_KHR = 45,
    COMMAND_PUSH_DESCRIPTOR_SET_WITH_TEMPLATE_KHR = 46,
    COMMAND_DEBUG_MARKER_BEGIN_EXT = 47,
    COMMAND_DEBUG_MARKER_END_EXT = 48,
    COMMAND_DEBUG_MARKER_INSERT_EXT = 49,
    COMMAND_DRAW_INDIRECT_COUNT_AMD = 50,
    COMMAND_DRAW_INDEXED_INDIRECT_COUNT_AMD = 51,
    COMMAND_SET_DEVICE_MASK_KHX = 52,
    COMMAND_DISPATCH_BASE_KHX = 53,
    COMMAND_PROCESS_COMMANDS_NVX = 54,
    COMMAND_RESERVE_SPACE_FOR_COMMANDS_NVX = 55,
    COMMAND_SET_VIEWPORT_W_SCALING_NV = 56,
    COMMAND_SET_DISCARD_RECTANGLE_EXT = 57,
};

class Command {
   public:
    Command() : type(COMMAND_UNKNOWN) {}
    Command(COMMAND_TYPE ctype) : type(ctype) {}

   private:
    COMMAND_TYPE type;
};

// Define state wrapper classes
class CommandBufferState {
public:
    CommandBufferState(const VkCommandBufferAllocateInfo*, VkCommandBuffer);
    // Add command to command buffer
    void Add(const Command);
    // Display this comand buffer
    void Display();

   private:
    VkCommandBuffer command_buffer;
    unique_ptr<safe_VkCommandBufferAllocateInfo> create_info;
    vector<Command> commands;
};

struct device_state_struct {
    unordered_map<VkCommandBuffer, unique_ptr<CommandBufferState>> command_buffer_map;
};

}

#endif