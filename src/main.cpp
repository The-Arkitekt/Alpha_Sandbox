#include <iostream>

#include "TextMessageNode.h"

int main(int argc, char** argv) {

	TextMessageNode* node = new TextMessageNode();

	if (!node->init()) {
		std::cout << "Failed to initialize node" << std::endl;
		return 1;
	}

	if (!node->run()) {
		std::cout << "Failed to run node" << std::endl;
		return 1;
	}

	return 0;

}