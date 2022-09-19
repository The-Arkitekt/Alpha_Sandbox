#include "ModeController.h"
#include "../../common/xmlParser/tinyxml2/tinyxml2.h"
#include <iostream>
#include <atomic>

ModeController::ModeController()
	: Node("../../../config/ModeController.xml")
	, modeCommandPublisher_("ModeCommand", "Byte", new BytePubSubType())
	, statusSubscriber_("WorkerStatus", "Status", new StatusPubSubType())
{}

void ModeController::configNode(tinyxml2::XMLElement* root) {
	tinyxml2::XMLElement* worker = root->FirstChildElement("Worker");
	while (worker != nullptr) {
		tinyxml2::XMLElement* name = worker->FirstChildElement("Name");
		workerStatuses_[name->GetText()] = DataTypes::StatusTypes::UNKNOWN;
		// get next node
		worker = worker->NextSiblingElement();
	}
}

void ModeController::initNode() {
	modeCommandPublisher_.init();
	statusSubscriber_.init();
}

bool ModeController::runNode() {
	Byte modeCommand;
	
	// get oldest status message
	if (statusSubscriber_.getNumMessages() > 0) {
		Status status = statusSubscriber_.popOldestMessage();
		// if status from unknown worker, shutdown
		if (workerStatuses_.find(status.nodeName()) == workerStatuses_.end()) {
			std::cout << "Received status from invalid worker: " << status.nodeName() << std::endl;
			modeCommand.data() = DataTypes::ModeTypes::SHUTDOWN;
			modeCommandPublisher_.publish(modeCommand);
			return false;
		}
		workerStatuses_[status.nodeName()] = status.data();
	}
	// iterate over all workers
	for (std::map<std::string, uint8_t>::iterator it = workerStatuses_.begin(); it != workerStatuses_.end(); ++it) {
		// if not all workers are subscribed to mode control topic or any node reports DOWN, reset
		if (it->second == DataTypes::StatusTypes::DOWN || it->second == DataTypes::StatusTypes::UNKNOWN) {
			modeCommand.data() = DataTypes::ModeTypes::STANDBY;
			modeCommandPublisher_.publish(modeCommand);
			return true;
		}
	}

	// RUN if all workers are initialized and UP
	modeCommand.data() = DataTypes::ModeTypes::RUN;
	modeCommandPublisher_.publish(modeCommand);

	return true; 
}