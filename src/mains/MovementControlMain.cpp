#include "../nodes/MovementGenerator/MovementGenerator.h"
#include "../nodes/MecanumMotorController/MecanumMotorController.h"
#include "../nodes/ModeController/ModeController.h"

#include <thread>

int main(int argc, char** argv) {

	ModeController* mc = new ModeController();
	MovementGenerator* mg = new MovementGenerator();
	MecanumMotorController* mmc = new MecanumMotorController();

	auto mct  = std::thread([&] { mc->runThread(); });
	auto mgt  = std::thread([&] { mg->runThread(); });
	auto mmct = std::thread([&] { mmc->runThread(); });

	mgt.join();
	delete(mg);
	mmct.join();
	delete(mmc);
	mct.join();
	delete(mc);

	return 0;
}