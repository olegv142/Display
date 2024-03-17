//
// Arduino-like SPI access.
// Facilitates porting Arduino code.
//

#pragma once

#include "main.h"
#include "debug.h"
#include <stdint.h>

class SPISettings {
public:
	SPISettings(SPI_HandleTypeDef* hspi) : m_hspi(hspi) {}
	virtual SPI_HandleTypeDef* apply() const { return m_hspi; }
protected:
	SPI_HandleTypeDef* const m_hspi;
};

typedef enum {
	clk_low_edge1  = 0,
	clk_low_edge2  = SPI_CR1_CPHA,
	clk_high_edge1 = SPI_CR1_CPOL,
	clk_high_edge2 = SPI_CR1_CPOL | SPI_CR1_CPHA,
} spi_clk_mode_t;

class SPIModeSettings : public SPISettings {
public:
	SPIModeSettings(SPI_HandleTypeDef* hspi, spi_clk_mode_t mode) : SPISettings(hspi), m_mode(mode) {}
	virtual SPI_HandleTypeDef* apply() const {
		uint32_t const cpol = m_mode & SPI_CR1_CPOL;
		uint32_t const cpha = m_mode & SPI_CR1_CPHA;
		if (cpol != m_hspi->Init.CLKPolarity || cpha != m_hspi->Init.CLKPhase) {
			HAL_StatusTypeDef rc = HAL_SPI_DeInit(m_hspi);
			BUG_ON(rc != HAL_OK);
			m_hspi->Init.CLKPolarity = cpol;
			m_hspi->Init.CLKPhase    = cpha;
			rc = HAL_SPI_Init(m_hspi);
			BUG_ON(rc != HAL_OK);
		}
		return m_hspi;
	}
private:
	spi_clk_mode_t m_mode;
};

typedef SPISettings& spi_settings_t;

class SPIStub {
public:
	void     begin() {}
	void     beginTransaction(SPISettings const& s) { 
			BUG_ON(m_hspi);
			m_hspi = s.apply(); 
		}
	void     endTransaction() {
			BUG_ON(!m_hspi);
			m_hspi = NULL;
		}
	void    transfer(uint8_t* buff, uint16_t len) {
			BUG_ON(!m_hspi);
			HAL_SPI_TransmitReceive(m_hspi, buff, buff, len, HAL_MAX_DELAY);
		}
	void    transfer(uint8_t const* buff, uint16_t len) {
			BUG_ON(!m_hspi);
			HAL_SPI_Transmit(m_hspi, (uint8_t*)buff, len, HAL_MAX_DELAY);
		}
	uint8_t  transfer(uint8_t v) { transfer(&v, 1); return v; }
	uint16_t transfer16(uint16_t v) {
			uint8_t buff[2] = {(uint8_t)(v >> 8), (uint8_t)v};
			transfer(buff, 2);
			return ((uint16_t)buff[0] << 8) | buff[1];
		}
	uint32_t transfer24(uint32_t v) {
			uint8_t buff[3] = {(uint8_t)(v >> 16), (uint8_t)(v >> 8), (uint8_t)v};
			transfer(buff, 3);
			return ((uint32_t)buff[0] << 16) | ((uint16_t)buff[1] << 8) | buff[2];
		}
private:
	SPI_HandleTypeDef* m_hspi;
};

extern SPIStub SPI;
