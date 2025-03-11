#ifndef _tinyDFA_keywords_state
#define _tinyDFA_keywords_state

#include "../Context.h"

/// @brief Forward-Declare a State name
#define Require_State(X) class X

/// @brief Declare a State
/// @param S State name
/// @returns State
#define State(S) class S : public State

/// @brief Provides the parameter list for State events
#define State_Parameters Context * context

/// @brief Declare the body for Enter method
/// @param context The current execution context
#define On_Enter void Enter (State_Parameters)

/// @brief Declare the body for Exit method
/// @param context The current execution context
#define On_Execute void Execute (State_Parameters)

/// @brief Declare the body for Exit method
/// @param context The current execution context
#define On_Exit void Exit (State_Parameters)

#endif