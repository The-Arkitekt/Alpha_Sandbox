#include <atomic>
class Node {
public:
	bool virtual init()    = 0;
	bool virtual standby() = 0;
	bool virtual run()     = 0;

protected:
	Node();
	int getNodeID();

	bool initialized_;

private:
	static std::atomic_int _counter_;
	std::atomic_int nodeID_;
};