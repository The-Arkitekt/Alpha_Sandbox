#ifndef WORKER_H
#define WORKER_H

#include "Node.h"
#include "Publisher.h"
#include "Subscriber.h"
#include "../../messages/Byte/BytePubSubTypes.h"
#include "../common/DataTypes.h"


class Worker : public Node {
public:
	Worker();
	void runThread();

protected:
	Subscriber<Byte, BytePubSubType>  modeCommandSubscriber_;
	Publisher<Byte, BytePubSubType> statusPublisher_;

private:
	bool standby();
	bool initWorker();
};

#endif // WORKER_H
