#include "ModeController.h"
#include <iostream>
#include <atomic>

ModeController::ModeController()
	: numNodes_(1)
	, modeCommandPublisher_("ModeCommand", "Byte", new BytePubSubType())
	, statusSubscriber_("Status", "Byte", new BytePubSubType())
{}

void ModeController::runThread() {
	if (!init())
		return;

	run();
}


bool ModeController::init() {
	modeCommandPublisher_.init();
	statusSubscriber_.init();

	return true;
}

bool ModeController::run() {
	Byte modeCommand;
	std::atomic_int numWorkersInitialized{ 0 };
	std::atomic_bool workersStarted{ false };
	while (true) {
		if (!workersStarted && modeCommandPublisher_.getNumSubscribers() == numNodes_) {
			modeCommand.data() = DataTypes::ModeTypes::STANDBY;
			modeCommandPublisher_.publish(modeCommand);

			// check for initialized status messages
			while (statusSubscriber_.getNumMessages() > 0) {
				Byte received = statusSubscriber_.popOldestMessage();
				if (received.data() == DataTypes::StatusTypes::INITIALIZED) {
					numWorkersInitialized++;
				}
			}
			if (numWorkersInitialized == numNodes_) {
				modeCommand.data() = DataTypes::ModeTypes::RUN;
				modeCommandPublisher_.publish(modeCommand);
				workersStarted = true;
			}
		}
		else if (workersStarted) {
			numWorkersInitialized = modeCommandPublisher_.getNumSubscribers();
			if (numWorkersInitialized < numNodes_) {
				modeCommand.data() = DataTypes::ModeTypes::SHUTDOWN;
				modeCommandPublisher_.publish(modeCommand);
				return true;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}