#ifndef MOTOR_HEAD_PLATFORM_CONFIG_H_
#define MOTOR_HEAD_PLATFORM_CONFIG_H_

#ifdef ARDUINO
    #include "portability/arduino/platform.h"
#elif defined(__linux) && !defined(DOXYGEN_FORCED)
    #include "portability/linux/platform.h"
#else
    #include "portability/template/platform.h"
#endif

#endif // MOTOR_HEAD_PLATFORM_CONFIG_H_
