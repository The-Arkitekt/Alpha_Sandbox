#include "Node.h"
#include <thread>
#include <iostream>

Node::Node(const char* config)
	: configFile_(config)
{
	std::cout << "Node created with config file: " << config << std::endl;
}

void Node::config() {
	tinyxml2::XMLDocument doc;
	doc.LoadFile(configFile_);
	tinyxml2::XMLElement* root = doc.RootElement();

	tinyxml2::XMLElement* delay = root->FirstChildElement("LoopDelay");
	loopDelay_ = atoi(delay->GetText());
	// do Node speific configure
	configNode(root);
}

void Node::runThread() {
	config();
	initNode();

	while (runNode()) {
		std::this_thread::sleep_for(std::chrono::milliseconds(loopDelay_));
	}
}