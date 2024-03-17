#pragma once

//
// This header facilitates building Arduino code
// on non-Arduino platform.
//

#include <stdint.h>

#include "xgpio.h"
#include "xspi.h"
#include "main.h"

#ifndef NULL
#define NULL nullptr
#endif

#ifndef PROGMEM
#define PROGMEM

static inline uint8_t pgm_read_byte(uint8_t const* ptr)
{
	return *ptr;
}

static inline uint16_t pgm_read_word(uint16_t const* ptr)
{
	return *ptr;
}

#endif

static inline void delay(uint32_t msec)
{
	HAL_Delay(msec);
}
