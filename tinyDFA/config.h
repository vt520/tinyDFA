#ifndef _tinyDFA_config_h
#define _tinyDFA_config_h


#ifndef tinyDFA_SERIAL_DEVICE
#define tinyDFA_SERIAL_DEVICE Serial
#endif

#ifndef tinyDFA_SERIAL_DELAY_MS
#define tinyDFA_SERIAL_DELAY_MS 100
#endif

#ifndef tinyDFA_SERIAL_SPEED
#define tinyDFA_SERIAL_SPEED 115200
#endif


#ifndef tinyDFA_TU_MILLIS 
#define tinyDFA_TU_MILLIS 1000
#endif

#ifndef tinyDFA_TU_MAX
#define tinyDFA_TU_MAX 4294967290
#endif

#define tinyDFA_TU_SECOND (1000 * tinyDFA_TU_MILLIS)


#endif
