#include <iostream>

#include "TextMessageNode.h"

TextMessageNode::TextMessageNode() 
	: Worker()
	, textMessagePublisher_(nullptr)
	, textMessageSubscriber_(nullptr)
{}

TextMessageNode::~TextMessageNode() {
	cleanup();
}

bool TextMessageNode::init() {
	if (initialized_) {
		return false;
	}
	textMessagePublisher_ = new Publisher<TextMessage, TextMessagePubSubType>("TextMessageTopic", "TextMessage", new TextMessagePubSubType());
	textMessageSubscriber_ = new Subscriber<TextMessage, TextMessagePubSubType>("TextMessageTopic", "TextMessage", new TextMessagePubSubType());

	textMessagePublisher_->init();
	textMessageSubscriber_->init();

	initialized_ = true;
	return true;
}

bool TextMessageNode::run() {
	if (!initialized_) {
		return false;
	}

	TextMessage tm_;
	tm_.message("Hello");

	uint32_t samples = 5;
	uint32_t sent = 0;

	while (sent < samples) {
		if (textMessagePublisher_->publish(tm_)) {
			sent++;
			std::cout << "Message: " << tm_.message() << " SENT" << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		while (textMessageSubscriber_->getNumMessages() > 0) {
			TextMessage received = textMessageSubscriber_->popOldestMessage();
			std::cout << "Message: " << received.message() << " RECEIVED" << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	return true;
}

void TextMessageNode::cleanup() {
	if (initialized_) {
		delete(textMessagePublisher_);
		delete(textMessageSubscriber_);
	}
	initialized_ = false;
}