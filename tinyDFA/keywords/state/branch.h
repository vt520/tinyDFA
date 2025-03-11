#ifndef _tinyDFA_keywords_state_branch_h
#define _tinyDFA_keywords_state_branch_h

/// @brief Informs the State that a branch process was completed on the previous execution
#define returning context->branched

/// @brief Informs the Process to branch to the provided State on next Execution
/// @param I State * Target State instance
#define branch_to_instance(I) {returning = false; context->branch = I; dfa_return;}

/// @brief Informs the Process to branch to the named State
/// @param S State name of the target State
#define branch_to(S) {branch_to_instance(State::Named<S>());}

/// @brief Used to test if the State has been re-entered from the provided instance.
/// @param I State * Target State instance
/// @return true when equal
#define returning_from_instance(I) (I == context->branch && returning)

/// @brief Used to test if the State has been re-entered from the Named instance
/// @param State name of the target State
#define returning_from(S) (returning_from_instance(State::Named<S>()))
#endif