#ifndef _tinyDFA_structure_process_h
#define _tinyDFA_structure_process_h
#include "../Process.h"
using namespace tiny::DFA;

#define _tinyDFA_structure_process_Select(A,B,SEL,...) SEL
#define _tinyDFA_structure_process_Process_1(P) Process * P = Process::Using<tiny::DFA::Stop>()
#define _tinyDFA_structure_process_Process_2(P,S) Process * P = Process::Using<S>()
#define _tinyDFA_structure_process_Process_C(...) _tinyDFA_structure_process_Select(__VA_ARGS__, _tinyDFA_structure_process_Process_2, _tinyDFA_structure_process_Process_1)

/// @brief Define a Process for Use
#define Process(...) _tinyDFA_structure_process_Process_C(__VA_ARGS__)(__VA_ARGS__)

Process(Bar);
void foo() {
    Bar->Execute();
    Process(Foo);
}
#endif