#include "ModeController.h"
#include <iostream>

ModeController::ModeController()
	: numNodes_(1)
	, modeCommandPublisher_(nullptr)
	, statusSubscriber_(nullptr)
{}

ModeController::~ModeController() {
	cleanup();
}
void ModeController::runThread() {
	if (!init())
		return;

	run();
	cleanup();
}


bool ModeController::init() {
	if (initialized_)
		return false;

	modeCommandPublisher_ = new Publisher<Byte, BytePubSubType>("ModeCommand", "Byte", new BytePubSubType());
	statusSubscriber_ = new Subscriber<Byte, BytePubSubType>("Status", "Byte", new BytePubSubType());

	modeCommandPublisher_->init();
	statusSubscriber_->init();

	initialized_ = true;
	return true;
}

bool ModeController::run() {
	if (!initialized_)
		return false;

	Byte modeCommand;
	uint16_t numWorkersInitialized = 0;
	bool workersStarted = false;
	while (true) {
		std::cout << "Num Subscribers: " << modeCommandPublisher_->getNumSubscribers() << std::endl;
		if (!workersStarted && modeCommandPublisher_->getNumSubscribers() == numNodes_) {
			modeCommand.data() = DataTypes::ModeTypes::STANDBY;
			modeCommandPublisher_->publish(modeCommand);

			// check for initialized status messages
			while (statusSubscriber_->getNumMessages() > 0) {
				Byte received = statusSubscriber_->popOldestMessage();
				if (received.data() == DataTypes::StatusTypes::INITIALIZED) {
					numWorkersInitialized++;
				}
			}
			if (numWorkersInitialized == numNodes_) {
				modeCommand.data() = DataTypes::ModeTypes::RUN;
				modeCommandPublisher_->publish(modeCommand);
				workersStarted = true;
			}
		}
		else if (workersStarted) {
			numWorkersInitialized = modeCommandPublisher_->getNumSubscribers();
			if (numWorkersInitialized < numNodes_) {
				modeCommand.data() = DataTypes::ModeTypes::SHUTDOWN;
				modeCommandPublisher_->publish(modeCommand);
				return true;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

void ModeController::cleanup() {
	if (initialized_) {
		delete(modeCommandPublisher_);
		delete(statusSubscriber_);
	}
	initialized_ = false;
}