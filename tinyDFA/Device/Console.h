#ifndef _tinyDFA_State_Output_h
#define _tinyDFA_State_Output_h
#include "../Device.h"
#include "../keywords.h"
#include "../types.h"
#include "../config.h"

namespace tiny {
    namespace DFA {
        /// @brief The Console Stream Device
        Device(Console,Stream) {
            On_Execute {
                if (!serial_device_active) {
                    tinyDFA_SERIAL_DEVICE.begin(tinyDFA_SERIAL_SPEED);
                    extended_debug_literal("SERIAL OUTPUT STARTED");
                    context->delay = tinyDFA_SERIAL_DELAY_MS;
                }
                resource = & tinyDFA_SERIAL_DEVICE;
                continue_to(tiny::DFA::Stop);
            }
        };
        /// @brief The Default console pointer; must be initialized _before_ use
        Stream * console = device(Console, Stream)->resource;
    }
}

#endif