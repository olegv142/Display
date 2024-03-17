//
// Arduino-like GPIO access.
// Facilitates porting Arduino code.
//

#pragma once

#include "main.h"
#include <stdint.h>

class Pin {
public:
	Pin(GPIO_TypeDef* port, uint16_t bit) : m_port(port), m_bit(bit) {}
	bool read() const { return m_port->IDR & m_bit; }
	void write(bool v) { m_port->BSRR = v ? (uint32_t)m_bit : (uint32_t)m_bit << 16; }
private:
	GPIO_TypeDef* const m_port;
	uint16_t      const m_bit;
};

#define HIGH true
#define LOW  false

typedef enum {
	INPUT,
	OUTPUT
} pin_mode_t;

static inline bool digitalRead(Pin const& p) { return p.read(); }
static inline void digitalWrite(Pin& p, bool v) { p.write(v); }
static inline void pinMode(Pin const& p, pin_mode_t mode) {}

#define PORT_PIN(PIN) PIN##_GPIO_Port, PIN##_Pin
