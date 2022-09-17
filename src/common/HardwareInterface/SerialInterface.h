#ifndef SERIALINTERFACE_H
#define SERIALINTERFACE_H

#include <iostream>
#include <string>

#include <stdint.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

class SerialInterface {

public:
	SerialInterface(const char*);

	bool writeData(const char*);
	const char* readData();
		
private:
	struct SerialSettings {
		const char* device;
		bool parity;
		bool stopBit;
		bool hwFlow;
		int baud;
	} settings_;
	int getBaud(int baud);

};

#endif // SERIALINTERFACE