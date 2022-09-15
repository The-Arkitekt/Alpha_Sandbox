#ifndef MODECONTROLLER_H
#define MODECONTROLLER_H

#include <map>

#include "../../common/DataTypes.h"
#include "../../base/Node.h"
#include "../../base/Publisher.h"
#include "../../base/Subscriber.h"
#include "../../../messages/Byte/BytePubSubTypes.h"
#include "../../../messages/Status/StatusPubSubTypes.h"

class ModeController : public Node {
public:
	ModeController();

private:
	void init();
	bool run();

	std::map<std::string, uint8_t> workerStatuses_;

	Publisher<Byte, BytePubSubType> modeCommandPublisher_;
	Subscriber<Status, StatusPubSubType> statusSubscriber_;
};

#endif //MODECONTROLLER_H