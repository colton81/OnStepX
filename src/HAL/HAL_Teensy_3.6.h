// Platform setup ------------------------------------------------------------------------------------
// Teensy3.6
#pragma once

// We define a more generic symbol, in case more Teensy boards based on different lines are supported
#define __TEENSYDUINO__

// This platform has digitalReadFast, digitalWriteFast, etc.
#define HAL_HAS_DIGITAL_FAST
// turn on support for using DACs to output digital signals using Arduino standard commands
#define HAL_DAC_AS_DIGITAL

// 1/2000 second sidereal timer
#define HAL_FRACTIONAL_SEC 2000.0F

// This platform has up to 16 bit PWM
#ifndef ANALOG_WRITE_PWM_BITS
  #define ANALOG_WRITE_PWM_BITS 12
#endif
#ifndef ANALOG_WRITE_PWM_RANGE
  #define ANALOG_WRITE_PWM_RANGE 4095
#endif
#ifndef ANALOG_WRITE_PWM_FREQUENCY
  #define ANALOG_WRITE_PWM_FREQUENCY 14648.437
#endif

// Lower limit (fastest) step rate in uS for this platform (in SQW mode) and width of step pulse
#if F_CPU >= 240000000
  #define HAL_MAXRATE_LOWER_LIMIT 2
  #define HAL_PULSE_WIDTH 200  // in ns, estimated
#elif F_CPU >= 180000000
  #define HAL_MAXRATE_LOWER_LIMIT 2.6
  #define HAL_PULSE_WIDTH 300  // in ns, estimated
#else
  #define HAL_MAXRATE_LOWER_LIMIT 4.8
  #define HAL_PULSE_WIDTH 400  // in ns, estimated
#endif
#define HAL_FAST_PROCESSOR

// New symbols for the Serial ports so they can be remapped if necessary -----------------------------

// SerialA is manidatory
#define SERIAL_A Serial
// SerialB is optional
#if SERIAL_B_BAUD_DEFAULT != OFF
  #define SERIAL_B Serial1
#endif
// SerialC is optional
#if SERIAL_C_BAUD_DEFAULT != OFF
  #define SERIAL_C Serial4
#endif
// SerialD/E are optional
#if defined(USB_DUAL_SERIAL) || defined(USB_TRIPLE_SERIAL)
  #if defined(SERIAL_D_BAUD_DEFAULT) && SERIAL_D_BAUD_DEFAULT != OFF
    #define SERIAL_D SerialUSB1
  #endif
#endif
#if defined(USB_TRIPLE_SERIAL)
  #if defined(SERIAL_E_BAUD_DEFAULT) && SERIAL_E_BAUD_DEFAULT != OFF
    #define SERIAL_E SerialUSB2
  #endif
#endif

// New symbol for the default I2C port -------------------------------------------------------------
#include <Wire.h>
#define HAL_Wire Wire1
#ifndef HAL_WIRE_CLOCK
  #define HAL_WIRE_CLOCK 100000
#endif

// Non-volatile storage ------------------------------------------------------------------------------
#if NV_DRIVER == NV_DEFAULT
  #include "EEPROM.h"
  #include "../lib/nv/NV_EEPROM.h"
  #define HAL_NV_INIT() nv.init(E2END + 1, true, 0, false);
#endif

//--------------------------------------------------------------------------------------------------
// General purpose initialize for HAL
#define HAL_INIT() { \
  analogReadResolution(10); \
  analogWriteResolution(ANALOG_WRITE_PWM_BITS); \
}

#define HAL_RESET() { \
  SCB_AIRCR = 0x05FA0004; \
  asm volatile ("dsb"); \
}

//--------------------------------------------------------------------------------------------------
// Internal MCU temperature (in degrees C)
#define _Tpin 70
#define HAL_TEMP() ( (-((analogRead(_Tpin)/1024.0)*3.3-0.719)/0.001715)+25.0 )

//---------------------------------------------------------------------------------------------------
// Misc. includes to support this processor's operation

// a really short fixed delay
#define HAL_DELAY_25NS() delayNanoseconds(10)
