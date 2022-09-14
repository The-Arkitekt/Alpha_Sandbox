#include "Worker.h"
#include <iostream>


Worker::Worker()
	: modeCommandSubscriber_(new Subscriber<Byte, BytePubSubType>("ModeCommand", "Byte", new BytePubSubType()))
	, statusPublisher_(new Publisher<Byte, BytePubSubType>("Status", "Byte", new BytePubSubType()))
{
	modeCommandSubscriber_->init();
	statusPublisher_->init();
}

Worker::~Worker() {
	delete(modeCommandSubscriber_);
	delete(statusPublisher_);
}

void Worker::moveToStandby() {
	// checking for ModeControl messages
	while (true) {
		while (modeCommandSubscriber_->getNumMessages() > 0) {
			Byte received = modeCommandSubscriber_->popOldestMessage();
			if (received.data() == DataTypes::ModeTypes::STANDBY) {
				Byte status;
				status.data() = DataTypes::StatusTypes::INITIALIZED;
				statusPublisher_->publish(status);
				return;
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

bool Worker::standby() {
	while (true) {
		while (modeCommandSubscriber_->getNumMessages() > 0) {
			Byte received = modeCommandSubscriber_->popOldestMessage();
			if (received.data() == DataTypes::ModeTypes::RUN)
				return true;

			else if (received.data() == DataTypes::ModeTypes::SHUTDOWN)
				return false;

			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}
	}
}

void Worker::runThread() {
	init();

	moveToStandby();

	if (!standby()) {
		Byte statusMsg;
		statusMsg.data(DataTypes::StatusTypes::DOWN);
		statusPublisher_->publish(statusMsg);
	}
	else if (initialized_) 
		run();

	cleanup();
}
