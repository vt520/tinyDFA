# tinyDFA Model

## State 
- Reference / Instance
  - **`state(S)`** `State::Named<State>()`
- Receives **Events** with a **Context**
  - **`On_Enter`** `Enter (context)`
  - **`On_Execute`** `Execute (context)`
  - **`On_Exit`** `Exit (context)`
- Informs State transitions 
  - **`continue_to(S)`** `context->next`
  - **`branch_to(S)`** `context->branch` `context->branched`
- Manipulates Context Content
  - **`content()`** `content(T)` **`set_content(V)`** `context->value`
- Informs and Adjusts State Timing
  - **`wait_for(N)`** `context->delay`
  - **`latency()`** `context-latency`

## Process
- Application and States create Processes
  - **`Process(P)`** `Process::()`
  - **`Process(P,S)`** `Process::Using<State>()`
  - **`process(P)`** `Process::Named<Process>()`
- Applications and States Inform Processes
  - **`select_process(P,S)`** `process->Select(S)`
  - **`process_selected_state(P)`** `process->Selected()`
  - **`process_active_state(P)`** `process->Active()`
  - **`execute_process(P)`** `process->Execute()`
  - **`activate_process(P)`** `process->Immediate()`
  - **`stop_process(P)`** `process->Stop()`
- Applications and States can Inspect and Inform Content
  - **`process_content(P)`** `process_content(P,T)` `process->Content()`
  - `**process_set_content(P,V)**` `process->SetContent(* value)`
- Processing Execution Algorithm
  1. Only when wait time has expired
     1. Start a child process if needed
     2. Only when there is a child process
        1. Execute the child process
        2. If the child executed successfully then Exit execution successfully
        3. Set the branched indicator
     3. Only when the next State is known
        1. Exit the current State
        2. Select the next state as current
        3. Only when content changes
           1. Update Context content
        4. Clear the next state
        5. Enter the current state then Exit execution successfully
     4. Only when the current State is valid
        1. Execute the current state
        2. Exit execution successfully
     5. Exit execution failed
  2. Exit execution successfully
## Device

## Context