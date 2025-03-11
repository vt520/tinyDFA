#ifndef _tinyDFA_Device_h
#define _tinyDFA_Device_h
#include "State.h"
#include "keywords.h"

namespace tiny {
    namespace DFA {
        /// @brief Devices are special State objects that expose a shared resource member
        /// @tparam ResourceType the type of pointer for the resource member
        template <typename ResourceType> class Device : public State {
            public:
            /// @brief Contains the shared object that this Device is responsible for
            inline static ResourceType * resource = nullptr;
            
            /// @brief Returns true if a resource is ready to use
            /// @return true if the resource object is valid
            virtual Is_Ready { 
                return resource;  
            }
            /// @brief Obtains a shared resource from a well-known Device State
            /// @tparam WellKnown the class name of the requested resource
            /// @return resource of ResourceType owned by the well-known Device State
            template <typename WellKnown> static Device<ResourceType> * Named() {
                Device<ResourceType> * requested = (Device<ResourceType>*)State::Named<WellKnown>();
                if (requested->Ready()) return requested;
                halt("Device Not Initialized");
            }
        };
    }
}

#include "Device/Console.h"

#endif