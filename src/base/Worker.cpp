#include "Worker.h"
#include <iostream>
#include <algorithm>

Worker::Worker(const char* config)
	: Node(config)
	, modeCommandSubscriber_("ModeCommand", "Byte", new BytePubSubType())
	, statusPublisher_("WorkerStatus", "Status", new StatusPubSubType())
	, standingBy_(false)
{}

void Worker::configNode(tinyxml2::XMLElement* root) {
	tinyxml2::XMLElement* name = root->FirstChildElement("Name");

	nodeName_ = std::string(name->GetText());

	configWorker(root);
}

void Worker::initNode() {
	modeCommandSubscriber_.init();
	statusPublisher_.init();

	initWorker();
}

bool Worker::runNode() {
	Byte modeCommand;
	Status status;

	status.nodeName() = nodeName_;

	// check for messages
	if (modeCommandSubscriber_.getNumMessages() > 0) {
		modeCommand = modeCommandSubscriber_.popOldestMessage();
		// standby
		if (modeCommand.data() == DataTypes::ModeTypes::STANDBY) {
			if (!standingBy_) {
				status.data() = DataTypes::StatusTypes::READY;
				statusPublisher_.publish(status);
				standingBy_ = true;
			}
		}
		// run
		else if (modeCommand.data() == DataTypes::ModeTypes::RUN) {
			standingBy_ = false;
			if (runWorker())
				status.data() = DataTypes::StatusTypes::READY;
			else
				status.data() = DataTypes::StatusTypes::DOWN;
		}
		// shutdown
		else if (modeCommand.data() == DataTypes::ModeTypes::SHUTDOWN)
			status.data() = DataTypes::StatusTypes::DOWN;

		// publish status
		if (!standingBy_)
			statusPublisher_.publish(status);

		// if status is DOWN, return
		if (status.data() == DataTypes::StatusTypes::DOWN)
			return false ;
	}	
	return true;
}
