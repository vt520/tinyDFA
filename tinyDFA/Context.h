#ifndef _tinyDFA_Context_h
#define _tinyDFA_Context_h

#include "types.h"

namespace tiny {
  namespace DFA {
    class State;
    /// @brief Contains an Execution context between a Process and a State
    
    class Context {
      private:
      /// @brief The number of Time Units since last check, used by ElapsedTime()
      Time last_check_time = 0;
      
      public:
      /// @brief When set (and branch is unset) Process should select this State as current on the next Execution
      State * next = nullptr;

      /// @brief When set (and branched is unset) Process should branch to this state on next Execution
      State * branch = nullptr;

      /// @brief Indicates that this Execution follows a branch exit
      bool branched = false;

      /// @brief Counts the number of times that Execute has been called on this State
      Count evaluation = 0;

      /// @brief Set to the amount of actual time that the State spent in the Execute function
      Time execute_time = 0;
      
      /// @brief Instructs the Processor to wait the given Time Units until Executing another action
      Time delay = 0;

      /// @brief Creates a new Context
      Context() {
        ElapsedTime(true);
      }

      /// @brief Gets the amount of Time Units since the timer was reset
      /// @param reset Resets the internal timer
      /// @return Time elapsed since last reset
      Time ElapsedTime(bool reset = false) {
        Time now = time();
        Delta elapsed = now - last_check_time;
        if (reset) last_check_time = now;
        return delta_time(elapsed);
      }

      /// @brief Decrements the delay by at most elapsed Time units
      /// @param elapsed The amount of Time units that should be removed
      /// @return true when delay is non-zero
      bool Idle(Time elapsed = 0) {
        Time decrement = min(delay, elapsed);
        //Serial.println(delay);
        delay -= decrement;
        return (delay > 0);
      }
    };
  }
}

#endif