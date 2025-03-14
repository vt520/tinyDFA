#ifndef _tinyDFA_Process_h
#define _tinyDFA_Process_h
#include "debug.h"
#include "types.h"
#include "State.h"
#include "State/Stop.h"
#include "State/Error.h"
#include "Context.h"

namespace tiny {
  namespace DFA {
    Require_State(Stop);

    class Process {
      private:
      
      Content_Ptr requested_content = nullptr;

      /// @brief Pointer to the currently loaded state
      State * current = nullptr;

      /// @brief The context for this process
      Context * context = new Context();

      /// @brief Pointer to a child Process
      Process * child = nullptr;

      void UnloadChild(bool halt = true) {
        if (child) {
          if (halt) child->Halt();
          delete child;
          child = nullptr;
          extended_debug_literal(">> Remove Child");
        }
      }

      /// @brief Attempts to Load/Execute a branch when requested
      /// @return true if a branch has been loaded or executed; false otherwise
      bool ExecutedBranch() {
        // Test if we should load a child process
        if (!context->branched && context->branch) {
          // Start Child Process
          extended_debug_literal(">> New Child");
          child = new Process(context->branch);
          context->branched = true;
          context->next = nullptr;
        }
        // If we have a child, execute it
        if (child) {
          // if the child is still executing, return true
          if(child->Execute()) return true;
          // remove the child 
          UnloadChild(false);
        }
        // No meaningful Branch action taken, return false
        return false;
      }

      /// @brief Attempts to execute the current State; and (optionally) performs timekeeping
      /// @return true if we have a valid State
      bool ExecutedCurrent() {
        if (current) {
          context->evaluation ++;
          current->Execute(context);
          context->execute_time += context->ElapsedTime();
          // Reset the branched flag
          if (context->branched)  {
            context->branch = nullptr;
            context->branched = false;
          }
        }
        return current;
      }

      /// @brief Attempts to change the current State is required
      /// @return true if a new State has been made current
      bool ChangedCurrent() {
        if (context->next) {
          if (current) {
            current->Exit(context);
          }
          // Special processing for Stop States
          current = State::Named<Stop>() == context->next ? nullptr : context->next;

          // if we have a valid state
          if (current) {
            context->next = nullptr;
            context->branch = nullptr;
            context->evaluation = 0;
            context->branched = false;
            context->ElapsedTime(true);
            current->Enter(context);
          }
          return current;
        }
        // No context switch occurred
        return false;
      }
      public:
      Process (State * init, Content_Ptr content) {
        if (!init) init = State::Named<Stop>();
        Start(init, content);
      }

      /// @brief Create a process using an initial State object
      /// @param init The initial state for this process
      Process (State * init) : Process(init, nullptr) { }

      /// @brief Removes automatically allocated resources
      ~Process() {
        delete context;
      }

      /// @brief Create a default process
      Process() : Process(nullptr) {  
        
      }
      /// @brief Creates a new process using a well-known State object
      /// @tparam KnownState the class name of the well-known State to use as the initial state
      /// @return Process loaded with the requested state
      template <typename KnownState> static Process * Using(Content_Ptr content = nullptr) {
        return new Process(State::Named<KnownState>(), content);
      }
      
      template <class KnownProcess> 
      static Process * Named(State * state = nullptr, Content_Ptr * content = nullptr) {
        static Process * instance = nullptr;
        if(!instance) instance = new KnownProcess(state);
        else if(state) instance->Start(state, content);
        return instance;
      }
      
      /// @brief Executes one step of the given process
      /// @return true if the Process is evaluating a valid State
      bool Execute() {
        // Obtain the amount of time elapsed since the last evaluation
        Time elapsed = context->ElapsedTime(true);
        
        // Perform an Idle test, and return if true
        if (context->Idle(elapsed)) return true; 

        // Attempt to Execute a branch
        if (ExecutedBranch()) return true;

        // Attempt to select (Exit/Enter) a new Current state
        if (ChangedCurrent()) return true;

        // Attempt to Execute the current state
        return ExecutedCurrent();
      }

      /// @brief Select the supplied State during the next execution (this will stop any child branch)
      /// @param state The state that the process should select
      /// @return true (Always succeeds)
      bool Select(State * state) {
        if (child) {
          child->Halt();
          child = nullptr;
          delete child;
        }
        if (state != current) context->next = state;
        return true;
      }

      
      /// @brief Instructs the Process to ignore any delay; and immediately execute the selected process
      /// @param state The state that the process should select
      /// @return true when the process has executed a state
      bool Immediate() {
        if (child) return child->Immediate();
        context->delay = 0;
        return Execute();

      }
      State * Halt() {
        State * halted_state = current;
        if (child) child->Halt();
        if (halted_state) {
          Select(State::Named<tiny::DFA::Stop>());
          Immediate();
        } 
        return halted_state;
      }

      State * Selected() {
        return current ? current : State::Named<tiny::DFA::Stop>();
      }

      State * Active() {
        State * active = nullptr;
        if (child) active = child->Active();
        if (!active) active = Selected();
        return active;
      }

      Content_Ptr Content() {
        return context->content;
      }

      void * SetContent(void * content) {
        State * halted_state = Halt();
        context->content = content;
        return halted_state;
      }

      void Start(State * state, Content_Ptr content) {
        SetContent(content);
        Select(state);
        Immediate();
      }
    };

    
    /// @brief The default process presented by tinyDFA
    Process* process;    
  }
}

#include "structure/process.h" // Must remain at end of file
#endif