#include <iostream>

#include "TextMessageNode.h"

TextMessageNode::TextMessageNode() 
	: Node()
	, textMessagePublisher_(nullptr)
	, textMessageSubscriber_(nullptr)
{}

TextMessageNode::~TextMessageNode() {
	if (initialized_) {
		delete(textMessagePublisher_);
	}
}

bool TextMessageNode::init() {
	if (initialized_) {
		return false;
	}
	std::cout << "Node ID: " << getNodeID() << std::endl;

	textMessagePublisher_ = new Publisher<TextMessage, TextMessagePubSubType>("TextMessageTopic", "TextMessage", new TextMessagePubSubType());
	textMessageSubscriber_ = new Subscriber<TextMessage, TextMessagePubSubType>("TextMessageTopic", "TextMessage", new TextMessagePubSubType());

	textMessagePublisher_->init();
	textMessageSubscriber_->init();

	initialized_ = true;
	return true;
}

bool TextMessageNode::standby() {
	if (!initialized_) {
		return false;
	}
	return true;
}

bool TextMessageNode::run() {
	if (!initialized_) {
		return false;
	}

	TextMessage tm_;
	tm_.index(0);
	tm_.message("Hello");

	uint32_t samples = 10;
	uint32_t sent = 0;

	while (sent < samples) {
		
		if (textMessagePublisher_->publish(tm_)) {
			sent++;
			std::cout << "Message: " << tm_.message() << " with index: " << tm_.index()
				<< " SENT" << std::endl;
			tm_.index(tm_.index() + 1);
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		while (textMessageSubscriber_->getNumMessages() > 0) {
			TextMessage received = textMessageSubscriber_->popOldestMessage();
			std::cout << "Message: " << received.message() << ", with index: " << received.index()
				<< " RECEIVED" << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	}
	return true;
}