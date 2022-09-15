#ifndef MOVECOMMANDHELPER_H
#define MOVECOMMANDHELPER_H

#include "../../../base/Node.h"
#include "../../../base/Publisher.h"
#include "../../../base/Subscriber.h"
#include "../../../../messages/TextMessage/TextMessagePubSubTypes.h"

class MoveCommandHelper : public Node {
public:
	MoveCommandHelper();

private:
	void init();
	bool run();

	Publisher<TextMessage, TextMessagePubSubType>moveCommandPublisher_;
};

#endif //MOVECOMMANDHELPER_H