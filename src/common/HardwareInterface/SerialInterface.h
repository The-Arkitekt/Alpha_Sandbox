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
	~SerialInterface();

	void config();
	int initPort(int, int);
	void closePort();
	bool writeData(uint8_t*);
	bool readData(uint8_t*);
		
private:
	int getBaudMacro(int);

	int serialPort_;
	const char* configName_;
	struct SerialSettings {
		std::string device;
		bool parity;
		bool stopBit;
		bool hwFlow;
		int baud;
	} settings_;
};

#endif // SERIALINTERFACE