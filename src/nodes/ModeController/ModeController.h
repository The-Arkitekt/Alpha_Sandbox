#ifndef MODECONTROLLER_H
#define MODECONTROLLER_H

#include <stdint.h>

#include "../../common/DataTypes.h"
#include "../../base/Node.h"
#include "../../base/Publisher.h"
#include "../../base/Subscriber.h"
#include "../../../messages/Byte/BytePubSubTypes.h"

class ModeController : public Node {
public:
	ModeController();
	~ModeController();
	void runThread();

private:
	bool init();
	bool run();
	void cleanup();

	uint16_t numNodes_;
	Publisher<Byte, BytePubSubType>* modeCommandPublisher_;
	Subscriber<Byte, BytePubSubType>* statusSubscriber_;
};

#endif //MODECONTROLLER_H