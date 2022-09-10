#include "../../base/Node.h"
#include "../../base/Publisher.h"
#include "../../base/Subscriber.h"

#include "../../../messages/TextMessage/TextMessagePubSubTypes.h"

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