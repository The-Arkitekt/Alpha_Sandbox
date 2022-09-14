#include <iostream>
#include <thread>
#include <future>

#include "nodes/ModeController/ModeController.h"
#include "nodes/TextMessageNode/TextMessageNode.h"

int main(int argc, char** argv) {

	ModeController mc;
	TextMessageNode tmn_1;

	std::thread mt(&ModeController::runThread, mc);
	std::thread tmt_1(&TextMessageNode::runThread, tmn_1);

	mt.join();
	tmt_1.join();

	return 0;

}