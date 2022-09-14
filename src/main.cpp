#include <iostream>
#include <thread>

#include "nodes/ModeController/ModeController.h"
#include "nodes/TextMessageNode/TextMessageNode.h"

int main(int argc, char** argv) {

	ModeController* mc = new ModeController();
	TextMessageNode* tmn_1 = new TextMessageNode();

	auto mt = std::thread([&] { mc->runThread(); });
	auto tmt_1 = std::thread([&] {tmn_1->runThread(); });

	tmt_1.join();
	delete(tmn_1);
	mt.join();
	delete(mc);

	return 0;

}