#pragma once

#include "LightNode/LightStrip.hpp"

#include <array>
#include <cstdint>


class StripAnalog : public LightStrip
{
public:
	StripAnalog(size_t ledCount, double gamma);

	virtual void update() override;
private:
	static const uint8_t I2C_ADDR = 0x40;
	static const uint8_t AI_MODE = 5;
	static const uint8_t OUTDRV_MODE = 2;
	static const uint8_t REGISTER_LED_START = 0x06;
	static const uint8_t PRESCALAR = 0x03;
	static const unsigned int PWM_RESOLUTION = 4096;
	static const size_t MAX_LED_COUNT = 5;

	std::array<uint16_t, 256> gammaTable;
};
