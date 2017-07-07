#ifndef __vulkaid_state_controller_h_
#define __vulkaid_state_controller_h_ 1

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
#include "vulkaid_state_view.h"

namespace vkaid {

class StateController {
   public:
    StateController(const device_state_struct *state, const StateView *view) : device_state(state), view(view) {}
    void HandlePresent() { view->DisplayCommandBuffers(); }

   private:
    const device_state_struct *device_state;
    const StateView *view;
};

}  // namespace vkaid
#endif  // __vulkaid_state_controller_h_