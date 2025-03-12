#ifndef _tinyDFA_keywords_declarations_h
#define _tinyDFA_keywords_declarations_h

/// @brief Declare a Device
/// @param S the State name of the Device
/// @param T the Resource type of the Device 
#define Device(S,T) class S: public Device<T>

/// @brief Declare the Ready method of a Device
/// @returns bool if resource is in a ready state 
#define Is_Ready bool Ready()


/// @brief Declare a Device that realizes an incomplete device
/// @param S the State name of the Device
/// @param C the State name of the Device to implement
#define DeviceInstance(S,C) class S : public C

#endif