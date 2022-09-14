#ifndef NODE_H
#define NODE_H

class Node {
protected:
	void virtual runThread() = 0;
	bool virtual init()		 = 0;
	bool virtual run()		 = 0;
	void virtual cleanup()   = 0;

	bool initialized_ = false;
};

#endif // NODE_H