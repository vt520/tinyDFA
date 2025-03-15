/**
 * tinyDFA - A Beginner Friendly Discrete Finite Automata (DFA) Scaffold
 * @author Mark C. Roduner Jr. // vt520
 */

#define OMIT_tinyDFA_extended_debug

#include "tinyDFA.h"

#define END_OF_BRANCH_MSG F("End of branch")
#define COUNT_COMPLETE_MESSAGE F("Count Complete")

#define TEST_DELAY_MS 1000

using namespace tiny::DFA;

Require_State(Branching);
State(Counter) {
  On_Execute {
    context->delay = millis_time(TEST_DELAY_MS);
    console->println(context->evaluation);
    if (context->evaluation == 10) {
      bool * content = static_cast<bool *>(context->content);
      if (content) {
        *content = true;
      }

      continue_to(Branching);
    }
  }
};

Require_State(Stop);
State(BranchEnd) {
  On_Enter {
    context->delay = millis_time(TEST_DELAY_MS);
    console->println(F("End of Branch"));
    continue_to(tiny::DFA::Stop);
  }
};

State(BranchOne) {
  On_Execute {
    context->delay = millis_time(TEST_DELAY_MS);
    continue_to(BranchEnd);
  }
};

State(BranchTwo) {
  On_Execute {
    context->delay = millis_time(TEST_DELAY_MS);
    continue_to(BranchEnd);
  }
};

Require_State(Counter);
Require_State(BranchOne);
State(Branching) {
  On_Execute {
    context->delay = millis_time(TEST_DELAY_MS);
    if(context->branched) {
      if (State::Named<BranchOne>() == context->branch) branch_to(BranchTwo);
      if (State::Named<BranchTwo>() == context->branch) continue_to(Counter);
      continue_to(tiny::DFA::Error);
    } else {
      branch_to(BranchOne);
    }
  }
};

State(Setup) {
  On_Execute {
    if (context->branched) {
      if (State::Is<Console>(context->branch)) {
        bool * content = static_cast<bool *>(context->content);
        if (content) {
          *content = true;
        }
        continue_to(Counter);
      } 
    }
    branch_to(Console);
  }
};

bool flag = false;
void setup() {
  //console->available();
  process = Process::Using<Setup>(&flag);
}

void loop() {
    if (flag) {
      Serial.println(flag);
      flag = false;
    }
    if(!process->Execute()) halt("Process Stopped");
}