#ifndef HARDWAREINTERFACE_H
#define HARDWAREINTERFACE_H

#include <iostream>
#include <string>

#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

class HardwareInterface {

public:
	HardwareInterface(const char*);

	bool writeData(const char*, uint8_t);
	const char* readData();
		
private:
	const char* device_;
	struct ttySettings {
		bool parity;
		bool stopBit;
		bool HwFlow;
		int baud;
	} settings_;

};

#endif // HARDWAREINTERFACE_H