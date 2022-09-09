class Node {
public:

	bool virtual init()   = 0;
	bool virtual run()    = 0;

protected:
	bool _initialized = false;
};