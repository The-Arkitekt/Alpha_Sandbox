#include "Node.h"
#include "../common/xmlParser/tinyxml2/tinyxml2.h"
#include <thread>
#include <iostream>

Node::Node(const char* config) {
	tinyxml2::XMLDocument doc;
	doc.LoadFile(config);
	tinyxml2::XMLElement* root = doc.RootElement();
	tinyxml2::XMLElement* delay = root->FirstChildElement("LoopDelay");
	loopDelay = atoi(delay->GetText());
}

void Node::runThread() {
	init();

	while (run()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(loopDelay));
	}
}