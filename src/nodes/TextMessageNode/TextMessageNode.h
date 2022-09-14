#ifndef TEXTMESSAGENODE_H
#define TEXTMESSAGENODE_H

#include "../../base/Worker.h"

#include "../../../messages/TextMessage/TextMessagePubSubTypes.h"

class TextMessageNode : public Worker {
public:
	TextMessageNode();
	~TextMessageNode();

private:
	bool init();
	bool run();
	void cleanup();

	Publisher<TextMessage, TextMessagePubSubType>* textMessagePublisher_;
	Subscriber<TextMessage, TextMessagePubSubType>* textMessageSubscriber_;
};

#endif // TEXTMESSAGENODE_H