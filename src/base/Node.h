#ifndef NODE_H
#define NODE_H

#include <stdint.h>

class Node {
public:
	Node(const char*);
	void runThread();
protected:
	void virtual init()		 = 0;
	bool virtual run()		 = 0;
	uint16_t loopDelay;
};

#endif // NODE_H