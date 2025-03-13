# tiny::DFA::Process

## Usage

## Properties

## Methods
### _Using_
> `Process * Process::Using<State>()`

### _Named_ * 
> `Process * Process::Named<Process>()`

### _Register_ *
> `bool Process::Register<Process>(Process *)`

### Active
> `State * Active()`

### Immediate
> `bool Immediate()`

### Execute
> `bool Execute()`

### Select
> `bool Select(State *)` 

### Selected
> `State * Selected()`

## Construction
> `Process::Using<State>()`
> `Process * process = Process(State::Named<State>)`
> [`CreateProcess(Name)`](../keywords/process.md) *
> [`CreateProcess(Name, State)`](../keywords/process.md) *