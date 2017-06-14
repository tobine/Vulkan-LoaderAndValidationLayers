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

#include "vulkaid_dispatch.h"
// Include vulkaid modules here
#include "vulkaid_state_tracker.h"
#include <memory>

namespace vkaid {

void PostCallCreateDevice(device_layer_data* device_data, const VkDeviceCreateInfo* pCreateInfo,
                          const VkAllocationCallbacks* pAllocator, VkDevice* pDevice) {
    device_data->state_ptr = static_cast<void*>(new device_state_struct());
}

void PostCallDestroyDevice(device_layer_data* device_data, const VkAllocationCallbacks* pAllocator) {
    delete static_cast<device_state_struct*>(device_data->state_ptr);
}

void PostCallAllocateCommandBuffers(
    device_layer_data*                          device_data,
    const VkCommandBufferAllocateInfo*          pAllocateInfo,
    VkCommandBuffer*                            pCommandBuffers)
{
    state_tracker::AllocateCommandBuffers(device_data, pAllocateInfo, pCommandBuffers);
}

} // namespace vkaid
