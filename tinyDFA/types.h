#ifndef _tinyDFA_types_h
#define _tinyDFA_types_h

#include "config.h"

namespace tiny {
  namespace DFA {
    using Content_Ptr = void *;
    
    /// @brief Convenience type for Time units
    using Time = unsigned long;

    /// @brief Convenience type for math operations on Time units
    using Delta = long;

    /// @brief Convenience type for Index items
    using Count = unsigned long;

    /// @brief Fetches the current runtime in Time units
    /// @return The current system tim in Time units
    Time time() { 
      return micros();
    }

    /// @brief Convert Milliseconds to Time units
    /// @param millis Milliseconds required
    /// @return Time units corresponding to the supplied Milliseconds
    Time millis_time(unsigned long millis) {
      return millis * tinyDFA_TU_MILLIS;
    }
    
    /// @brief Converts a Delta units to Time units
    /// @param delta a Delta between two Times
    /// @return Time units corresponding to the Delta
    Time delta_time(Delta delta) {
      return delta >= 0 ? delta : tinyDFA_TU_MAX + delta;
    }
  }
}

#endif