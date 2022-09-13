#include <atomic>
class Node {
public:
	bool virtual init()    = 0;
	bool virtual standby() = 0;
	bool virtual run()     = 0;

protected:
	bool initialized_ = false;
};