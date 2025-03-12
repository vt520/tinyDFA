
# tinyDFA - An Easy, Powerful, Automata Framework
> Preview Release

Welcome to `tinyDFA`, an Arduino library for designing responsive projects with Discrete Finite Automata.

## Incomplete Features (Nearly done.)
- Advanced timing adjustment
  - Enable detecting non-execution time
- Generic Contexts
  - Context<T>->T * value // BasicContext : Context<bool>
- Generalize State to accept generalized Context
  - State(Name) : State<bool>
  - State(Name, ContextType) : State<ContextType>
    - ie: State(SetupCamera, CameraDevice) where context->value is CameraDevice *
- Process launching with Context Value
  - process = Process:Using<SetupCamera>(camera_context);
- Documentation cleanup and segmentation
  - ie: move parts of this into docs/


What is a **Discrete Finite Automata**, you ask?  Simple; a **DFA** is a method of modeling a _System_ as a graph of connected _States_.  

If we _imagine_ a very basic Arduino project, we often have several states we may find our device in.  At high level we _Setup_, _Acquire Data_, _Process_, then  _Respond_.  This seems trivial at first glance; We should be able easily transition from one to the other... but many would-be creators soon find themselves in a morass of unexpected problems (mostly deeply nested conditions and obscure flags) attempting to do so.

There is a better way, `tinyDFA` Lets you keep the Trivial model trivial; for example a basic Project could be defined using the following:

```cpp
State(Respond) {
    On_Execute {
        // Respond to the processed data
        continue_to(Acquire);
    }
}
State(Process) {
    On_Execute {
        // Process acquired data
        continue_to(Respond);
    }
}
State(Acquire) {
    On_Execute {
        // Acquire data for processing
        continue_to(Process);
    }
}
State(Setup) {
    On_Execute {
        // Setup our project
        continue_to(Acquire);
    }
}
```

Executing a State is easy; we create a Process using our `Setup` State, and we ask the Process to Execute the current state, and continue as needed.  This proceeds until a State continues to a `Stop`

```cpp
void setup() {
    // Set the default process to a new Process
    process = Process::Using<Setup>();
}
void loop() {
    process->Execute(); // Execute a process step
}
```

You can also easy create a Cooperative Multi-Tasking (CMT) project by creating multiple processes and executing them in turn, for example

```cpp
void setup() {
    // setup processes.....
}
void loop() {
    // you can have as many processes as memory allows
    for (int i = 0; i < sizeof(processes); i++)
        processes[i]->Execute();
}
```

Each State is always presented with a handy Context that you can use to create rich behaviors, or effect the next Process step.
If you want or need millisecond precision timing in your code (Animations, Sampling, Etc); you can use delay member of the context of your State to inform the Process how long an Execution must take, it's easy:
```cpp
State(AnimationStep) {
    On_Execute { 
        // This construction is only reliable when load is low (ie: Process->Execute is called _very_ often)
        // Using the timekeeping extension context->latency to adjust delay under load
        Time max_frame_time = millis_time(100); 
        if (context->evaluation == 10) continue_to(NextAnimationStep);
        /** Inform the Process to wait 100ms from the /start/ 
         * of execution to perform the next action
         */
        context->delay = max_frame_time;
        if (context->ElapsedTime() > max_time_time) continue_to(tiny::DFA::Error);
    }
}
```

`tinyDFA` also presents a powerful construct known as branching.  A branch is a State that grows from a pre-existing state.  Unlike continuation, a branch will return back to the State that created it, allowing for a structured readable orchestration of States.

```cpp
State(Setup) {
    On_Enter {
        // cannot use Console device yet, not started
        if(!device(Console,Stream)->Ready()) {
            extended_debug_literal("Console not ready");
        } else {
            resource(Console,Stream)->println("Hello")
        }
    }
    On_Execute {
        // Start Devices for Console, SD_Storage, Wifi, HTTP
        // Then continue to Running
        if(returning_from(HTTP))            continue_to(Running);
        if(returning_from(Wifi))            branch_to(Server);
        if(returning_from(SD_Storage))      branch_to(Wifi);
        if(returning_from(Console))         branch_to(SD_Storage);
                                            branch_to(Console);
        // branches are programed in reverse order for ease of coding
    }
    On_Exit {
        resource(Console,Stream)->println("Completed Setup");
    }
}
```

There is even a _simple_ driver model; where a State takes responsibility for a static resource that may be shared by any state or context.  There is a sample device provided `Console` that gives access to the Stream methods of a HardwareSerial device.  By design, Device States should be branched_to from a Process (as they should Stop when set up)

Best of all?  `tinyDFA` simplifies development while allowing access to the full suite of and power of existing Arduino libraries.

## tinyDFA Releases
Download the newest release from release/*/tinyDFA.zip.  Stable versions will be noted by the absence of the "development_release" marker.

## Building the tinyDFA library from sources
1. git-clone this repository
2. execute ./build_library.sh
3. install the resulting tinyDFA.zip into the Arduino IDE / Markers Workshop / Platform IO

## Upcoming `tinyDFA` Features
- Branch Caller Identification
- Context Aware Devices
- Modularity
  - OMIT_DFA_TIMEKEEPING  
  Omit Timekeeping Functionality
    - context->delay >> #context_delay(Time)
    - context-runtime >> #context_runtime
    - context->ElapsedTime() >> #context_elapsed
    - context->Idle()
  - OMIT_DFA_BRANCHING  
  Omit Branching Functionality
    - context->branch 
    - context->branched
- tinyAF Automata Framework
  - Inter-Process Communication
  - Power Management
  - Process Orchestration Extensions

## Using tinyDFA

Include and use the tinyDFA scaffold
```cpp
#include <tinyDFA.h>
using namespace tiny::DFA;
```

Define a State Machine
```cpp
State(MyFirstState) {
    On_Enter {
        Serial.begin(115200);
    }
    On_Execute {
        Serial.println("Hello world");
        continue_to(tiny::DFA::Stop);
    }
    On_Exit {
        Serial.println("Bye!");
    }
};
```

Create a process
```cpp
void setup () {
    process = Process::Using<MyFirstState>();
}
```

Execute the process until it stops
```cpp
void loop () {
    if (!process->Execute()) halt("Finished");
}
```

## tiny DFA Keywords

### Require_State(S)
Declares that the state **S** may potentially exist in another location

| Type        | Value                                                  |
|:-----------:|--------------------------------------------------------|
| Replacement | `class S`                                              |
---
```cpp
Require_State(NextState);
State(SomeState) {
    On_Enter {
        continue_to(NextState);
    }
};
```

### State(S)
> Defines a State **S**

| Type        | Value                                                  |
|:-----------:|--------------------------------------------------------|
| Replacement | `class S : public State`                               |
---
```cpp
State(SomeState) {
    On_Execute { 
        // body
    }
};
```

### Device(S,T)
> Defines a Device named **S** that is responsible for a resource of type **T**


| Type        | Value                                                  |
|:-----------:|--------------------------------------------------------|
| Replacement | `class S : public Device<T>`                           |
---
```cpp
Device(SomeDevice,Stream) { 
    Is_Ready { return false; }
};
```

### On_Enter
> Defines the actions taken when the state is selected as current by a Process


| Type        | Value                                                  |
|:-----------:|--------------------------------------------------------|
| Replacement | `protected: void Enter (Context * context) override`   |
---
```cpp
State(SomeState) {
    On_Enter {
        // initialization
    }
};
```

### On_Execute
> Defines the actions taken when the state is activated a Process

| Type        | Value                                                  |
|:-----------:|--------------------------------------------------------|
| Replacement | `protected: void Execute (Context * context) override` |
---
```cpp
State(SomeState) {
    On_Execute {
        // periodic execution
    }
};
```

### On_Exit
> Defines the actions taken when the state is being replaced with another as current by a Process

| Type        | Value                                                  |
|:-----------:|--------------------------------------------------------|
| Replacement | `protected: void Exit (Context * context) override`    |
---
```cpp
State(SomeState) {
    On_Exit {
        // clean up
    }
};
```

### Is_Ready
> Tests if a Device State has been properly configured

| Type        | Value                                                  |
|:-----------:|--------------------------------------------------------|
| Replacement | `public: bool Ready () override`                       |
---
```cpp
Device(SomeState) {
    Is_Ready {
        return device;
    }
};
```

### extended_debug_emit_location
> Print the line number and function signature to the console

| Type      | Value |
|-----------|:-----:|
| Statement | None  |
---

```cpp
void setup() {
    extended_debug_emit_location;
}
```

### extended_debug_literal(S)
> Print the message and function signature to the console

| Type      | Value |
|-----------|:-----:|
| Statement | None  |
---

```cpp
void loop() {
    extended_debug_literal("Loop Message");
}
```

### halt(S)
> Print the message and function signature to the console and stops execution

| Type      | Value |
|-----------|:-----:|
| Statement | None  |
---

```cpp
void loop() {
    if(!process->Execute()) halt("Completed");
}
```

### extended_debug_variables(V)
> Print the variable and function signature to the console 

| Type      | Value |
|-----------|:-----:|
| Statement | None  |
---
| Parameter | Notes                    |
|:---------:|--------------------------|
| V         | Any Variable or Constant |

```cpp
State(SomeState) {
    On_Execute {
        extended_debug_variables(context->evaluation);
    }  
};
```

### dfa_return
> `Internal` Emits debug information and exits from the current method

| Type      | Value |
|-----------|:-----:|
| Statement | None  |
---
```cpp
void loop() {
    if (!process->Execute()) dfa_return;
}
```

### device(D,T)
> Get a device pointer

| Type      | Value       |
|-----------|:-----------:|
| Function  | Device&lt;T&gt;&ast;  |
---
| Parameter | Notes                    |
|:---------:|--------------------------|
| D         | Device State Name        |
| T         | Device Resource Type     |

### resource(D, T)
> Get the resource pointer held by a Device object.  This _will_ Error if the device is not Ready

| Type      | Value       |
|-----------|:-----------:|
| Function  | T&ast;  |
---
| Parameter | Notes                    |
|:---------:|--------------------------|
| D         | Device State Name        |
| T         | Device Resource Type     |

```cpp
static Stream * Output = resource(Console,Stream);
State(SomeState) {
    On_Enter {
        Output->println("Hello World");
    }
};
```

### branch_to_instance(I)
> _Internal Use:_
> Return and branch to a State instance

| Type      | Value |
|-----------|:-----:|
| Statement | None  |
---
| Parameter | Notes                    |
|:---------:|--------------------------|
| I         | State Instance Pointer   |

### branch_to(S)
> Return and branch to a the well-known instance of a State

| Type      | Value |
|-----------|:-----:|
| Statement | None  |
---
| Parameter | Notes                    |
|:---------:|--------------------------|
| S         | State Name               |


```cpp
Require_State(Application);
Require_State(Wifi);
Require_State(Console);
State(Setup) {
    On_Execute {
        if (returning_from(Console)) branch_to(Wifi);
        if (returning_from(Wifi)) continue_to(Application);
        branch_to(Console);
    }
};
```

### continue_to_instance(I)
> _Internal Use:_
> Return and switch the current process state to the given State instance

| Type      | Value |
|-----------|:-----:|
| Statement | None  |
---
| Parameter | Notes                    |
|:---------:|--------------------------|
| I         | State Instance Pointer   |

### continue_to(N)
> Return and switch the current process state to a well-known instance of the given State

| Type      | Value |
|-----------|:-----:|
| Statement | None  |
---
| Parameter | Notes                    |
|:---------:|--------------------------|
| S         | State Name               |


### returning
> Returns true when the current state has been re-activated following a branch

| Type      | Value |
|-----------|:-----:|
| Value     | bool  |
---

### returning_from_instance(I)
> `Internal`
> Returns true when the current state has been re-activated from a Branch matching the State instance

| Type      | Value |
|-----------|:-----:|
| Statement | None  |
---
| Parameter | Notes                    |
|:---------:|--------------------------|
| S         | State Name               |

### returning_from(S)
Returns true when the current state has been re-activated from a Branch matching the the well-known instance of the given State

| Type      | Value |
|-----------|:-----:|
| Function  | bool  |
---
| Parameter | Notes                    |
|:---------:|--------------------------|
| S         | State Name               |

## tinyDFA Class Basics

### Context
> An object provided by a Process evaluating a State. A Context informs the State of the execution context, and informs the Process as to the next action to take.
---
```cpp
class tiny::DFA::Context {
    State * next;
    State * branch;
    bool branched;
    Count evaluation;
    Time execute_time;
    Time delay;
    Time ElapsedTime(bool);
    bool Idle(Time);
};
```
| Context &ast; context | Process Use | State Use |
|---|---|---|
| context->next | when set **Change** to the given state | None. |
| context->branch | when set **Branch** to the given state when branched is false | the branch returned from when branched is true |
| context->branched | when set, clear the branch value and this value | indicates when a branch has finished executing |
| context->evaluation | Incremented per **Execution**, reset on **Change** | The number of times that this state has been **Executed** in this Context |
| context->execute_time | Incremented by the amount of time spent **Executing**, reset on **Change**  | The value actual time spent **Executing** this state in Time Units |
| context->delay | Skip Execution for a given amount of Time Units | Inform the Process to defer performing any actions until the given amount of Time Units have passed since the last action |
| context->Idle(Time) | Decrements delay by the given Time Units, returns true if delay is non-zero | None. |
| context->ElapsedTime(bool) | Used to determine timekeeping offsets | Used to determine how long the current Execution has taken |

### State
```cpp
class tiny::DFA::State {
    template <class T> static State<T> * Named();
    template <class T> static bool Is(State *);
};
```
| State:: | Usage |
|---|---|
| Named&lt;T&gt;() | Get a well-known State pointer for the type T |
| Is&lt;T&gt;(State *) | Is the provided State pointer equal to the well-known State pointer for the given type T |

### Driver
```cpp
template <typename T> class tiny::DFA::Driver : public State {
    T * resource;
    bool Ready();
};
```
| Device&lt;T&gt; &ast; device | Usage |
|---|---|
| resource | a pointer of type T |
| Ready() | Is the pointer at resource ready for use? |

### Process
```cpp
class tiny::DFA::Process {
    Process(State *);
    bool Execute();
    template <typename T> static Process * Using();
};
```
| Process:: | Usage |
|---|---|
| Process(State *) | Create a new process starting with the given State pointer |
| Using<T>() | Returns a new process using the well-known instance of a State |
---
| Process * process | Usage |
|---|---|
| Execute() | Execute one action on the current State, returns false when there are no more actions to perform. |

### Error
> A State that represents an unrecoverable error

```cpp
class tiny::DFA::Error : public State {
    void Enter();
};
```

### Stop
> A State that indicates Process completion

```cpp
class tiny::DFA::Stop : public State {

};
```

### Console
> A simple device to illustrate the concept

```cpp
class tiny::DFA::Console : public Device<Stream> {
    Stream * resource;
    bool Ready();
}
```

## Configuration
### OMIT Flags
- #OMIT_tinyDFA_extended_debug  
  Removes debugging code from the final executable

### Debug and Console
- #tinyDFA_SERIAL_DEVICE  
  > Default Value: **Serial**  
  
  Configures the default serial device for debug helpers and Console Device

- #tinyDFA_SERIAL_DELAY_MS  
  > Default Value: **100**  
  
  How long to wait for the serial port to settle
- #tinyDFA_SERIAL_SPEED  
  > Default Value: **115200**  

  Default serial communication speed

### Timekeeping
- #tinyDFA_TU_MILLIS  
  > Default Value: **1000**  
  
  Number of system Time Units in 1 Millisecond

- #tinyDFA_TU_MAX  
  > Default Value: **4294967290**  
  
  Maximum number of TU before roll over

- #tinyDFA_TU_SECOND  
  > Default Value: **1000000**
  
  Number of TU in 1 Second