#include <iostream>
#include <algorithm>

#include "TextMessageNode.h"

TextMessageNode::TextMessageNode()
	: textMessagePublisher_("TextMessageTopic", "TextMessage", new TextMessagePubSubType())
	, textMessageSubscriber_("TextMessageTopic", "TextMessage", new TextMessagePubSubType())
{}

bool TextMessageNode::init() {
	textMessagePublisher_.init();
	textMessageSubscriber_.init();
	return true;
}

bool TextMessageNode::run() {
	TextMessage tm_;
	tm_.message("Hello");

	uint32_t samples = 5;
	uint32_t sent = 0;

	while (sent < samples) {
		if (textMessagePublisher_.publish(tm_)) {
			sent++;
			std::cout << "Message: " << tm_.message() << " SENT" << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		while (textMessageSubscriber_.getNumMessages() > 0) {
			TextMessage received = textMessageSubscriber_.popOldestMessage();
			std::cout << "Message: " << received.message() << " RECEIVED" << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	return true;
}