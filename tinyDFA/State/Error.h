#ifndef _tinyDFA_State_Error_h
#define _tinyDFA_State_Error_h
#include "../State.h"
#include "../types.h"
#include "../keywords.h"

namespace tiny {
  namespace DFA {
    /// @brief State that Indicates an unrecoverable error; stops _all_ evaluation and prints a message to the debug device 
    State(Error) {
      On_Enter {
        extended_debug_literal("HALT");
      }
      On_Execute {
        context->delay = millis_time(500);
      }
    };
  }
}
#endif