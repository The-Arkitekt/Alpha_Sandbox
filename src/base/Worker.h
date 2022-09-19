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

protected:
	void virtual configWorker(tinyxml2::XMLElement*) = 0;
	void virtual initWorker()						 = 0;
	bool virtual runWorker()						 = 0;

	Subscriber<Byte, BytePubSubType>  modeCommandSubscriber_;
	Publisher<Status, StatusPubSubType> statusPublisher_;

private:
	void configNode(tinyxml2::XMLElement*);
	void initNode();
	bool runNode();

	bool standingBy_;
	std::string nodeName_;
};

#endif // WORKER_H
