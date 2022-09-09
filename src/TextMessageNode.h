#include "Node.h"
#include "Publisher.h"
#include "Subscriber.h"

#include "TextMessagePubSubTypes.h"

class TextMessageNode : public Node {
public:
	TextMessageNode();
	~TextMessageNode();
	bool init();
	bool run();

private:
	Publisher<TextMessage, TextMessagePubSubType>* _textMessagePublisher;
	Subscriber<TextMessage, TextMessagePubSubType>* _textMessageSubscriber;
};