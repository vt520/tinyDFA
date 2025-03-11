#ifndef _tinyDFA_debug_h
#define _tinyDFA_debug_h
#include <Arduino.h>
#include "config.h"
namespace tiny {
    namespace DFA {
        namespace Text {
            constexpr char * marker = "***";
            constexpr char * halt = "HALT";
            constexpr char * message = "MESSAGE";
            constexpr char space = ' ';
            constexpr char eol = '\n';
        }
        bool serial_device_active = false;
        
        template <class ...Types> void debug(Types&& ...values) {
            if (!serial_device_active) {
                tinyDFA_SERIAL_DEVICE.begin(tinyDFA_SERIAL_SPEED);
                serial_device_active = true;
            } 
	        (tinyDFA_SERIAL_DEVICE.print(values), ...);
            tinyDFA_SERIAL_DEVICE.flush();
        }

        template <class ...Types> void debugln(Types&& ...inputs) {
            debug(inputs...);
            tinyDFA_SERIAL_DEVICE.println();
        }
    }
}

#define halt(S) { \
    tiny::DFA::debugln( \
        tiny::DFA::Text::marker, \
        tiny::DFA::Text::space, \
        tiny::DFA::Text::halt, \
        tiny::DFA::Text::space, \
        tiny::DFA::Text::marker, \
        tiny::DFA::Text::eol, \
        __PRETTY_FUNCTION__, \
        tiny::DFA::Text::eol, \
        tiny::DFA::Text::message, \
        F(": "), \
        S \
    ); \
    while(true); \
}

#ifdef OMIT_tinyDFA_extended_debug
    #define extended_debug_emit_location
    #define extended_debug_literal(S)
    #define extended_debug_variables(S)
#else
    namespace tiny {
        namespace DFA {
            namespace Text {
                constexpr char * f_start = "<";
                constexpr char * f_end = ">";
                constexpr char * l_start = "[";
                constexpr char * l_end = "]";
            }
        }
    }

    #define extended_debug_signature { \
        debug( \
            tiny::DFA::Text::space, \
            tiny::DFA::Text::f_start, \
            __PRETTY_FUNCTION__, \
            tiny::DFA::Text::f_end \
        ); \
    }
    
    #define extended_debug_variables(...) { \
        extended_debug_signature; \
        tiny::DFA::debug(__VA_ARGS__); \
    }

    #define extended_debug_literal(S) { \
        extended_debug_variables(F(S)); \
    }
            
    #define extended_debug_emit_location { \
        debug( \
            tiny::DFA::Text::l_start, \
            __LINE__, \
            tiny::DFA::Text::l_end \
        ); \
        extended_debug_signature; \
    }
#endif

#endif