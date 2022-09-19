#ifndef DATATYPES_H
#define DATATYPES_H

#include "stdint.h"


class DataTypes {
public:

	const static uint8_t MAX_NODE_NAME_SIZE = 25;

	enum ModeTypes {STANDBY, RUN, SHUTDOWN};
	static const char* modeTypesToString(uint8_t& mode) {
		switch (mode) {
		case(STANDBY):
			return "STANDBY";
			break;
		case(RUN):
			return "RUN";
			break;
		case(SHUTDOWN):
			return "SHUTDOWN";
			break;
		default:
			return "INVALID";
		}
	}

	enum StatusTypes {UNKNOWN, READY, DOWN};
	static const char* statusTypesToString(uint8_t& status) {
		switch (status) {
		case(UNKNOWN):
			return "UNKNOWN";
			break;
		case(READY):
			return "READY";
			break;
		case(DOWN):
			return "DOWN";
		default:
			return "INVALID";
		}
	}



};

#endif // DATATYPES_H