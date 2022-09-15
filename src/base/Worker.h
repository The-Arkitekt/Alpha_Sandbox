#ifndef WORKER_H
#define WORKER_H

#include "Node.h"
#include "Publisher.h"
#include "Subscriber.h"
#include "../../messages/Byte/BytePubSubTypes.h"
#include "../../messages/Status/StatusPubSubTypes.h"
#include "../common/DataTypes.h"


class Worker : public Node {
public:
	Worker(const char*);
	void runThread();

protected:
	Subscriber<Byte, BytePubSubType>  modeCommandSubscriber_;
	Publisher<Status, StatusPubSubType> statusPublisher_;

private:
	void initWorker();

	bool readyToRun;
	const char* nodeName;
};

#endif // WORKER_H
