#include "Worker.h"
#include "../common/xmlParser/tinyxml2/tinyxml2.h"
#include <iostream>
#include <algorithm>

Worker::Worker(const char* config)
	: Node(config)
	, modeCommandSubscriber_("ModeCommand", "Byte", new BytePubSubType())
	, statusPublisher_("WorkerStatus", "Status", new StatusPubSubType())
	, readyToRun(false)
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(config);
	tinyxml2::XMLElement* root = doc.RootElement();
	tinyxml2::XMLElement* name = root->FirstChildElement("Name");
	nodeName = name->GetText();
}
	

void Worker::initWorker() {
	modeCommandSubscriber_.init();
	statusPublisher_.init();
}

void Worker::runThread() {
	initWorker();
	init();

	Byte modeCommand;
	Status status;

	status.nodeName() = nodeName;

	bool standingBy = false;
	while (true) {
		// check for messages
		if (modeCommandSubscriber_.getNumMessages() > 0) {
			modeCommand = modeCommandSubscriber_.popOldestMessage();
			// standby
			if (modeCommand.data() == DataTypes::ModeTypes::STANDBY) {
				if (!standingBy) {
					status.data() = DataTypes::StatusTypes::READY;
					statusPublisher_.publish(status);
					standingBy = true;
				}
			}
			// run
			else if (modeCommand.data() == DataTypes::ModeTypes::RUN) {
				standingBy = false;
				if (run())
					status.data() = DataTypes::StatusTypes::READY;
				else
					status.data() = DataTypes::StatusTypes::DOWN;
			}
			// shutdown
			else if (modeCommand.data() == DataTypes::ModeTypes::SHUTDOWN)
				status.data() = DataTypes::StatusTypes::DOWN;

			// publish status
			if (!standingBy)
				statusPublisher_.publish(status);

			// if status is DOWN, return
			if (status.data() == DataTypes::StatusTypes::DOWN)
				return ;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(loopDelay));
	}	
}
