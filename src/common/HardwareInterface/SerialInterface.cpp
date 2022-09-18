#include "SerialInterface.h"
#include "../xmlParser/tinyxml2/tinyxml2.h"

SerialInterface::SerialInterface(const char* config) {
	tinyxml2::XMLDocument doc;
	doc.LoadFile("../../../config/HardwareInterface.xml");
	tinyxml2::XMLElement* root = doc.RootElement();
	tinyxml2::XMLElement* serial = root->FirstChildElement("SerialInterface");
	tinyxml2::XMLElement* configuration = serial->FirstChildElement(config);

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

bool SerialInterface::writeData(uint8_t* data) {

	// open serial port
	int serial_port = open(settings_.device, O_RDWR);

	// create new termios struct
	struct termios tty;

	// Read in existing settings, and handle any error
	if (tcgetattr(serial_port, &tty) != 0) {
		printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
		return false;
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
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);											// Turn off s/w flow ctrl
	tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL);	// Disable any special handling of received bytes

	tty.c_oflag &= ~OPOST;															// Prevent special interpretation of output bytes (e.g. newline chars)
	tty.c_oflag &= ~ONLCR;															// Prevent conversion of newline to carriage return/line feed

	// Set out baud rate 
	cfsetospeed(&tty,settings_.baud);

	// Save tty settings, also checking for error
	if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
		return false;
	}

	// Write to serial port
	unsigned char msg[] = { 'H', 'e', 'l', 'l', 'o', '\r' };
	write(serial_port, msg, sizeof(msg));


	// Allocate memory for read buffer, set size according to your needs
	char read_buf[256];

	// Normally you wouldn't do this memset() call, but since we will just receive
	// ASCII data for this example, we'll set everything to 0 so we can
	// call printf() easily.
	memset(&read_buf, '\0', sizeof(read_buf));

	// Read bytes. The behaviour of read() (e.g. does it block?,
	// how long does it block for?) depends on the configuration
	// settings above, specifically VMIN and VTIME
	int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

	// n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
	if (num_bytes < 0) {
		printf("Error reading: %s", strerror(errno));
		return false;
	}

	// Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and
	// print it to the screen like this!)
	printf("Read %i bytes. Received message: %s", num_bytes, read_buf);

	close(serial_port);

	return true;
}

bool SerialInterface::readData(uint8_t* buf) {
	return true;
}	

int getBaud(int baud) {
	switch (baud) {
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