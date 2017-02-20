#include <LightNode/LightNode.hpp>
#include "StripAnalog.hpp"

#include <memory>
#include <thread>
#include <chrono>

#define LED_COUNT	4
#define GAMMA	2.4

int main() {
	LightNode lightNode(Communicator::NodeType::ANALOG,
		std::make_shared<StripAnalog>(LED_COUNT, GAMMA));
	
	for(;;) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	return 0;
}
