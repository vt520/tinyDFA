#ifndef _tinyDFA_keywords_device_h
#define _tinyDFA_keywords_device_h
#include "Device.h"

/// @brief Obtain a Device instance for a named device
/// @param D the name of the Device
/// @param T the resource type of the device
/// @return Device<T> *
#define device(D,T)  Device<T>::Named<D>()

/// @brief Obtain the resource held by a device
/// @param D the name of the Device
/// @param T the resource type held by the device
/// @return T *
#define resource(D,T)  device(D,T)->resource

#endif