#ifndef __vulkaid_state_view_h_
#define __vulkaid_state_view_h_ 1

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

#include "vulkaid_state_structs.h"

namespace vkaid {

class StateView {
   public:
    StateView(const device_state_struct *state) : device_state(state){};
    // Display command buffer state
    void DisplayCommandBuffers() const {
        for (auto const &cb_state_pair : device_state->command_buffer_map) {
            const auto &command_buffer = cb_state_pair.first;
            auto commands = device_state->command_buffer_map.find(command_buffer)->second->Get();
            fprintf(stdout, "Cmd buffer 0x%p:\n", command_buffer);
            // Print commands in command buffer
            for (uint32_t i = 0; i < commands.size(); ++i) {
                fprintf(stdout, "   %s\n", command_string[static_cast<uint32_t>(commands[i].GetType())]);
            }
        }
    }

   private:
    const device_state_struct *device_state;
};

}  // namespace vkaid
#endif  // __vulkaid_state_view_h_