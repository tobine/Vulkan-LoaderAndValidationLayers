#ifndef __vulkaid_state_tracker_h_
#define __vulkaid_state_tracker_h_ 1

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

#include "vulkaid_layer_data.h"
#include "vulkaid_state_structs.h"

namespace vkaid {

namespace state_tracker {

void AllocateCommandBuffers(
    device_layer_data*                          device_data,
    const VkCommandBufferAllocateInfo*          pAllocateInfo,
    VkCommandBuffer*                            pCommandBuffers);

} // close namespace state_tracker
} // close namespace vkaid
#endif