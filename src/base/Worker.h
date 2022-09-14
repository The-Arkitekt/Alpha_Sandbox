#ifndef WORKER_H
#define WORKER_H

#include "Node.h"
#include "Publisher.h"
#include "Subscriber.h"
#include "../../messages/Byte/BytePubSubTypes.h"
#include "../common/DataTypes.h"


class Worker : public Node {
public:
	void runThread();

protected:
	Worker();
	~Worker();

	Subscriber<Byte, BytePubSubType>* modeCommandSubscriber_;
	Publisher<Byte, BytePubSubType>* statusPublisher_;

private:
	void moveToStandby();
	bool standby();
};

#endif // WORKER_H
