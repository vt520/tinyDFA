/**
 * tinyDFA - A Beginner Friendly Discrete Finite Automata (DFA) Scaffold
 * @author Mark C. Roduner Jr. // vt520
 */

 #define OMIT_tinyDFA_extended_debug

 #include "tinyDFA.h"
 
 using namespace tiny::DFA;
 
 Process * broadcast; // define a process
 
 State(OffAir) {
   On_Enter {
     // Create a 30 second air gap before Executing
     context->delay = millis_time(30000);
   }
   On_Execute {
     // do callsign here
 
     // don't execute again for 10 minutes
     context->delay = millis_time(10 * 60 * 1000);
   }
 };
 
 State(OnAir) {
   On_Execute {
     // housekeeping for the broadcast
     bool stillBroadcasting = true; // let's pretend this marks the end of broadcast
     if (!stillBroadcasting) continue_to(OffAir);
     // don't execute again for 1 second
     context->delay = millis_time(1000);
   }
 };
 
 State(Application) {
   On_Execute {
     if (0 == context->evaluation % 6) {
       // Every 6 executions, start broadcasting
       broadcast->Select(State::Named<OnAir>());
     }
     context->delay = millis_time(10000); // delay for 10 second
   }
 };
 
 void setup() {
   process = Process::Using<Application>();
   broadcast = Process::Using<OffAir>();
 }
 
 void loop() {
   process->Execute();
 