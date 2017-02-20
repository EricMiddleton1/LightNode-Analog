#include "StripAnalog.hpp"

#include <stdexcept>
#include <iostream>
#include <cmath>
#include <bcm2835.h>

StripAnalog::StripAnalog(size_t _ledCount, double _gamma)
	:	LightStrip((_ledCount < MAX_LED_COUNT) ? _ledCount : MAX_LED_COUNT) {
	
	for(unsigned int i = 0; i < gammaTable.size(); ++i) {
		gammaTable[i] = (PWM_RESOLUTION) * std::pow(i / 255., _gamma) + 0.5;
	}

	if(!bcm2835_init()) {
		throw std::runtime_error("StripAnalog::StripAnalog: "
			"Failed to initialize bcm2835 library (Are you root?)");
	}
	if(!bcm2835_i2c_begin()) {
		throw std::runtime_error("StripAnalog::StripAnalog: "
			"Failed to initialize I2C module");
	}

	bcm2835_i2c_setSlaveAddress(I2C_ADDR);

	std::vector<char> buffer;

	//Set PWM frequency to max
	buffer = {0xFE, PRESCALAR};
	if(bcm2835_i2c_write(buffer.data(), buffer.size()) != 0) {
		throw std::runtime_error("AnalogStrip::AnalogStrip: "
			"Error setting prescalar");
	}

	//Initialize PCA9685 for AI and OUTDRV mode
	buffer = {0x00, 1 << AI_MODE, 1 << OUTDRV_MODE};
	if(bcm2835_i2c_write(buffer.data(), buffer.size()) != 0) {
		throw std::runtime_error("AnalogStrip::AnalogStrip: "
			"Error configuring PCA9585");
	}

	//Clear LEDs
	update();
}

void StripAnalog::update() {
	std::unique_lock<std::mutex> ledLock(ledMutex);

	std::vector<char> buffer;

	buffer.push_back(REGISTER_LED_START);

	for(auto& color : leds) {
		std::vector<char> values({color.getRed(), color.getGreen(),
			color.getBlue()});

		for(auto& value : values) {
			uint16_t gammaVal = gammaTable[value];
			buffer.push_back(0);
			buffer.push_back(0);
			buffer.push_back(gammaVal & 0xFF);
			buffer.push_back(gammaVal >> 8);
		}
	}

	if(bcm2835_i2c_write(buffer.data(), buffer.size()) != BCM2835_I2C_REASON_OK) {
		std::cout << "[Error] StripAnalog::update: Error sending I2C data"
			<< std::endl;
	}
}
