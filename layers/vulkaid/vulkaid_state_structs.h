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
    COMMAND_BEGIN = 1,
    COMMAND_END = 2,
    COMMAND_BIND_PIPELINE = 3,
    COMMAND_SET_VIEWPORT = 4,
    COMMAND_SET_SCISSOR = 5,
    COMMAND_SET_LINE_WIDTH = 6,
    COMMAND_SET_DEPTH_BIAS = 7,
    COMMAND_SET_BLEND_CONSTANTS = 8,
    COMMAND_SET_DEPTH_BOUNDS = 9,
    COMMAND_SET_STENCIL_COMPARE_MASK = 10,
    COMMAND_SET_STENCIL_WRITE_MASK = 11,
    COMMAND_SET_STENCIL_REFERENCE = 12,
    COMMAND_BIND_DESCRIPTOR_SETS = 13,
    COMMAND_BIND_INDEX_BUFFER = 14,
    COMMAND_BIND_VERTEX_BUFFERS = 15,
    COMMAND_DRAW = 16,
    COMMAND_DRAW_INDEXED = 17,
    COMMAND_DRAW_INDIRECT = 18,
    COMMAND_DRAW_INDEXED_INDIRECT = 19,
    COMMAND_DISPATCH = 20,
    COMMAND_DISPATCH_INDIRECT = 21,
    COMMAND_COPY_BUFFER = 22,
    COMMAND_COPY_IMAGE = 23,
    COMMAND_BLIT_IMAGE = 24,
    COMMAND_COPY_BUFFER_TO_IMAGE = 25,
    COMMAND_COPY_IMAGE_TO_BUFFER = 26,
    COMMAND_UPDATE_BUFFER = 27,
    COMMAND_FILL_BUFFER = 28,
    COMMAND_CLEAR_COLOR_IMAGE = 29,
    COMMAND_CLEAR_DEPTH_STENCIL_IMAGE = 30,
    COMMAND_CLEAR_ATTACHMENTS = 31,
    COMMAND_RESOLVE_IMAGE = 32,
    COMMAND_SET_EVENT = 33,
    COMMAND_RESET_EVENT = 34,
    COMMAND_WAIT_EVENTS = 35,
    COMMAND_PIPELINE_BARRIER = 36,
    COMMAND_BEGIN_QUERY = 37,
    COMMAND_END_QUERY = 38,
    COMMAND_RESET_QUERY_POOL = 39,
    COMMAND_WRITE_TIMESTAMP = 40,
    COMMAND_COPY_QUERY_POOL_RESULTS = 41,
    COMMAND_PUSH_CONSTANTS = 42,
    COMMAND_BEGIN_RENDER_PASS = 43,
    COMMAND_NEXT_SUBPASS = 44,
    COMMAND_END_RENDER_PASS = 45,
    COMMAND_EXECUTE_COMMANDS = 46,
    COMMAND_PUSH_DESCRIPTOR_SET_KHR = 47,
    COMMAND_PUSH_DESCRIPTOR_SET_WITH_TEMPLATE_KHR = 48,
    COMMAND_DEBUG_MARKER_BEGIN_EXT = 49,
    COMMAND_DEBUG_MARKER_END_EXT = 50,
    COMMAND_DEBUG_MARKER_INSERT_EXT = 51,
    COMMAND_DRAW_INDIRECT_COUNT_AMD = 52,
    COMMAND_DRAW_INDEXED_INDIRECT_COUNT_AMD = 53,
    COMMAND_SET_DEVICE_MASK_KHX = 54,
    COMMAND_DISPATCH_BASE_KHX = 55,
    COMMAND_PROCESS_COMMANDS_NVX = 56,
    COMMAND_RESERVE_SPACE_FOR_COMMANDS_NVX = 57,
    COMMAND_SET_VIEWPORT_W_SCALING_NV = 58,
    COMMAND_SET_DISCARD_RECTANGLE_EXT = 59,
    COMMAND_TYPE_MAX = 60,
};

static const char* const command_string[COMMAND_TYPE_MAX] = {
    "COMMAND_UNKNOWN",                                // index 0
    "COMMAND_BEGIN",                                  // index 1
    "COMMAND_END",                                    // index 2
    "COMMAND_BIND_PIPELINE",                          // index 3
    "COMMAND_SET_VIEWPORT",                           // index 4
    "COMMAND_SET_SCISSOR",                            // index 5
    "COMMAND_SET_LINE_WIDTH",                         // index 6
    "COMMAND_SET_DEPTH_BIAS",                         // index 7
    "COMMAND_SET_BLEND_CONSTANTS",                    // index 8
    "COMMAND_SET_DEPTH_BOUNDS",                       // index 9
    "COMMAND_SET_STENCIL_COMPARE_MASK",               // index 10
    "COMMAND_SET_STENCIL_WRITE_MASK",                 // index 11
    "COMMAND_SET_STENCIL_REFERENCE",                  // index 12
    "COMMAND_BIND_DESCRIPTOR_SETS",                   // index 13
    "COMMAND_BIND_INDEX_BUFFER",                      // index 14
    "COMMAND_BIND_VERTEX_BUFFERS",                    // index 15
    "COMMAND_DRAW",                                   // index 16
    "COMMAND_DRAW_INDEXED",                           // index 17
    "COMMAND_DRAW_INDIRECT",                          // index 18
    "COMMAND_DRAW_INDEXED_INDIRECT",                  // index 19
    "COMMAND_DISPATCH",                               // index 20
    "COMMAND_DISPATCH_INDIRECT",                      // index 21
    "COMMAND_COPY_BUFFER",                            // index 22
    "COMMAND_COPY_IMAGE",                             // index 23
    "COMMAND_BLIT_IMAGE",                             // index 24
    "COMMAND_COPY_BUFFER_TO_IMAGE",                   // index 25
    "COMMAND_COPY_IMAGE_TO_BUFFER",                   // index 26
    "COMMAND_UPDATE_BUFFER",                          // index 27
    "COMMAND_FILL_BUFFER",                            // index 28
    "COMMAND_CLEAR_COLOR_IMAGE",                      // index 29
    "COMMAND_CLEAR_DEPTH_STENCIL_IMAGE",              // index 30
    "COMMAND_CLEAR_ATTACHMENTS",                      // index 31
    "COMMAND_RESOLVE_IMAGE",                          // index 32
    "COMMAND_SET_EVENT",                              // index 33
    "COMMAND_RESET_EVENT",                            // index 34
    "COMMAND_WAIT_EVENTS",                            // index 35
    "COMMAND_PIPELINE_BARRIER",                       // index 36
    "COMMAND_BEGIN_QUERY",                            // index 37
    "COMMAND_END_QUERY",                              // index 38
    "COMMAND_RESET_QUERY_POOL",                       // index 39
    "COMMAND_WRITE_TIMESTAMP",                        // index 40
    "COMMAND_COPY_QUERY_POOL_RESULTS",                // index 41
    "COMMAND_PUSH_CONSTANTS",                         // index 42
    "COMMAND_BEGIN_RENDER_PASS",                      // index 43
    "COMMAND_NEXT_SUBPASS",                           // index 44
    "COMMAND_END_RENDER_PASS",                        // index 45
    "COMMAND_EXECUTE_COMMANDS",                       // index 46
    "COMMAND_PUSH_DESCRIPTOR_SET_KHR",                // index 47
    "COMMAND_PUSH_DESCRIPTOR_SET_WITH_TEMPLATE_KHR",  // index 48
    "COMMAND_DEBUG_MARKER_BEGIN_EXT",                 // index 49
    "COMMAND_DEBUG_MARKER_END_EXT",                   // index 50
    "COMMAND_DEBUG_MARKER_INSERT_EXT",                // index 51
    "COMMAND_DRAW_INDIRECT_COUNT_AMD",                // index 52
    "COMMAND_DRAW_INDEXED_INDIRECT_COUNT_AMD",        // index 53
    "COMMAND_SET_DEVICE_MASK_KHX",                    // index 54
    "COMMAND_DISPATCH_BASE_KHX",                      // index 55
    "COMMAND_PROCESS_COMMANDS_NVX",                   // index 56
    "COMMAND_RESERVE_SPACE_FOR_COMMANDS_NVX",         // index 57
    "COMMAND_SET_VIEWPORT_W_SCALING_NV",              // index 58
    "COMMAND_SET_DISCARD_RECTANGLE_EXT",              // index 59
};

class Command {
   public:
    Command() : type(COMMAND_UNKNOWN) {}
    Command(COMMAND_TYPE ctype) : type(ctype) {}
    COMMAND_TYPE GetType() { return type; }

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