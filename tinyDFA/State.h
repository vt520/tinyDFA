#ifndef _tinyDFA_State_h
#define _tinyDFA_State_h
#include "debug.h"

#include "structure/state.h"

namespace tiny {
  namespace DFA {
    // Forward Declare Required Objects
    class Process;
    class Stop;
    class Error;
    
    /// @brief Represents a Discrete Finite Automata Sate
    class State {
      public:
      /// @brief Entry point for State Initialization
      /// @param context The current execution context
      virtual On_Enter {extended_debug_emit_location;}

      /// @brief Entry point for State Execution
      /// @param context The current execution context
      virtual On_Execute {extended_debug_emit_location;};

      /// @brief Entry point for State Exit
      /// @param context The current execution context
      virtual On_Exit {extended_debug_emit_location;}

      /// @brief Convenience factory to fetch known instances of State objects
      /// @tparam KnownState the class name of a State
      /// @return A well-known instance of the given State type
      public : template <class KnownState> static  State * Named() {
        static KnownState * instance = new KnownState();
        return instance;
      }
      /// @brief Tests if a State pointer matches the well-known instance of a State
      /// @tparam KnownState the class name of a well-known State
      /// @param state the State pointer to test
      /// @return true on match
      public : template <class KnownState> static bool Is(State * state) {
        return (State::Named<KnownState>() == state);
      }
    };    
  }
}

#endif