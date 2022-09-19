#ifndef NODE_H
#define NODE_H

#include <stdint.h>
#include "Subscriber.h"
#include "Publisher.h"
#include "../common/xmlParser/tinyxml2/tinyxml2.h"

class Node {
public:
	Node(const char*);
	void runThread();

protected:
	void virtual configNode(tinyxml2::XMLElement*) = 0;
	void virtual initNode()					       = 0;
	bool virtual runNode()	                       = 0;

private:
	void config();

	const char* configFile_;
	uint16_t loopDelay_;
};

#endif // NODE_H