#ifndef _tinyDFA_keywords_state_basic_h
#define _tinyDFA_keywords_state_basic_h

/// @brief Prints debug information and exits the current function
#define dfa_return {extended_debug_emit_location; return;}

/// @brief Informs the Process to select a specified State as current 
/// @param I State * of the target state
#define continue_to_instance(I) {context->next = I; dfa_return;}

/// @brief Informs the Process to select the named State as current
/// @param S State Name 
#define continue_to(S) {continue_to_instance(State::Named<S>());}

#endif