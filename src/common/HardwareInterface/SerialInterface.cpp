#include "SerialInterface.h"
#include "../xmlParser/tinyxml2/tinyxml2.h"

SerialInterface::SerialInterface(const char* configName) 
	: serialPort_(-1)
	, configName_(configName)
{}

SerialInterface::~SerialInterface() {
	if (serialPort_ > 0)
		closePort();
}

void SerialInterface::config() {
	tinyxml2::XMLDocument doc;
	doc.LoadFile("../../../config/HardwareInterface.xml");
	tinyxml2::XMLElement* root = doc.RootElement();
	tinyxml2::XMLElement* serial = root->FirstChildElement("SerialInterface");
	tinyxml2::XMLElement* configuration = serial->FirstChildElement(configName_);

	tinyxml2::XMLElement* deviceConfig = configuration->FirstChildElement("Device");
	settings_.device = deviceConfig->GetText();

	tinyxml2::XMLElement* parityConfig = configuration->FirstChildElement("Parity");
	parityConfig->QueryBoolText(&settings_.parity);

	tinyxml2::XMLElement* stopBitConfig = configuration->FirstChildElement("StopBit");
	stopBitConfig->QueryBoolText(&settings_.stopBit);

	tinyxml2::XMLElement* hwFlowConfig = configuration->FirstChildElement("HWFlowControl");
	hwFlowConfig->QueryBoolText(&settings_.hwFlow);

	tinyxml2::XMLElement* baudConfig = configuration->FirstChildElement("Baud");
	baudConfig->QueryIntText(&settings_.baud);
}

int SerialInterface::initPort(int vTime, int vMin) {
	// open serial port
	std::cout << "Device: " << settings_.device << std::endl;
	std::cout << "Parity: " << settings_.parity << std::endl;
	std::cout << "StopBit: " << settings_.stopBit << std::endl;
	std::cout << "HWFlowControl: " << settings_.hwFlow << std::endl;
	std::cout << "Baud: " << settings_.baud << std::endl;

	serialPort_ = open(settings_.device.c_str(), O_RDWR);

	// create new termios struct
	struct termios tty;

	// Read in existing settings, and handle any error
	if (tcgetattr(serialPort_, &tty) != 0) {
		printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
		return -1;
	}

	// configure tty using configuration given
	tty.c_cflag &= ~PARENB;															// set parity
	tty.c_cflag |= CSTOPB;															// set stop bit
	tty.c_cflag &= ~CSIZE;															// Clear all bits that set the data size 
	tty.c_cflag |= CS8;																// 8 bits per byte (most common)
	tty.c_cflag &= (CRTSCTS * settings_.hwFlow);									// Disable RTS/CTS hardware flow control (most common)
	tty.c_cflag |= CREAD | CLOCAL;													// Turn on READ & ignore ctrl lines (CLOCAL = 1)

	tty.c_lflag &= ~ICANON;
	tty.c_lflag &= ~ECHO;															// Disable echo
	tty.c_lflag &= ~ECHOE;															// Disable erasure
	tty.c_lflag &= ~ECHONL;															// Disable new-line echo
	tty.c_lflag &= ~ISIG;															// Disable interpretation of INTR, QUIT and SUSP
	tty.c_lflag &= ~IEXTEN;
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);											// Turn off s/w flow ctrl

	tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);	// Disable any special handling of received bytes

	tty.c_oflag &= ~OPOST;															// Prevent special interpretation of output bytes (e.g. newline chars)
	tty.c_oflag &= ~ONLCR;															// Prevent conversion of newline to carriage return/line feed

	cfmakeraw(&tty);

	tty.c_cc[VTIME] = vTime;    // time to block waiting for read data
	tty.c_cc[VMIN] = vMin;		// minimum number of characters to recieve (blocks until all characters received)

	// Set out baud rate 
	cfsetospeed(&tty, getBaudMacro(settings_.baud));

	// Save tty settings, also checking for error
	if (tcsetattr(serialPort_, TCSANOW, &tty) != 0) {
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
		return -1;
	}

	return serialPort_;
}

bool SerialInterface::writeData(std::vector<int8_t> data) {
	if (serialPort_ < 0)
		return false;

	// Write to serial port
	//write(serialPort_, data.data(), data.size());
	int8_t msg[]{-1};
	write(serialPort_, msg, 1);

	return true;
}

bool SerialInterface::readData(std::vector<int8_t>* readBuf, int numBytesToRead) {
	if (serialPort_ < 0)
		return false;

	// read size number of bytes one byte at a time
	int numBytesTotal = 0;
	int numBytes = 0;
	unsigned char readByte[1]{0};
	while (numBytesTotal < numBytesToRead) {
		numBytes = read(serialPort_, &readByte, 1);

		// n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
		if (numBytes < 0) {
			printf("Error reading: %s", strerror(errno));
			return false;
		} 
		std::cout << "Byte read " << numBytes << ": " << int(readByte[0]) << std::endl;
		readBuf->push_back(*readByte);
		numBytesTotal++;
	}
	return true;
}	

void SerialInterface::closePort() {
	close(serialPort_);
}

int SerialInterface::getBaudMacro(int baudValue) {
	switch (baudValue) {
	case 9600:
		return B9600;
	case 19200:
		return B19200;
	case 38400:
		return B38400;
	case 57600:
		return B57600;
	case 115200:
		return B115200;
	case 230400:
		return B230400;
	case 460800:
		return B460800;
	case 500000:
		return B500000;
	case 576000:
		return B576000;
	case 921600:
		return B921600;
	case 1000000:
		return B1000000;
	case 1152000:
		return B1152000;
	case 1500000:
		return B1500000;
	case 2000000:
		return B2000000;
	case 2500000:
		return B2500000;
	case 3000000:
		return B3000000;
	case 3500000:
		return B3500000;
	case 4000000:
		return B4000000;
	default:
		return 0;
	}
}