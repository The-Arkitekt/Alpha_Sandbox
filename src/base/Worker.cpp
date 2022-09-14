#include "Worker.h"
#include <iostream>
#include <algorithm>

Worker::Worker()
	: modeCommandSubscriber_("ModeCommand", "Byte", new BytePubSubType())
	, statusPublisher_("Status", "Byte", new BytePubSubType())
{}
	

bool Worker::initWorker() {
	modeCommandSubscriber_.init();
	statusPublisher_.init();
	return true;
}

bool Worker::standby() {

	while (true) {
		while (modeCommandSubscriber_.getNumMessages() > 0) {
			Byte received = modeCommandSubscriber_.popOldestMessage();
			if (received.data() == DataTypes::ModeTypes::RUN)
				return true;

			else if (received.data() == DataTypes::ModeTypes::SHUTDOWN)
				return false;

			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
}

void Worker::runThread() {
	if (!initWorker())
		return;

	if (!init())
		return;

	bool modeControllerReady = false;
	while (!modeControllerReady) {
		while (modeCommandSubscriber_.getNumMessages() > 0) {
			Byte received = modeCommandSubscriber_.popOldestMessage();
			if (received.data() == DataTypes::ModeTypes::STANDBY) {
				modeControllerReady = true;
				Byte status;
				status.data() = DataTypes::StatusTypes::INITIALIZED;
				statusPublisher_.publish(status);
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}

	if (!standby()) {
		Byte statusMsg;
		statusMsg.data(DataTypes::StatusTypes::DOWN);
		statusPublisher_.publish(statusMsg);
		return;
	}

	if (!run())
		return;
}
